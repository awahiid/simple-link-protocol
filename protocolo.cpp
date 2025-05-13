
#include "protocolo.hpp"
#include "interfaz.hpp"

int seleccionarMecanismo()
{
    int mecanismo;
    printf("Seleccione el tipo de operación:\n");
    printf("[1]Operación selección.\n[2]Operación sondeo.\n[3]Salir.");

    scanf("%d", &mecanismo);

    return mecanismo;
}

void solicitarConexion(interface_t *iface, Metadata *metadata, char dir){

    enviarPeticionConexion(iface, metadata, dir);
    apacket_t trama_recibida = ReceiveFrame(iface);

    while (!seHaRecibidoConfirmacion(metadata, trama_recibida, dir, '0')){
        trama_recibida = ReceiveFrame(iface);
    }

}
char esperarConexion(interface_t * iface, Metadata *metadata){

    apacket_t trama_recibida = ReceiveFrame(iface);

    while (trama_recibida.packet == nullptr || !seHaRecibidoConexion(metadata, trama_recibida, obtenerDireccion(trama_recibida))){
        trama_recibida = ReceiveFrame(iface);
    }

    enviarConfirmacion(iface, metadata, trama_recibida);
    
    return obtenerDireccion(trama_recibida);
}

char *introducirError(const char *datos)
{
    printf("INTRODUCIENDO ERROR\n");
    char *datos_error = strdup(datos);
    datos_error[0] = 184;
    return datos_error;
}

void faseDeEnvio(interface_t *iface, Metadata *metadata, bool esMaestro, char dir)
{

    if (esMaestro && dir == 'R' || !esMaestro && dir == 'T'){ 
        // Para maestro en seleccion o esclavo en sondeo.
        ifstream fichero;
        fichero.open("EProtoc.txt");
        char linea_fichero[255];
        char num_secuencia = '0';
        apacket_t trama_recibida;
        int BCE;
        int errores = 0;
        char *linea_error;

        while (fichero.peek() != EOF){
            if (kbhit())
                if (seleccionarOpcion() == 'S')
                    errores++;

            fichero.get(linea_fichero, 255);
            BCE = calcularBCE(linea_fichero, fichero.gcount());

            if (errores > 0){
                linea_error = introducirError(linea_fichero);
                enviarTramaDatosConProtocolo(iface, metadata, dir, num_secuencia, fichero.gcount(), linea_error, BCE);
                errores--;
                free(linea_error);
            } else{
                enviarTramaDatosConProtocolo(iface, metadata, dir, num_secuencia, fichero.gcount(), linea_fichero, BCE);
            }

            trama_recibida = ReceiveFrame(iface);

            while (!seHaRecibidoConfirmacion(metadata, trama_recibida, dir, num_secuencia)){

                if (seHaRecibidoRetransmision(metadata, trama_recibida, dir, num_secuencia)){
                    enviarTramaDatosConProtocolo(iface, metadata, dir, num_secuencia, fichero.gcount(), linea_fichero, BCE);
                }

                if (kbhit())
                    if (seleccionarOpcion() == 'S')
                        errores++;

                trama_recibida = ReceiveFrame(iface);
            }

            num_secuencia = (num_secuencia == '0') ? '1' : '0';
        }

        fichero.close();
    }
    else if (!esMaestro && dir == 'R' || esMaestro && dir == 'T'){ 
        // Para maestro en sondeo o esclavo en seleccion

        ofstream fichero;
        fichero.open("RProtoc.txt");

        apacket_t trama_recibida = ReceiveFrame(iface);
        char num_cierre = '0';
        char num_secuencia = '0';

        while (true){

            if (seHaRecibidoCierre(metadata, trama_recibida, dir, num_cierre)){

                if(!esMaestro){
                    enviarConfirmacion(iface, metadata, trama_recibida);
                    printf("\n");
                    fichero.close();
                    return;
                }

                printf("¿Aceptas la petición de cierre?\n");
                printf("[1] Si.\n");
                printf("[2] No.\n");

                int decision;
                scanf("%d", &decision);
                if(decision == 1){
                    enviarConfirmacion(iface, metadata, trama_recibida);
                    printf("\n");
                    fichero.close();
                    return;
                }else{
                    enviarRetransmision(iface, metadata, trama_recibida);
                    num_cierre = (num_cierre == '0') ? '1' : '0';
                }

            }else if (seHaRecibidoTramaDatos(metadata, trama_recibida, dir, num_secuencia)){
                
                mostrarTrama('R', dir, formatearPrimitiva(obtenerPrimitiva(trama_recibida)), obtenerNumSecuencia(trama_recibida), obtenerBCE(trama_recibida), calcularBCE(obtenerDatos(trama_recibida), obtenerLongitud(trama_recibida)));
                if (!hayErroresTrama(trama_recibida)){
                    enviarConfirmacion(iface, metadata, trama_recibida);
                    fichero.write((char *)obtenerDatos(trama_recibida), obtenerLongitud(trama_recibida));
                    num_secuencia = (num_secuencia == '0') ? '1' : '0';
                }
                else{
                    enviarRetransmision(iface, metadata, trama_recibida);
                }

            }

            trama_recibida = ReceiveFrame(iface);
        }
    }
}

