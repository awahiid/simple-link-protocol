
#ifndef COMUNICACION_HPP
#define COMUNICACION_HPP

#include <stdio.h>
#include <iostream>
#include "linkLayer.h"
#include "link.h"
#include "kbhit.h"
#include "metadata.hpp"
#include "trama.hpp"
#include "protocolo.hpp"
#include "primitivas.hpp"

// Función para enviar caracteres de forma interactiva
void envioCaracteresInteractivo(interface_t * iface, Metadata * metadata);

// Función para enviar un archivo sin protocolo
void enviarArchivoSinProtocolo(interface_t * iface, Metadata * metadata);


#endif //COMUNICACION_HPP