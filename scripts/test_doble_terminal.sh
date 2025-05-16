#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
VETH_A="slp-test0"
VETH_B="slp-test1"
DRY_RUN=0
KEEP_NET=0
SUDO_PREFIX=""
SUDO_CMD=()

usage() {
  cat <<'EOF'
Uso: scripts/test_doble_terminal.sh [--dry-run] [--keep-net]

Crea dos interfaces virtuales distintas y abre dos terminales con el binario
para probar la ejecución en la misma máquina.

Opciones:
  --dry-run   Muestra las acciones sin crear red ni abrir terminales.
  --keep-net  No borra la red virtual al salir.
EOF
}

log() {
  printf '%s\n' "$*"
}

need_cmd() {
  command -v "$1" >/dev/null 2>&1 || {
    printf 'Falta el comando requerido: %s\n' "$1" >&2
    exit 1
  }
}

cleanup() {
  if [[ "$DRY_RUN" -eq 1 || "$KEEP_NET" -eq 1 ]]; then
    return
  fi

  run_ip link del "$VETH_A" >/dev/null 2>&1 || true
}

trap cleanup EXIT INT TERM

terminal_cmd() {
  local terminal="$1"
  local inner="$2"
  case "$terminal" in
    gnome-terminal)
      printf 'gnome-terminal -- bash -lc %q' "$inner"
      ;;
    konsole)
      printf 'konsole --hold -e bash -lc %q' "$inner"
      ;;
    xfce4-terminal)
      printf 'xfce4-terminal --hold --command=%q' "bash -lc '$inner'"
      ;;
    xterm|x-terminal-emulator)
      printf 'xterm -hold -e bash -lc %q' "$inner"
      ;;
    *)
      return 1
      ;;
  esac
}

launch_terminal() {
  local terminal="$1"
  local title="$2"
  local cmd="$3"

  case "$terminal" in
    gnome-terminal)
      gnome-terminal --title="$title" -- bash -lc "$cmd"
      ;;
    konsole)
      konsole --hold -p tabtitle="$title" -e bash -lc "$cmd"
      ;;
    xfce4-terminal)
      xfce4-terminal --title="$title" --hold --command="bash -lc '$cmd'"
      ;;
    xterm|x-terminal-emulator)
      xterm -T "$title" -hold -e bash -lc "$cmd"
      ;;
    *)
      return 1
      ;;
  esac
}

choose_terminal() {
  for term in gnome-terminal konsole xfce4-terminal xterm x-terminal-emulator; do
    if command -v "$term" >/dev/null 2>&1; then
      printf '%s\n' "$term"
      return 0
    fi
  done
  return 1
}

create_veth() {
  log "Creando red virtual: $VETH_A <-> $VETH_B"
  run_ip link del "$VETH_A" >/dev/null 2>&1 || true
  run_ip link add "$VETH_A" type veth peer name "$VETH_B"
  run_ip link set "$VETH_A" up
  run_ip link set "$VETH_B" up
}

run_ip() {
  if [[ ${#SUDO_CMD[@]} -gt 0 ]]; then
    "${SUDO_CMD[@]}" ip "$@"
  else
    ip "$@"
  fi
}

main() {
  while [[ $# -gt 0 ]]; do
    case "$1" in
      --dry-run)
        DRY_RUN=1
        ;;
      --keep-net)
        KEEP_NET=1
        ;;
      -h|--help)
        usage
        exit 0
        ;;
      *)
        printf 'Opción desconocida: %s\n' "$1" >&2
        usage >&2
        exit 1
        ;;
    esac
    shift
  done

  need_cmd make
  need_cmd ip

  local terminal
  terminal="$(choose_terminal || true)"
  if [[ -z "$terminal" ]]; then
    printf 'No se encontró ningún emulador de terminal compatible (gnome-terminal, konsole, xfce4-terminal, xterm, x-terminal-emulator).\n' >&2
    exit 1
  fi

  if [[ "$(id -u)" -ne 0 && -z "$(command -v sudo 2>/dev/null || true)" ]]; then
    printf 'Necesitas ejecutar como root o tener sudo disponible para crear la pareja veth.\n' >&2
    exit 1
  fi

  if [[ "$(id -u)" -ne 0 ]]; then
    SUDO_PREFIX="sudo -E"
    SUDO_CMD=(sudo -E)
  fi

  log "Terminal detectado: $terminal"
  log "Interfaces esperadas: $VETH_A y $VETH_B"

  if [[ "$DRY_RUN" -eq 1 ]]; then
    log "Modo dry-run: no se crean interfaces ni se abren terminales."
    log "Comandos que se ejecutarían:"
    log "  ${SUDO_PREFIX:+$SUDO_PREFIX }ip link add $VETH_A type veth peer name $VETH_B"
    log "  ${SUDO_PREFIX:+$SUDO_PREFIX }ip link set $VETH_A up"
    log "  ${SUDO_PREFIX:+$SUDO_PREFIX }ip link set $VETH_B up"
    log "  ${SUDO_PREFIX:+$SUDO_PREFIX }./build/simple-link-protocol en dos terminales"
    exit 0
  fi

  make -C "$ROOT_DIR"
  create_veth

  local cmd_a cmd_b
  cmd_a="cd '$ROOT_DIR' && printf 'Elige interfaz $VETH_A\n'; ${SUDO_PREFIX:+$SUDO_PREFIX }./build/simple-link-protocol; exec bash"
  cmd_b="cd '$ROOT_DIR' && printf 'Elige interfaz $VETH_B\n'; ${SUDO_PREFIX:+$SUDO_PREFIX }./build/simple-link-protocol; exec bash"

  log "Abriendo dos terminales..."
  launch_terminal "$terminal" "SLP A ($VETH_A)" "$cmd_a" &
  launch_terminal "$terminal" "SLP B ($VETH_B)" "$cmd_b" &
  wait || true

  log "Listo. En cada terminal selecciona la interfaz indicada cuando aparezca el menú."
  log "Pulsa Ctrl+C aquí para limpiar la red virtual, o usa --keep-net para conservarla."
  read -r -p "Pulsa Enter para limpiar y salir..." _
}

main "$@"

