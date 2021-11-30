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

void crearImagen(FILE* archivoComprimido, imagen* img);
BYTE* leerNBytes(FILE* archivoComprimido, unsigned int N);
void contexto(imagen* img, int ind, BYTE* a, BYTE* b, BYTE* c, BYTE* d);
unsigned int leerUnario(BYTE b, BYTE bitComienzo);
void descomprimir( char* archivoEntrada, char* archivoSalida );


#endif
