#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

if ! command -v make >/dev/null 2>&1; then
  echo "Error: make no está disponible en el sistema." >&2
  exit 1
fi

printf 'Ejecutando make en %s...\n' "$ROOT_DIR"
make -C "$ROOT_DIR" clean
make -C "$ROOT_DIR"

printf 'Compilación correcta con make.\n'

