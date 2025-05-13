#include "metadata.hpp"

void mostrarDireccionMAC(unsigned char mac[6]){
   for(int i = 0; i < 6; i++){
      printf("%02x", mac[i]);
      if(i < 5) printf(":");
   }

   printf("\n");
}

void cambiarMACOrigen(Metadata *metadata, unsigned char *new_mac_src){
  memcpy(metadata->mac_src, new_mac_src, 6);
}

void cambiarMACDestino(Metadata *metadata, const unsigned char *new_mac_src) {
  memcpy(metadata->mac_dst, new_mac_src, 6);
}

void mostrarMACOrigen(Metadata *metadata){
  mostrarDireccionMAC(metadata->mac_src);
}

void mostrarMACDestino(Metadata *metadata){
  mostrarDireccionMAC(metadata->mac_dst);
}

void mostrarTipo(Metadata *metadata){
  printf("%02x%02x", metadata->type[0], metadata->type[1]);
}