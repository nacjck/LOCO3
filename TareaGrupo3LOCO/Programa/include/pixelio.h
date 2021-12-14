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

typedef struct _imagen * Imagen;

Imagen crearImagen( int tamanio );

void escribirCabezalPGM( Imagen img, FILE * archivoDescomprimido, FILE * archivoComprimido );

void determinarContexto( Imagen img, unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d );

bool esFinDeLinea( Imagen img );

void destruirImagen( Imagen img );

//COMPRESOR
int obtenerUltimoCaracter( Imagen img, FILE * archivoComprimido );

#endif