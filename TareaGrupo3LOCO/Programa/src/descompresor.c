#include "../include/descompresor.h"
#include "../include/compartido.h"
#include "../include/modalidad.h"
#include "../include/pixelio.h"

#include <stdio.h>

void leerCabezal(int *s,modalidad_t * modalidad); //FUNCION AUXILIAR DESCOMPRESOR
void inicializarContextos(); //COMPARTIDO
void inicializarBuffer(); //PIXELIO
unsigned char obtenerUltimoCaracter(); //PIXELIO
void determinarContexto(); //PIXELIO
int comienzaModoDeRun(); //PIXELIO
void determinarParametroGolombK(); //COMPARTIDO
void determinarMapeoRice(); //COMPARTIDO
void determinarCodificacionGolomb(); //COMPARTIDO

void descomprimir( char* archivoEntrada, char* archivoSalida ) {
    FILE * compressedFile;
    FILE * decompressedFile;
    int s;
    modalidad_t modalidad; 
    int k;
    unsigned char x;

    leerCabezal(&s,&modalidad); //Lee el cabezal que contiene s y modalidad
    compressedFile = fopen(archivoEntrada,"rb");
    decompressedFile = fopen(archivoSalida,"wb");
    
    inicializarContextos();
    inicializarBuffer();
    if(modalidad == Run) {
        
        while(x = obtenerUltimoCaracter() != EOF) {
            determinarContexto();
            if(!comienzaModoDeRun()) {
                determinarParametroGolombK();
                determinarMapeoRice();
                determinarCodificacionGolomb();
                actualizarContextos();
            }
            else {
                unsigned char xAnterior;
                do {
                    xAnterior = x;
                    k = 3;
                    determinarMapeoRice();
                    determinarCodificacionGolomb();
                    actualizarContextos();
                    x =  obtenerUltimoCaracter();
                } while(x == xAnterior && x != EOF);
                if(x == EOF) break;
            }
        }
    }
    else {
        while(x = obtenerUltimoCaracter() != EOF) {
            determinarContexto();
            determinarParametroGolombK();
            determinarMapeoRice();
            determinarCodificacionGolomb();
            actualizarContextos();
        }
    }
}

/*
 * Función auxiliar encargada de obtener los parámetros 's' y
 * la modalidad (Run o Normal) enviados del lado del compresor
 */
void leerCabezal(int *s,modalidad_t * modalidad) {
    //PROVISORIO
    *s = 0;
    *modalidad = Normal;
}