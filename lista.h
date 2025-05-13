#include<stdio.h>
#include<stdlib.h>
#include <pcap.h> 

#ifndef LISTA_HPP
#define LISTA_HPP

struct pkthdr{
    int caplen;
    int len;
    struct timeval ts;
};


typedef struct assembled_Packet {
    struct pkthdr header;
    const unsigned char *packet;
}apacket_t;

/*typedef struct datos{
        unsigned char *paquete;
        int frameSize;
}datos;*/

typedef struct nodo {
    apacket_t info;
    struct nodo *sig;
}nodo;

void insertar( struct pcap_pkthdr *h,unsigned char* x);
int vacia();
apacket_t extraer();
void imprimir();
void liberar();

#endif