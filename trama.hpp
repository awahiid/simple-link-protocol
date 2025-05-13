/*
    Utilidades para el envío, recepción y manejo de tramas.

    Una trama ethernet tiene la siguiente forma:
            0            5    6          11   12    13
        --> MAC fuente (6B) | MAC Dst. (6B) | Tipo (2B)

    A esto se le añade un paquete si es de control:
            14               15               16
        --> Dirección (1B) | Primitiva (1B) | Número de trama (1B)

    Y si es de datos:
            14               15        16                     17              18
        --> Dirección (1B) | 02 (1B) | Número de trama (1B) | Longitud (1B) | Datos (255B) | BCE (1B)
*/

#ifndef TRAMA_HPP
#define TRAMA_HPP

#include "lista.h"
#include "metadata.hpp"
#include "linkLayer.h"
#include <cstring>

using namespace std;

void mostrarTrama(char tipo, char direccion, char * primitiva, char num_secuencia);
void mostrarTrama(char tipo, char direccion, char * primitiva, char num_secuencia, unsigned char BCE);
void mostrarTrama(char tipo, char direccion, char * primitiva, char num_secuencia, unsigned char BCE, unsigned char BCE_calculado);

/**
 * @brief Obtiene la dirección MAC de origen de una trama.
 * 
 * @param trama La trama de la cual se obtendrá la dirección MAC de origen.
 * @return Puntero a la dirección MAC de origen.
 */
unsigned char *obtenerMACFuente(apacket_t trama);

/**
 * @brief Obtiene la dirección MAC de destino de una trama.
 * 
 * @param trama La trama de la cual se obtendrá la dirección MAC de destino.
 * @return Puntero a la dirección MAC de destino.
 */
unsigned char *obtenerMACDst(apacket_t trama);

/**
 * @brief Obtiene el tipo de una trama.
 * 
 * @param trama La trama de la cual se obtendrá el tipo.
 * @return Puntero al tipo de la trama.
 */
unsigned char *obtenerTipo(apacket_t trama);

/**
 * @brief Obtiene la dirección de la trama.
 * 
 * @param trama La trama de la cual se obtendrá la dirección.
 * @return La dirección de la trama.
 */
unsigned char obtenerDireccion(apacket_t trama);

/**
 * @brief Obtiene la primitiva de la trama.
 * 
 * @param trama La trama de la cual se obtendrá la primitiva.
 * @return La primitiva de la trama.
 */
unsigned char obtenerPrimitiva(apacket_t trama);

/**
 * @brief Obtiene el número de secuencia de la trama.
 * 
 * @param trama La trama de la cual se obtendrá el número de secuencia.
 * @return El número de secuencia de la trama.
 */
unsigned char obtenerNumSecuencia(apacket_t trama);

/**
 * @brief Obtiene la longitud de la trama.
 * 
 * @param trama La trama de la cual se obtendrá la longitud.
 * @return La longitud de la trama.
 */
unsigned char obtenerLongitud(apacket_t trama);

/**
 * @brief Obtiene los datos de la trama. NO LE AÑADE \0.
 * 
 * @param trama La trama de la cual se obtendrán los datos.
 * @return Puntero a los datos de la trama.
 */
char *obtenerDatos(apacket_t trama);

/**
 * @brief Obtiene el byte de comprobación de errores (BCE) de la trama.
 * 
 * @param trama La trama de la cual se obtendrá el BCE.
 * @return El BCE de la trama.
 */
unsigned char obtenerBCE(apacket_t trama);

/**
 * @brief Calcula el bloque de comprobación de errores (BCE) de los datos de la trama.
 * 
 * @param datos Los datos de la trama de los cuales se calculará el BCE.
 * @param longitud La longitud de los datos.
 * @return El BCE calculado.
 */
unsigned char calcularBCE(char *datos, int longitud);

/**
 * @brief Comprueba si hay errores en la trama.
 * 
 * @param trama La trama de la cual se obtendrá el BCE.
 * @return True si hay errores y False en caso contrario.
 */
bool hayErroresTrama(apacket_t trama);

/**
 * @brief Envía un único dato.
 * 
 * @param iface La interfaz a través de la cual se enviará la trama.
 * @param metadata Metadatos asociados con la trama.
 * @param dato El dato a enviar.
 */
void enviarDato(interface_t *iface, Metadata *metadata, unsigned char * dato);

/**
 * @brief Envía una trama de datos siguiendo el protocolo.
 * 
 * @param iface La interfaz a través de la cual se enviará la trama.
 * @param metadata Metadatos asociados con la trama.
 * @param direccion Dirección de la trama.
 * @param numTrama Número de trama.
 * @param datos Los datos que se enviarán en la trama.
 * @param BCE El BCE de los datos.
 */
void enviarTramaDatosConProtocolo(interface_t *iface, Metadata *metadata, unsigned char direccion, unsigned char numTrama, int longitud, char * datos, unsigned char BCE);

/**
 * @brief Envía una trama de datos SIN protocolo.
 * 
 * @param iface La interfaz a través de la cual se enviará la trama.
 * @param metadata Metadatos asociados con la trama.
 * @param datos Los datos que se enviarán en la trama.
 */
void enviarTramaDatosSinProtocolo(interface_t *iface, Metadata * metadata, char * dato);

/**
 * @brief Envía una trama de control.
 * 
 * @param iface La interfaz a través de la cual se enviará la trama.
 * @param metadata Metadatos asociados con la trama.
 * @param direccion Dirección de la trama.
 * @param primitiva Primitiva de la trama.
 * @param numTrama Número de trama.
 */
void enviarTramaControl(interface_t *iface, Metadata *metadata, unsigned char direccion, unsigned char primitiva, unsigned char numTrama);

/**
 * @brief Envía una trama vacia (sin paquete).
 * 
 * @param iface La interfaz a través de la cual se enviará la trama.
 * @param metadata Metadatos asociados con la trama.
 */
void enviarTramaVacia(interface_t *iface, Metadata *metadata);

/**
 * @brief Recibe una trama.
 * 
 * @param iface La interfaz a través de la cual se recibirá la trama.
 * @return La trama recibida.
 */
apacket_t recibirTrama(interface_t *iface);

/**
 * @brief Muestra por pantalla los datos de la trama.
 * 
 * @param trama La trama de la cual se obtendrán los datos a mostrar.
 */
void mostrarDatosTrama(apacket_t trama,bool interactivo);

void mostrarDatosTramaSinProtocolo(apacket_t paquete_recibido);

#endif