/*Manda peticiones de cierre hasta que se aceptan*/
void pedirCierreForzado(interface_t *iface, Metadata *metadata, char dir){
    char num_secuencia = '0';
    apacket_t trama_recibida;
    enviarPeticionCierre(iface, metadata, dir,'0');

    while(trama_recibida.packet == nullptr){        
        trama_recibida = ReceiveFrame(iface);
    }

    while (!seHaRecibidoConfirmacion(metadata, trama_recibida, dir, num_secuencia)){

        if(seHaRecibidoRetransmision(metadata, trama_recibida, dir, num_secuencia)){//NACK
           num_secuencia = (num_secuencia == '0') ? '1' : '0';
           enviarPeticionCierre(iface, metadata, dir, num_secuencia); 
        }

        trama_recibida = ReceiveFrame(iface);
    }
}

void faseDeCierre(interface_t * iface, Metadata * metadata, char dir){

    enviarPeticionCierre(iface, metadata, dir, '0');

    apacket_t trama_recibida = ReceiveFrame(iface);

    while(!seHaRecibidoConfirmacion(metadata,trama_recibida,dir,'0')){
        trama_recibida = ReceiveFrame(iface);
    }
}

void ejecucionMaestroSeleccion(interface_t *iface, Metadata *metadata)
{
    char dir = 'R';

    solicitarConexion(iface, metadata, dir);
    printf("\n");

    faseDeEnvio(iface, metadata, true, dir);

    printf("\n");
    faseDeCierre(iface, metadata, dir);

    printf("Fin de selección por parte de maestro.\n");
}

void ejecucionEsclavoSeleccion(interface_t *iface, Metadata *metadata){

    char dir = 'R';

    faseDeEnvio(iface, metadata, false, dir);

    printf("Fin de selección por parte de esclavo.\n");
}

void ejecucionMaestroSondeo(interface_t *iface, Metadata *metadata)
{
    char dir = 'T';

    solicitarConexion(iface, metadata, dir);
    printf("\n");

    faseDeEnvio(iface, metadata, true, dir);

    printf("Fin de sondeo por parte de maestro.\n");
}

void ejecucionEsclavoSondeo(interface_t *iface, Metadata *metadata)
{
    char dir = 'T';

    faseDeEnvio(iface, metadata, false, dir);

    printf("\n");
    pedirCierreForzado(iface, metadata, dir);

    printf("Fin de sondeo por parte de esclavo.\n");
}

void enviarArchivoConProtocolo(interface_t *iface, Metadata *metadata, bool esMaestro)
{
    printf("Protocolo paro y espera. Para salir pulse ESC.\n");
    
    int mecanismo = 0;

    if (esMaestro){
        mecanismo = seleccionarMecanismo();
        if (mecanismo == 1){
            ejecucionMaestroSeleccion(iface, metadata);
        }else if (mecanismo == 2){
            ejecucionMaestroSondeo(iface, metadata);
        }
    }else{
        printf("Esperando al Maestro\n");
        apacket_t trama_recibida = ReceiveFrame(iface);
        char dir = esperarConexion(iface, metadata);
        printf("\n");
        if(dir == 'R'){
            ejecucionEsclavoSeleccion(iface, metadata);
        }else if(dir == 'T'){
            ejecucionEsclavoSondeo(iface, metadata);
        }
    }
}