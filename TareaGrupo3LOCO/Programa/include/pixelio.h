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

/*
 * Variables para buffer de pixeles
 */

void inicializarBufferImagen( int tamanio );

void determinarContexto(int * a, int * b, int * c, int * d );

//Extracto * determinarExtracto( unsigned char x, unsigned char a, unsigned char b, unsigned char c );

void destruirBuffer();

//COMPRESOR
int obtenerUltimoCaracter();

#endif