
#ifndef PROTOCOLO_HPP
#define PROTOCOLO_HPP

#include "trama.hpp"
#include "primitivas.hpp"
#include <fstream>
using namespace std;

/*
    Seleccion
    MAESTRO                               ESCLAVO
    
    ********* Fase de conexion **********
    Peticion conexion ENQ --------------> Recibida peticion (Comprueba que es R o T y decide si recibir o mandar)
    Confirmacion conexion <-------------- Confirmacion conexion positiva 

    ********* Fase de transferencia *****
    Envio de dato ----------------------> Recibido envio de datos
    Confirmacion recibida <-------------- Confirmacion de recepcion ACK y almacenamiento si trama correcta (No hay errores todavia --> Error)
    Envio de datos ---------------------> Recibido envio de datos
    Confirmacion recibida <-------------- Confirmacion de recepcion ACK y almacenamiento 
    ....
    ********* Fase de cierre ************
    Peticion de desconexion EOT --------> Recibida peticion y cierre de fichero 
    Confirmacion de cierre <------------- Envio de confirmacion de cierre   


    F3                                    F3
    1 Seleccion                           
    2 Sondeo 
    1.1 
*/



/*Manda un archivo con protocolo de paro y espera*/
void enviarArchivoConProtocolo(interface_t * iface, Metadata * metadata, bool esMaestro);

/*Ejecuta la lógica del maestro en mecanismo de selección*/
/*
    Manda trama de control ENQ y num de secuencia 0
    Ejecuta seleccion o sondeo segun se elija
    Manda petición de conexion
    Espera peticion de conexion
    Elige modo
    En seleccion: 
        Mientras no se termine de leer archivo
            Mandar trama con BCE 
            Esperar confirmacion de recepcion ACK
        Manda peticion de cierre de conexion EOT
*/
//void ejecucionMaestroSeleccion(interface_t * iface, Metadata * metadata);

/*Ejecuta la lógica del esclavo en mecanismo de selección*/
/*
    Espera a recibir una trama de control ENQ y num de secuencia 0
    Ejecuta seleccion o sondeo segun
    En seleccion: 
        Manda confirmacion de conexion 
        Abre fichero
        Mientras no se reciba trama de cierre
            Espera a recibir datos
            Recibe datos
            Comprueba BCE y almacena datos si es correcto
            Manda confirmacion de recepcion de datos ACK
        Cierre de fichero 
        Confirmacion de cierre

*/
//void ejecucionEsclavoSeleccion(interface_t * iface, Metadata * metadata);

//void ejecucionMaestroSondeo(interface_t * iface, Metadata * metadata);

//void ejecucionEsclavoSondeo(interface_t * iface, Metadata * metadata);

#endif