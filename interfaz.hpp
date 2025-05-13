/*
   Funciones de interfaz de usuario y control de la aplicación.
*/

#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

#include <stdio.h>
#include <iostream>
#include "linkLayer.h"
#include "link.h"
#include "kbhit.h"
#include "metadata.hpp"
#include "trama.hpp"
#include "protocolo.hpp"
#include "primitivas.hpp"
#include "comunicacion.hpp"

// Función para manejar el error al abrir el adaptador de red
int errorAbrirAdaptador();

// Función para abrir el adaptador de red
void abrirAdaptador(interface_t * iface);

// Función para mostrar las interfaces de red disponibles
void mostrarAdaptadoresDisponibles(pcap_if_t * lista_interfaces, int * tam);

// Función para seleccionar una interfaz de red
void seleccionarInterfaz(pcap_if_t * lista_interfaces, interface_t * iface, int num_interfaces);

// Función para mostrar las interfaces de red disponibles y seleccionar una
void mostrarYSeleccionarInterfaz(interface_t * iface);

// Función para seleccionar el modo de la estación (maestra o esclava)
bool seleccionarModo(interface_t * iface, Metadata * metadata,int grupo);

// Función para mostrar las funciones disponibles en función del modo seleccionado
void mostrarFunciones(bool esMaestro);

// Función para seleccionar una opción del menú
char seleccionarOpcion();

// Función para seleccionar y ejecutar una función en función del modo seleccionado
void seleccionarYEjecutarFuncion(interface_t * iface, Metadata * metadata, bool esMaestro);

//Funcion para seleccionar el grupo al que pertenece el alumno
int seleccinarGrupo(Metadata * metadata);

#endif
