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
#include "modalidad.h"

#include <stdio.h>
#include <stdlib.h>

<<<<<<< HEAD
#define bool int

=======
typedef unsigned char BYTE;
>>>>>>> d06982d (Primer commit. Función lectora de encabezado probada.)
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

void escribirCabezalPGM( FILE * archivoDescomprimido, FILE * archivoComprimido, int * ancho );

void determinarContexto( unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d );

bool esFinDeLinea();

void destruirBuffer();

//COMPRESOR
int obtenerUltimoCaracter( FILE * archivoComprimido );
=======
// Definiciones de tipos

typedef unsigned char PIX; // Alias para valores de pixel
typedef unsigned char BYTE; // Byte sin signo
>>>>>>> 185eb18 (Se cambiarona lagunos tipos y se terminó archivo de encabezados.)

#endif
