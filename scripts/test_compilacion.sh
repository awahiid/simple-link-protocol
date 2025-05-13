#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
OUTPUT="$BUILD_DIR/simple-link-protocol"

mkdir -p "$BUILD_DIR"

if ! command -v g++ >/dev/null 2>&1; then
  echo "Error: g++ no está disponible en el sistema." >&2
  exit 1
fi

if [[ ! -f /usr/include/pcap.h && ! -f /usr/include/pcap/pcap.h ]]; then
  echo "No se puede ejecutar la prueba: falta el encabezado pcap.h (instala libpcap-dev o equivalente)." >&2
  exit 2
fi

if ! command -v pkg-config >/dev/null 2>&1; then
  echo "Aviso: pkg-config no está disponible; se compilará directamente contra libpcap si existe." >&2
fi

if ! ldconfig -p 2>/dev/null | grep -q 'libpcap'; then
  echo "Aviso: no se detecta libpcap en el cargador dinámico; la compilación puede fallar si falta el desarrollo." >&2
fi

SRC_FILES=("$ROOT_DIR"/*.cpp)

printf 'Compilando %s...\n' "$OUTPUT"

g++ -std=c++17 -Wall -Wextra -O2 -I"$ROOT_DIR" \
  "${SRC_FILES[@]}" "$ROOT_DIR/linkLayer.a" \
  -lpcap -pthread -o "$OUTPUT"

printf 'Compilación correcta: %s\n' "$OUTPUT"

