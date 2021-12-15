/*
 * descompresor.h
 *
 * Brinda las herramientas necesarias para descomprimir
 * un archivo dados los parámetros necesarios
 *
 */

#ifndef descompresor_h
#define descompresor_h

#include "../include/compartido.h"
#include "../include/pixelio.h"

#include <string.h>

typedef unsigned char BYTE;

void descomprimir( char* archivoEntrada, char* archivoSalida );


#endif
