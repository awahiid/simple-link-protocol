#include "primitivas.hpp"

char * formatearPrimitiva(unsigned char primitiva){ 
    switch (primitiva)
    {
    case 2:
        return "STX ";
        break;
    case 4:
        return "EOT ";
        break;
    case 5:
        return "ENQ ";
        break;
    case 6:
        return "ACK ";
        break;
    case 21:
        return "NACK";
        break;
    default:
        break;
    }
}

void mostrarTramaRecibida(apacket_t trama_recibida){
    mostrarTrama('R', obtenerDireccion(trama_recibida), formatearPrimitiva(obtenerPrimitiva(trama_recibida)), obtenerNumSecuencia(trama_recibida));
}

bool esTramaCorrecta(Metadata * metadata, apacket_t trama_recibida, unsigned char primitiva, char dir, char num_secuencia){
    if(trama_recibida.packet != nullptr){
        bool esMiTrama = (memcmp(metadata->mac_src, obtenerMACDst(trama_recibida), 6) == 0);
        bool esDir = dir == obtenerDireccion(trama_recibida);
        bool esConexion = primitiva == obtenerPrimitiva(trama_recibida);
        bool secuenciaCorrecta = num_secuencia == obtenerNumSecuencia(trama_recibida);
        return esMiTrama && esDir && esConexion && secuenciaCorrecta;
    }
    return false;
}

bool comprobarYMostrarTrama(Metadata * metadata, apacket_t trama_recibida, char primitiva, char dir, char num_secuencia){
    if(esTramaCorrecta(metadata, trama_recibida, primitiva, dir, num_secuencia)){
        mostrarTramaRecibida(trama_recibida);
        return true;
    }
    return false;
}


void enviarPeticionConexion(interface_t * iface, Metadata * metadata, char dir){
    enviarTramaControl(iface, metadata, dir, ENQ, '0');
    mostrarTrama('E', dir, "ENQ ", '0');
}

void enviarPeticionCierre(interface_t * iface, Metadata * metadata, char dir,char num_sec){
    enviarTramaControl(iface, metadata, dir, EOT, num_sec);
    mostrarTrama('E', dir, "EOT ", num_sec);
}

void enviarConfirmacion(interface_t * iface, Metadata * metadata, apacket_t trama_recibida){
    enviarTramaControl(iface, metadata, obtenerDireccion(trama_recibida), ACK, obtenerNumSecuencia(trama_recibida));
    mostrarTrama('E', obtenerDireccion(trama_recibida), "ACK ", obtenerNumSecuencia(trama_recibida));
}

void enviarRetransmision(interface_t * iface, Metadata * metadata, apacket_t trama_recibida){
    enviarTramaControl(iface, metadata, obtenerDireccion(trama_recibida), NACK, obtenerNumSecuencia(trama_recibida));
    mostrarTrama('E', obtenerDireccion(trama_recibida), "NACK", obtenerNumSecuencia(trama_recibida));
}

bool seHaRecibidoConexion(Metadata * metadata, apacket_t trama_recibida, char dir){
    return comprobarYMostrarTrama(metadata, trama_recibida, ENQ, dir, '0');
}

bool seHaRecibidoCierre(Metadata * metadata, apacket_t trama_recibida, char dir,char num_sec){
    return comprobarYMostrarTrama(metadata, trama_recibida, EOT, dir, num_sec);
}

bool seHaRecibidoConfirmacion(Metadata * metadata, apacket_t trama_recibida, char dir, char num_secuencia){
    return comprobarYMostrarTrama(metadata, trama_recibida, ACK, dir, num_secuencia);
}

bool seHaRecibidoRetransmision(Metadata * metadata, apacket_t trama_recibida, char dir, char num_secuencia){
    return comprobarYMostrarTrama(metadata, trama_recibida, NACK, dir, num_secuencia);
}

bool seHaRecibidoTramaDatos(Metadata *metadata, apacket_t trama_recibida, char dir, char num_secuencia){
    if(trama_recibida.packet != nullptr){
        return esTramaCorrecta(metadata, trama_recibida, STX, dir, num_secuencia);
    }
    return false;
}