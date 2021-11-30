#include "../include/descompresor.h"
#include "../include/compartido.h"

#define BUFFER_SIZE 10

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

unsigned int leerUnario(BYTE b, BYTE bitInicio) {
  /* Cuenta la cantidad de 0 que preceden al primer 1 desde el bit bitComienzo
  del byte (código unario). Si el byte es 0 devuelve 8 */

  int i;
  b <<= bitInicio;
  for (i=0; i<8-bitInicio; i++) {
    if ((b << i) & 128) { // MSB == 1
      return i;
    }
  }
  return i;
}

unsigned int leerCadenaBinario(BYTE b, BYTE bitInicio, BYTE bitFin) {
  /* Devuelve la sub-cadena de binarios entre bitInicio y bitFin del byte b */
  return ( (b << bitInicio) >> (bitInicio + 8 - bitFin) );
}

void descomprimir( char* pathArchivoEntrada, char* pathArchivoSalida ) {
  FILE * archivoComprimido;
  FILE * archivoPGM;
  imagen img; // Donde se guardan valores de pixel y parámetros de la imagen
  BYTE a,b,c,d, x_p;
  BYTE* bcodigo;
  unsigned int ip, k, lu, bin, un;
  Extracto * fC;

  // archivoComprimido = fopen(pathArchivoEntrada, "rb");
  // archivoPGM = fopen(pathArchivoSalida, "wb");
  //
  // crearImagen(archivoComprimido, &img); // Lee parámetros y reserva memoria
  // inicializarExtractos(img.s);
  //
  // // Para cada pixel
  // for (int fila=1; fila <= img.alto; fila++) {
  //   for (int col=1; col <= img.ancho; col++) {
  //
  //     ip = fila*img.ancho + col;
  //     contexto(&img, ip, &a,&b,&c,&d);
  //     x_p = predecirX(a,b,c);
  //     fC = determinarExtracto(x_p, a,b,c);
  //     k = determinarGolombK(fC); // También es el largo de la parte binaria
  //
  //     // Se leen bytes del archivo
  //     bcodigo = leerNBytes(archivoComprimido, BUFFER_SIZE);
  //     // Los k bits del comienzo son la parte binaria
  //
  //   }
  // }
  //
  // free(img.datos);
  // fclose(archivoComprimido);
  // fclose(archivoPGM);

}

/*============================================================================*/
void main( int argc, char* argv[] ) {
  printf("%hhu", leerCadenaBinario( atoi(argv[1]), atoi(argv[2]), atoi(argv[3]) ));
}
