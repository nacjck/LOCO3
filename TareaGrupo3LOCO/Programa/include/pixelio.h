/*
 * pixelio.h
 *
 * Aquí se encuentran funciones encargadas de la entrada y salida
 * de pixeles
 *
 */
#include <stdint.h>

#ifndef pixelio_h
#define pixelio_h

#include "modalidad.h"

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
/*
 * Variables para buffer de pixeles
 */

 typedef struct Imagen {
   // Parámetros de compresión
   BYTE s;
   Modalidad r;
   // Parámetros de la imagen
   unsigned int ancho;
   unsigned int alto;
   BYTE maxValorPixel;
   // Valores de pixel
   BYTE** datos;
 } imagen;

void inicializarBuffer( int tamanio );

void determinarContexto( unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d );

//Extracto * determinarExtracto( unsigned char x, unsigned char a, unsigned char b, unsigned char c );

void destruirBuffer();

//COMPRESOR
int obtenerUltimoCaracter();

#endif
