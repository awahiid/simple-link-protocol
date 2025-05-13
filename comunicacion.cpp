
#include "comunicacion.hpp"

void envioCaracteresInteractivo(interface_t *iface, Metadata *metadata){
    unsigned char dato;
    char dato_recibido;

    printf("Pulse los caracteres a enviar:\n");

    apacket_t trama_recibida;

    while (dato != 27)
    { // dato != ESC
        trama_recibida = ReceiveFrame(iface);

        if (trama_recibida.packet != nullptr){
            dato_recibido = trama_recibida.packet[14];
            printf("Recibido: %c\n", dato_recibido);
        }

        if (kbhit()){
            dato = getch();
            if (dato != 27){
                enviarDato(iface, metadata, &dato);
            }
        }
    }
}

void enviarArchivoSinProtocolo(interface_t *iface, Metadata *metadata){
    char linea_fichero[255];

    ifstream fichero;
    fichero.open("Envio.txt");

    if (!fichero.is_open()){
        printf("Error al abrir el fichero.\n");
    }else{
        printf("El archivo se ha abierto correctamente.\n");

        while (fichero.peek() != EOF){

            fichero.get(linea_fichero, 255);
            int long_linea = strlen(linea_fichero)+1;
            
            char cadena[long_linea];
            for (int i = 0; i < long_linea; i++){
                cadena[i] = linea_fichero[i];
            }

            enviarTramaDatosSinProtocolo(iface, metadata, cadena);
        }
    }
}
