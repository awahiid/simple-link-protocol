#include "trama.hpp"

void mostrarTrama(char tipo, char direccion, char *primitiva, char num_secuencia) {
    printf("%c   %c   %s   %c\n", tipo, direccion, primitiva, num_secuencia);
}

void mostrarTrama(char tipo, char direccion, char *primitiva, char num_secuencia, unsigned char BCE) {
    printf("%c   %c   %s   %c   %i\n", tipo, direccion, primitiva, num_secuencia, BCE);
}

void mostrarTrama(char tipo, char direccion, char *primitiva, char num_secuencia, unsigned char BCE, unsigned char BCE_calculado) {
    printf("%c   %c   %s   %c   %i   %i\n", tipo, direccion,  primitiva, num_secuencia, BCE, BCE_calculado);
}

unsigned char * obtenerMACFuente(apacket_t trama){
    const unsigned char * paquete = trama.packet;
    unsigned char * mac_fnt = (unsigned char *)malloc(sizeof(unsigned char)*6);
    memcpy(mac_fnt, &paquete[6], 6);
    return mac_fnt;
}

unsigned char *  obtenerMACDst(apacket_t trama){
    const unsigned char * paquete = trama.packet;
    unsigned char * mac_dst = (unsigned char *)malloc(sizeof(unsigned char)*6);
    memcpy(mac_dst, &paquete[0], 6);
    return mac_dst;
}

unsigned char * obtenerTipo(apacket_t trama){
    const unsigned char * paquete = trama.packet;
    unsigned char * tipo = (unsigned char *)malloc(sizeof(unsigned char)*2);
    memcpy(tipo, &paquete[12], 2);
    return tipo;
}

unsigned char obtenerDireccion(apacket_t trama){
    const unsigned char * paquete = trama.packet;
    return paquete[14];
}

unsigned char obtenerPrimitiva(apacket_t trama){
    const unsigned char * paquete = trama.packet;
    return paquete[15];
}

unsigned char obtenerNumSecuencia(apacket_t trama){
    const unsigned char * paquete = trama.packet;
    return paquete[16];
}

unsigned char obtenerLongitud(apacket_t trama){
    const unsigned char * paquete = trama.packet;
    return paquete[17];
}

char * obtenerDatos(apacket_t trama){
    const unsigned char * paquete = trama.packet;
    char * datos;
    int longitud = obtenerLongitud(trama);
    datos = (char *)malloc(sizeof(char) * (longitud + 1));
    memcpy(datos, &paquete[18], longitud);
    return datos;
}

unsigned char obtenerBCE(apacket_t trama){
    const unsigned char * paquete = trama.packet;
    int longitud = obtenerLongitud(trama);
    return paquete[18+longitud];
}

unsigned char calcularBCE(char * datos, int longitud){
    unsigned char BCE = datos[0];
    for(int i = 1; i < longitud; i++){
        BCE = BCE xor datos[i];
    }
    if(BCE == 255 || BCE == 0) BCE = 1;
    return BCE;
}

bool hayErroresTrama(apacket_t trama){
    if(trama.packet != nullptr) {
        return obtenerBCE(trama) != calcularBCE(obtenerDatos(trama), obtenerLongitud(trama));
    }
    return true;
}

void enviarDato(interface_t * iface, Metadata * metadata, unsigned char * dato){
    unsigned char *trama = BuildFrame(metadata->mac_src, metadata->mac_dst, metadata->type, dato);
    SendFrame(iface, trama, 1); 
    free(trama);
}

void enviarTramaDatosConProtocolo(interface_t * iface, Metadata * metadata, unsigned char direccion, unsigned char numTrama, int longitud, char * datos, unsigned char BCE) {
    unsigned char * paqueteDatos = (unsigned char *)malloc(sizeof(unsigned char ) * (4+longitud+1));
    paqueteDatos[0] = direccion;
    paqueteDatos[1] = 2;
    paqueteDatos[2] = numTrama;
    paqueteDatos[3] = longitud;
    memcpy(&paqueteDatos[4], datos, longitud); 
    paqueteDatos[4+longitud] = BCE; 
    unsigned char *trama = BuildFrame(metadata->mac_src, metadata->mac_dst, metadata->type, paqueteDatos);
    SendFrame(iface, trama, 4+longitud+1);
    mostrarTrama('E', direccion, "STX ", numTrama, BCE);
    free(paqueteDatos);
    free(trama);
}

void enviarTramaDatosSinProtocolo(interface_t * iface, Metadata * metadata, char * datos){
    int longitud = strlen(datos);
    unsigned char * paqueteDatos = (unsigned char *)malloc(sizeof(unsigned char ) * (longitud));
    memcpy(&paqueteDatos[0], datos, longitud); 
    unsigned char *trama = BuildFrame(metadata->mac_src, metadata->mac_dst, metadata->type, paqueteDatos);
    SendFrame(iface, trama, longitud);
    free(paqueteDatos);
    free(trama);
}

void enviarTramaControl(interface_t * iface, Metadata * metadata, unsigned char direccion, unsigned char primitiva, unsigned char numTrama){
    unsigned char paqueteControl[3] = {direccion, primitiva, numTrama}; 
    unsigned char *trama = BuildFrame(metadata->mac_src, metadata->mac_dst, metadata->type, paqueteControl);
    SendFrame(iface, trama, 3); 
    free(trama);
}

void enviarTramaVacia(interface_t * iface, Metadata * meta_data){
    unsigned char * trama;
    trama = BuildHeader(meta_data->mac_src, meta_data->mac_dst, meta_data->type);
    SendFrame(iface, trama, 0);
    free(trama);
}

apacket_t recibirTrama(interface_t * iface){
    return ReceiveFrame(iface);
}

void mostrarDatosTramaSinProtocolo(apacket_t paquete_recibido){
    int longitud = paquete_recibido.header.len-14;
    char * cadena_recibida = (char*)malloc(sizeof(char)*longitud+1);
    memcpy(cadena_recibida, &paquete_recibido.packet[14], longitud);
    cadena_recibida[longitud] = '\0';
    printf("Nº de caracteres recibido: %d. %s.", longitud, cadena_recibida);
}

void mostrarDatosTrama(apacket_t paquete_recibido){
    char * cadena_recibida;
    int longitud = obtenerLongitud(paquete_recibido);
    if(longitud > 0){
        cadena_recibida = (char *)malloc(sizeof(char)*longitud+1);
        memcpy(cadena_recibida, (char*)obtenerDatos(paquete_recibido), longitud);
        cadena_recibida[longitud] = '\0';
        printf("Recibido: %s. ", cadena_recibida);
    }
    free(cadena_recibida);
}

