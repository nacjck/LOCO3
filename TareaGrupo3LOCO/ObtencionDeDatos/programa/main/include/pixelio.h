/*
 * pixelio.h
 *
 * Aquí se encuentran funciones encargadas de la entrada y salida
 * de pixeles
 *
 */

#ifndef pixelio_h
#define pixelio_h

#include <stdio.h>
#include <stdlib.h>

#ifndef BYTE
  #define BYTE unsigned char
#endif

#ifndef PIX
  #define PIX unsigned char
#endif

#ifndef bool
  #define bool int
#endif

typedef struct _datosCabezal * DatosCabezal;
typedef struct _imagen * Imagen;

Imagen crearImagen( DatosCabezal dtCabezal );

DatosCabezal escribirCabezalPGM( FILE * archivoDescomprimido, FILE * archivoComprimido );

int obtenerAncho( Imagen img );
int obtenerAltura( Imagen img );
int obtenerMaxValue( Imagen img );

void determinarContexto( Imagen img, PIX * a, PIX * b, PIX * c, PIX * d );

bool esFinDeLinea( Imagen img );

void destruirImagen( Imagen img );

void destruirDatosCabezal( DatosCabezal );

void avanzarPixel( Imagen img );

//COMPRESOR
int obtenerUltimoCaracter( Imagen img, FILE * archivoComprimido );

//DESCOMPRESOR
void agregarCaracter( Imagen img, PIX c );

#endif
