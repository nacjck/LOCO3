#include "../include/pixelio.h"
#include "../include/compresorIO.h"

#include <string.h>
#include <stdio.h>

void leerCabezalComprimido(FILE* archivoComprimido, imagen* img) {
  /* Lee la cabecera de un archivo comprimido y guarda los parámetros en un
  tipo imagen */

  char tipo[3]; // Para verificar el tipo de imagen pgm

  // Parámetros de compresión
  fscanf(archivoComprimido, "%hhu", &(img->s)); // s
  fscanf(archivoComprimido, "%d", &(img->r)); // Modo

  // Parámetros pgm
  fscanf(archivoComprimido, "%s", tipo); // magic-number
  if (strcmp(tipo, "P5")) {
    printf("Tipo de imagen no soportado.");
    exit(1);
  }
  fscanf(archivoComprimido, "%u %u", &(img->ancho), &(img->alto)); // dimensiones
  fscanf(archivoComprimido, "%hhu", &(img->maxValorPixel)); // Valor máximo

}

/*----------------------------------------------------------------------------*/

void main( int argc, char* argv[] ) {

  FILE * archivoComp;
  imagen img;

  archivoComp = fopen(argv[1], "rb");
  leerCabezalComprimido(archivoComp, &img);

  printf("%hhu %d\n", img.s, img.r);
  printf("%u %u\n", img.ancho, img.alto);
  printf("%hhu", img.maxValorPixel);
}
