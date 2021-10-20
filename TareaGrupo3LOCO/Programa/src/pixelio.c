#include "../include/pixelio.h"

//Estructura de arreglo circular que hice para otra materia
#define MAX_BUFFER 123456 //PROVISORIO, este valor va a ser dinámico

typedef struct {
    int cant;
    int ini,fin;
    unsigned char * valores;
} cola_t;

cola_t crearCola(int size) {
    cola_t res = {0,0,0,malloc(size*sizeof(unsigned char))}; 
    return res;
}

void encolar(cola_t * cola,char elem) {
    if(cola->cant != 0) {
        if(cola->ini != 0) {
            cola->ini--;
        }
        else {
            cola->ini = MAX_BUFFER - 1;
        }
    }
    cola->valores[cola->ini] = elem;
    cola->cant++;
}

void destruirCola(cola_t * cola) {
    free(cola->valores);
}

/*
void desencolar(cola_t * cola) {
        cola->cant--;
        if(cola->cant != 0) {
            if(cola->fin != 0) {
                cola->fin--;
            }
            else {
                cola->fin = MAX_BUFFER - 1;
            }
        }
}*/