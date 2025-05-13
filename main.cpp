#include "interfaz.hpp"

int main(){
    int grupo;
    
    printf("----------------------------\n");
    printf("-- simple-link-protocol --\n");
    printf("----------------------------\n");

    interface_t iface;

    // Selección de interfaz
    mostrarYSeleccionarInterfaz(&iface);

    // Apertura del adaptador
    abrirAdaptador(&iface);

    // Metadatos iniciales
    Metadata metadata = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // mac_src
        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05}, // mac_dst
        {0x30, 0x00}                          // type
    };

    cambiarMACOrigen(&metadata, iface.MACaddr);

    printf("La MAC es: "); mostrarMACOrigen(&metadata);

    // Selección de grupo
    grupo=seleccinarGrupo(&metadata);
    printf("\n");
    bool esMaestro = seleccionarModo(&iface, &metadata,grupo);

    // Ejecución del protocolo
    seleccionarYEjecutarFuncion(&iface, &metadata, esMaestro);
    
    return 0;
}