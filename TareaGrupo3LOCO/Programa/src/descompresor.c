#include "../include/descompresor.h"
#include "../include/compartido.h"
#include "../include/modalidad.h"
#include "../include/pixelio.h"

#include <stdio.h>

void leerCabezal(FILE * compressedFile, int *s,modalidad_t * modalidad); //FUNCION AUXILIAR DESCOMPRESOR
void inicializarExtractos(); //COMPARTIDO
void inicializarBuffer(); //PIXELIO
unsigned char obtenerUltimoCaracter(); //PIXELIO
void determinarContexto(unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d); //PIXELIO
unsigned char predecirX(unsigned char a, unsigned char b, unsigned char c); //COMPARTIDO
int determinarExtracto(unsigned char x, unsigned char a, unsigned char b, unsigned char c); //PIXELIO
int determinarParametroGolombK(); //COMPARTIDO
void determinarCodificacionGolomb(); //COMPARTIDO

void descomprimir( char* archivoEntrada, char* archivoSalida ) {
    FILE * compressedFile;
    FILE * decompressedFile;
    int s;
    modalidad_t modalidad; 
    int k;
    int ultimoCaracterLeido;    /* Promoción temporal de x a entero */
    unsigned char x;
    unsigned char xPrediccion;
    int fExtracto;              /* f(C) */
    unsigned char a,b,c,d;      /* Contexto */
    int output;                 /* Salida del programa */

    compressedFile = fopen(archivoEntrada,"rb");
    decompressedFile = fopen(archivoSalida,"wb");

    leerCabezal(compressedFile,&s,&modalidad);    /* Obtiene del cabezal s y modalidad */
    inicializarExtractos();
    inicializarBuffer();
    if(modalidad == Run) {
        while(ultimoCaracterLeido = obtenerUltimoCaracter() != EOF) {
            x = (unsigned char)ultimoCaracterLeido;
            determinarContexto(&a,&b,&c,&d);
            xPrediccion = predecirX(a,b,c);
            fExtracto = determinarExtracto(xPrediccion,a,b,c);
            if(a != b || b!=c || c==d) {    /* No es modo de run */
                k = determinarParametroGolombK(fExtracto); //Crear la maldita estructuraaa
                determinarCodificacionGolomb(k,x-xPrediccion,fExtracto); //METER ESTA FUNCION Y LA DE ABAJO FUERA DEL IF, NECESITO FUNCION EXTRA DE MAPEO DE RICE?
                actualizarContextos();
            }
            else {
                unsigned char xAnterior;
                do {
                    xAnterior = x;
                    k = 3;
                    determinarCodificacionGolomb();
                    actualizarContextos();
                    ultimoCaracterLeido =  obtenerUltimoCaracter();
                    x = (unsigned char)ultimoCaracterLeido;
                } while(ultimoCaracterLeido != EOF && x == xAnterior);
                if(x == EOF) break;
            }
        }
    }
    else {
        while(ultimoCaracterLeido = obtenerUltimoCaracter() != EOF) {
            x = (unsigned char)ultimoCaracterLeido;
            fExtracto = determinarContexto(x);
            determinarParametroGolombK();
            determinarCodificacionGolomb();
            actualizarContextos();
        }
    }
}

/*
 * Función auxiliar encargada de obtener los parámetros 's' y
 * la modalidad (Run o Normal) enviados del lado del compresor
 * 
 * Precondición:
 *     -compressedFile ya ha sido inicializado con el archivo
 *     correspondiente.
 */
void leerCabezal(FILE * compressedFile, int *s,modalidad_t * modalidad) {
    //PROVISORIO
    *s = 0;
    *modalidad = Normal;
}