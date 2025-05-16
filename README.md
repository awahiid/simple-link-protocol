# simple-link-protocol

Proyecto C++ para practicar comunicación entre interfaces de red usando tramas Ethernet y control de enlace.

## Requisitos
- `g++`
- `make`
- `libpcap` y sus cabeceras (`libpcap-dev` en Debian/Ubuntu)

## Compilación
```bash
make
```

## Ejecución
La forma más simple de ejecutarlo es:
```bash
make run
```

Si prefieres lanzar el binario manualmente, queda en:
```bash
build/simple-link-protocol
```

## Prueba rápida
Si quieres comprobar la compilación con diagnóstico claro:
```bash
./scripts/test_compilacion.sh
```

## Prueba con dos terminales
Para crear una pareja `veth` y abrir dos terminales con el programa:
```bash
./scripts/test_doble_terminal.sh
```

