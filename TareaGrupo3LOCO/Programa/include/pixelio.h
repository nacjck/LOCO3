/*
 * pixelio.h
 *
 * Aquí se encuentran funciones encargadas de la entrada y salida
 * de pixeles
 *
 */

#ifndef pixelio_h
#define pixelio_h

void inicializarBuffer( int tamanio );

char obtenerUltimoCaracter();

void determinarContexto( unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d );

Extracto * determinarExtracto( unsigned char x, unsigned char a, unsigned char b, unsigned char c );

void actualizarBuffer( int output, int cantidadBits, FILE * decompressedFile );

#endif