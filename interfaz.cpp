#include "interfaz.hpp"

int errorAbrirAdaptador(){
    printf("Error al abrir el adaptador.");
    return 1;
}

void abrirAdaptador(interface_t *iface){
    if (OpenAdapter(iface)){
        errorAbrirAdaptador();
    }else{
        printf("Puerto abierto correctamente\n");
    }
}

void mostrarAdaptadoresDisponibles(pcap_if_t *lista_interfaces, int *tam){

    pcap_if_t *lista_auxiliar = lista_interfaces;

    printf("Interfaces disponibles: \n");

    for (*tam = 0; lista_auxiliar != NULL; (*tam)++){
        printf("[%d]", *tam);
        printf("%s\n", lista_auxiliar->name);
        lista_auxiliar = lista_auxiliar->next;
    }
}

void seleccionarInterfaz(pcap_if_t *lista_interfaces, interface_t *iface, int num_interfaces){
    pcap_if_t *lista_auxiliar = lista_interfaces;
    int indice_seleccionado = -1;

    while (indice_seleccionado < 0 || indice_seleccionado >= num_interfaces){
        printf("Seleccione interfaz: ");
        if (scanf("%i", &indice_seleccionado) != 1){
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }

    while (lista_auxiliar != NULL && indice_seleccionado > 0){
        indice_seleccionado--;
        lista_auxiliar = lista_auxiliar->next;
    }

    if (lista_auxiliar != NULL){
        setDeviceName(iface, lista_auxiliar->name);
        GetMACAdapter(iface);
    }else{
        printf("Índice de interfaz fuera de rango.\n");
    }
}

void mostrarYSeleccionarInterfaz(interface_t *iface){
    pcap_if_t *lista_interfaces = NULL;
    lista_interfaces = GetAvailAdapters();
    int num_interfaces;

    mostrarAdaptadoresDisponibles(lista_interfaces, &num_interfaces);

    seleccionarInterfaz(lista_interfaces, iface, num_interfaces);

    printf("Interfaz elegida: %s\n", iface->deviceName);
}

int seleccinarGrupo(Metadata *metadata){
    int grupo;
    printf("Introduzca el grupo al que pertenezca\n");
    while (grupo < 0 || grupo >= 10){
        cin >> grupo;
    }
    metadata->type[0] = 0x30 + grupo;
    return grupo;
}

bool seleccionarModo(interface_t *iface, Metadata *metadata,int grupo){

    bool esMaestro = false;
    char seleccion;

    printf("Selecciona el modo de la estación: \n");

    printf("[1] Modo Maestra \n");
    printf("[2] Modo Esclava \n");

    while (seleccion != '1' && seleccion != '2'){
        seleccion = getchar();
        if (seleccion == '1'){
            esMaestro = true;
        }
    }

    printf("\n");
    metadata->type[1] = (esMaestro) ? 0x01 : 0x02;
    const unsigned char tipoEsperado = (esMaestro) ? 0x02 : 0x01;
    const char *mensajeEsperado = (esMaestro) ? "Esclava" : "Maestra";
    printf("Esperando que se una la estación %s\n", mensajeEsperado);

    if (esMaestro){
        memset(metadata->mac_dst, 0xFF, 6 * sizeof(unsigned char));
        enviarTramaVacia(iface, metadata);
    }

    bool confirmacionRecibida = false;

    FreeBuffer();
    while (!confirmacionRecibida){
        apacket_t paquete_recibido = ReceiveFrame(iface);
        if (paquete_recibido.packet != nullptr && paquete_recibido.packet[12] == 0x30+grupo 
        /* en vez de 0x30 + grupo se puede poner type->[0] creo*/ 
        && paquete_recibido.packet[13] == tipoEsperado){
            confirmacionRecibida = true;
            const unsigned char *mac_destino = &paquete_recibido.packet[6];
            cambiarMACDestino(metadata, mac_destino);
            if (!esMaestro)
                enviarTramaVacia(iface, metadata);
            printf("Estación encontrada. La MAC es: ");
            mostrarMACDestino(metadata);
            printf("\n");
        }
    }

    metadata->type[1] = 0x00;

    return esMaestro;
}

void mostrarFunciones(bool esMaestro){
    printf("Seleccione opción: \n");
    printf("[F1] - Envío de carácteres interactivo\n");
    if (esMaestro)
        printf("[F2] - Envío de un fichero\n");
    printf("[F3] - Protocolo paro y espera\n");
    printf("[ESC] - Salir\n");
}

char seleccionarOpcion(){
    char car = 0;
    // Devuelve en car O, P, Q, R, S
    // si no se ha recibido nada devuelve 0
    car = getch();
    if (car == 27 && kbhit()){
        car = getch();
        if (car == 'O' && kbhit()){
            car = getch();
        }
        else{
            car = 0;
        }
    }
    return car;
}


void seleccionarYEjecutarFuncion(interface_t *iface, Metadata *metadata, bool esMaestro){
    bool interactivo;
    bool ejecutado =false;
    char car;
    //Limpiamos el input
    while ((car = getchar()) != '\n' && car != EOF) { }
    car = 0;

    mostrarFunciones(esMaestro);

    while (car != 27){

        if (!esMaestro && car != 27){ 
            // Opcion solo para esclavo
            // Comprueba constantemente si le ha llegado una trama y la muestra
            // Puede recibir un solo dato, varios datos de un archivo o protocolo
            apacket_t trama_recibida = ReceiveFrame(iface);
            if (trama_recibida.packet != nullptr){
                bool esMiTrama = memcmp(metadata->mac_src, obtenerMACDst(trama_recibida), 6) == 0;
                if (esMiTrama && (car == 0)){
                    mostrarDatosTramaSinProtocolo(trama_recibida);
                    printf("\n");
                }
            }
        }

        if (kbhit()){
            car = seleccionarOpcion();
            printf("\n");
            switch (car){
                case ('P'): // Se ha pulsado F1
                    // Lo ejecutan ambos
                    envioCaracteresInteractivo(iface, metadata);
                    mostrarFunciones(esMaestro);
                    car = 0;
                    break;
                case ('Q'): // Se ha pulsado F2
                    if (esMaestro){
                        // Opcion solo para maestro
                        enviarArchivoSinProtocolo(iface, metadata);
                        mostrarFunciones(esMaestro);
                        car = 0;
                    }
                    break;
                case ('R'): // Se ha pulsado F3
                    // Lo ejecutan ambos
                    enviarArchivoConProtocolo(iface, metadata, esMaestro);
                    mostrarFunciones(esMaestro);
                    car = 0;
                    break;
                default:
                    break;
            }
        }
    }
}
