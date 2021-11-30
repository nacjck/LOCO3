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

<<<<<<< HEAD
=======
#include "modalidad.h"

#include <stdio.h>
#include <stdlib.h>

>>>>>>> baa1db37007e2a985a1392796c629e08f2e18f46
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
