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

typedef struct _datosCabezal * DatosCabezal;
typedef struct _imagen * Imagen;

Imagen crearImagen( DatosCabezal dtCabezal );

DatosCabezal escribirCabezalPGM( FILE * archivoDescomprimido, FILE * archivoComprimido );

int obtenerAncho( Imagen img );

int obtenerAltura( Imagen img );

int obtenerMaxValue( Imagen img );

void determinarContexto( Imagen img, unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d );

bool esFinDeLinea( Imagen img );

void destruirImagen( Imagen img );

void destruirDatosCabezal( DatosCabezal );

//COMPRESOR
int obtenerUltimoCaracter( Imagen img, FILE * archivoComprimido );

//DESCOMPRESOR
void agregarCaracter( Imagen img, unsigned char c );
#endif