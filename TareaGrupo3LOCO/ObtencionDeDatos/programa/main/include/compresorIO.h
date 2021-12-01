#ifndef compresorIO_h
#define compresorIO_h

#include <stdio.h>

void escribirParametrosCabezal( FILE * archivoComprimido, int s, Modalidad modalidad );

void inicializarBufferCompresion();

void imprimirCompresion( int golombBinario, int largoGolombBinario, int largoGolombUnario, FILE * archivoComprimido);

void vaciarBuffer( FILE * archivoComprimido );

#endif