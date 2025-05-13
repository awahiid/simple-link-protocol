/*
    Estructura de metadatos de una trama y utilidades asociadas.
*/

#ifndef METADATA_HPP
#define METADATA_HPP

#include <stdio.h>
#include <iostream>
#include <cstring>
using namespace std;

/**
 * @brief Estructura para almacenar metadatos de un paquete de red
 */
typedef struct metadata {
    unsigned char mac_src[6]; ///< Dirección MAC de origen
    unsigned char mac_dst[6]; ///< Dirección MAC de destino
    unsigned char type[2];    ///< Tipo de paquete
} Metadata;


/**
 * @brief Muestra la direccion MAC.
 * 
 * @param mac MAC a mostrar.
 */
void mostrarDireccionMAC(unsigned char mac[6]);

/**
 * @brief Cambia la dirección MAC de origen en los metadatos de un paquete de red
 * 
 * @param metadata Puntero a la estructura de metadatos
 * @param new_mac_src Nueva dirección MAC de origen
 */
void cambiarMACOrigen(Metadata *metadata, unsigned char *new_mac_src);
/**
 * @brief Cambia la dirección MAC de destino en los metadatos de un paquete de red
 * 
 * @param metadata Puntero a la estructura de metadatos
 * @param new_mac_src Nueva dirección MAC de destino
 */
void cambiarMACDestino(Metadata *metadata, const unsigned char *new_mac_src);

/**
 * @brief Muestra la dirección MAC de origen almacenada en los metadatos
 * 
 * @param metadata Puntero a la estructura de metadatos
 */
void mostrarMACOrigen(Metadata *metadata);

/**
 * @brief Muestra la dirección MAC de destino almacenada en los metadatos
 * 
 * @param metadata Puntero a la estructura de metadatos
 */
void mostrarMACDestino(Metadata *metadata);

/**
 * @brief Muestra el tipo de paquete almacenada en los metadatos
 * 
 * @param metadata Puntero a la estructura de metadatos
 */
void mostrarTipo(Metadata *metadata);

#endif 
