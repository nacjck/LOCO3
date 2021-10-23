#include "../include/arregloCircular.h"

ArregloCircular * crearArregloCircular( int tamanio ) {
    int i;
    ArregloCircular * res;

    res = malloc(sizeof(ArregloCircular));
    res->maxIndice = tamanio - 1;
    res->ini = res->fin = 0;
    res->valores = malloc(tamanio*sizeof(unsigned char));
    for(i = 0; i < tamanio; i++) {
        res->valores[i] = 0;    /* arreglo inicialmente lleno con 0's */
    }
    return res;
}

void agregarElemento( ArregloCircular * ac, unsigned char elem ) {
    if (ac->ini != 0) {
        ac->ini--;
    }
    else {
        ac->ini = ac->maxIndice;
    }
    ac->valores[ac->ini] = elem;
}

unsigned char obtenerElemento( ArregloCircular * ac, int indice ) {
    int indiceElemento;
    int elemento;

    indiceElemento = ac->ini + indice;
    if (indiceElemento > ac->maxIndice) {
        indiceElemento -= ac->maxIndice + 1;
    }
    elemento = ac->valores[indiceElemento];
    return elemento;
}

/*
unsigned char obtenerUltimoElemento( Cola * cola ) {
    return cola->valores[cola->fin];
}
*/

/*
unsigned char desencolar(Cola * cola) {
    cola->cant--;
    if(cola->cant != 0) {
        if(cola->fin != 0) {
            cola->fin--;
        }
        else {
            cola->fin = cola->maxIndice;
        }
    }
}
*/

void destruirArregloCircular( ArregloCircular * ac ) {
    free(ac->valores);
    free(ac);
}

