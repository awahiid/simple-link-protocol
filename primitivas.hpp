/*
    Primitivas básicas para establecer y gestionar la comunicación.
*/

#include "linkLayer.h"
#include "metadata.hpp"
#include "trama.hpp"
#include <string>
using namespace std;
#ifndef PRIMITIVAS_HPP
#define PRIMITIVAS_HPP

#define ENQ 5
#define EOT 4
#define ACK 6
#define NACK 21
#define STX 2

/**
 * @brief Formatea la primitiva a una cadena de texto.
 * 
 * @param primitiva La primitiva a formatear.
 */
char * formatearPrimitiva(unsigned char primitiva);

/**/
/**
 * @brief Envía una petición de conexión.
 * 
 * @param iface La interfaz a través de la cual se enviará la petición.
 * @param metadata Metadatos asociados con la comunicación.
 * @param dir La dirección a la que se enviará la petición.
 */
void enviarPeticionConexion(interface_t *iface, Metadata *metadata, char dir);

/**
 * @brief Envía una petición de cierre de conexión.
 * 
 * @param iface La interfaz a través de la cual se enviará la petición.
 * @param metadata Metadatos asociados con la comunicación.
 * @param dir La dirección a la que se enviará la petición.
 */
void enviarPeticionCierre(interface_t *iface, Metadata *metadata, char dir,char num_sec);

/*RESPUESTAS*/
/**
 * @brief Envía una respuesta de confirmación.
 * 
 * @param iface La interfaz a través de la cual se enviará la petición.
 * @param metadata Metadatos asociados con la comunicación.
 * @param trama_recibida La trama recibida a la que se va a responder.
 */
void enviarConfirmacion(interface_t *iface , Metadata *metadata , apacket_t trama_recibida);

/**
 * @brief Envía una respuesta de negación.
 * 
 * @param iface La interfaz a través de la cual se enviará la petición.
 * @param metadata Metadatos asociados con la comunicación.
 * @param trama_recibida La trama recibida a la que se va a responder.
 */
void enviarRetransmision(interface_t *iface , Metadata *metadata , apacket_t trama_recibida);

/**
 * @brief Verifica si se ha recibido una petición de conexión.
 * 
 * @param metadata Metadatos asociados con la comunicación.
 * @param trama_recibida La trama recibida que se va a verificar.
 * @param dir La dirección esperada en la trama.
 * @param num_secuencia El número de secuencia esperado en la trama.
 * @return Verdadero si se ha recibido una petición de conexión, falso en caso contrario.
 */
bool seHaRecibidoConexion(Metadata *metadata, apacket_t trama_recibida, char dir);

/**
 * @brief Verifica si se ha recibido una petición de cierre de conexión.
 * 
 * @param metadata Metadatos asociados con la comunicación.
 * @param trama_recibida La trama recibida que se va a verificar.
 * @param dir La dirección esperada en la trama.
 * @return Verdadero si se ha recibido una petición de cierre de conexión, falso en caso contrario.
 */
bool seHaRecibidoCierre(Metadata *metadata, apacket_t trama_recibida, char dir,char num_sec);

/**
 * @brief Verifica si se ha recibido una confirmación.
 * 
 * @param metadata Metadatos asociados con la comunicación.
 * @param trama_recibida La trama recibida que se va a verificar.
 * @param dir La dirección esperada en la trama.
 * @param num_secuencia El número de secuencia esperado de la trama recibida.
 * @return Verdadero si se ha recibido una confirmación, falso en caso contrario.
 */
bool seHaRecibidoConfirmacion(Metadata *metadata, apacket_t trama_recibida, char dir, char num_secuencia);

/**
 * @brief Verifica si se ha recibido una negación.
 * 
 * @param metadata Metadatos asociados con la comunicación.
 * @param trama_recibida La trama recibida que se va a verificar.
 * @param dir La dirección esperada en la trama.
 * @param num_secuencia El número de secuencia esperado de la trama recibida.
 * @return Verdadero si se ha recibido una negación, falso en caso contrario.
 */
bool seHaRecibidoRetransmision(Metadata *metadata, apacket_t trama_recibida, char dir, char num_secuencia);

bool seHaRecibidoTramaDatos(Metadata *metadata, apacket_t trama_recibida, char dir, char num_secuencia);

#endif