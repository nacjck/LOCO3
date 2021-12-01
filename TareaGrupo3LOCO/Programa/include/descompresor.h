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

// Estructura de datos para el descompresor
 typedef struct Imagen {
   // Parámetros de compresión
   BYTE s;
   Modalidad r;
   // Parámetros de la imagen
   unsigned int ancho;
   unsigned int alto;
   BYTE maxValorPixel;
   // Datos descomprimidos
   BYTE* datos;
 } imagen;

void crearImagen(FILE* archivoComprimido, imagen* img);
BYTE* leerNBytes(FILE* archivoComprimido, unsigned int N);
void contexto(imagen* img, int ind, BYTE* a, BYTE* b, BYTE* c, BYTE* d);
unsigned int leerUnario(BYTE b, BYTE bitComienzo);
void descomprimir( char* archivoEntrada, char* archivoSalida );


#endif
