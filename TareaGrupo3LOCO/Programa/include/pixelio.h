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

#define bool int

/*
 * Variables para buffer de pixeles
 */

void inicializarBuffer( int tamanio );

void escribirCabezalPGM( FILE * archivoDescomprimido, FILE * archivoComprimido, int * ancho );

void determinarContexto( unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d );

bool esFinDeLinea();

void destruirBuffer();

//COMPRESOR
int obtenerUltimoCaracter();

#endif