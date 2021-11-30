/*
 * pixelio.h
 *
 * Aquí se encuentran funciones encargadas de la entrada y salida
 * de pixeles
 *
 */


#ifndef pixelio_h
#define pixelio_h

#include "modalidad.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned char BYTE;

// Estructura de datos para el descompresor
 typedef struct Imagen {
   // Parámetros de compresión
   BYTE s;
   Modalidad r;
   // Parámetros de la imagen
   unsigned int ancho;
   unsigned int alto;
   BYTE maxValorPixel;
   // Datos descomprimidos
   BYTE* datos;
 } imagen;

void inicializarBuffer( int tamanio );

void escribirCabezalPGM( FILE * archivoDescomprimido, FILE * archivoComprimido, int * ancho );

void determinarContexto( unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d );

bool esFinDeLinea();

void destruirBuffer();

//COMPRESOR
int obtenerUltimoCaracter( FILE * archivoComprimido );


#endif
