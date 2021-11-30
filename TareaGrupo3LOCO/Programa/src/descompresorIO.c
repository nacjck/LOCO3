
#include "../include/descompresorIO.h"

void crearImagen(FILE* archivoComprimido, imagen* img) {
  /* Lee la cabecera de un archivo comprimido y guarda los parámetros en un
  tipo imagen. Reserva memoria para los datos, que se inicializan en 0. Se
  agrega una columna a la izquierda y una fila arriba para facilitar el cálculo
  de contextos en los bordes. */

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
  fscanf(archivoComprimido, "%u %u", &(img->ancho), &(img->alto)); // dimensiones de la imagen
  fscanf(archivoComprimido, "%hhu\n", &(img->maxValorPixel)); // Valor máximo

  // Reserva memoria para los datos + padding
  // La imagen es cero si uno de los indices (vertial u horizontal) es cero
  img->datos = calloc( (img->ancho+1)*(img->alto+1), sizeof(BYTE) );

}

BYTE* leerNBytes(FILE* archivoComprimido, unsigned int N) {
  /* Lee N bytes del archivo comprimido. Devuelve un puntero a un arreglo. */

  BYTE* bytesLeidos;

  bytesLeidos = malloc(N * sizeof(BYTE));

  fread(bytesLeidos, 1, N, archivoComprimido);
  return bytesLeidos;

}

void contexto(imagen* img, int ind, BYTE* a, BYTE* b, BYTE* c, BYTE* d) {
  /* Determina los valores de los pixeles del contexto del pixel de la Imagen
  cuyo índice es *ind*. Se asume que la imagen tiene padding (no se chequean
  condiciones de borde). */

  *a = img->datos[ind - 1];
  *b = img->datos[ind - img->ancho - 1];
  *c = img->datos[ind - img->ancho - 2];
  *d = img->datos[ind - img->ancho];
}

/*----------------------------------------------------------------------------*/

void main( int argc, char* argv[] ) {

  FILE * archivoComp;
  imagen img;
  BYTE* lect;

  archivoComp = fopen(argv[1], "rb");
  crearImagen(archivoComp, &img);

  printf("%hhu %d\n", img.s, img.r);
  printf("%u %u\n", img.ancho, img.alto);
  printf("%hhu\n", img.maxValorPixel);
  printf("------------------------\n");

  lect = leerNBytes(archivoComp, 4);
  for (int i = 0; i < 4; i++) {
    printf("%d\n", *(lect+i));
  }

  fclose(archivoComp);
  free(img.datos);
}