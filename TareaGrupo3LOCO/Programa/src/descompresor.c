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

BYTE leerSubUn(BYTE buff, BYTE bitInicio) {
  /* Cuenta la cantidad de 0 que preceden al primer 1 desde el bit bitInicio
  del byte (código unario). Si el byte es 0 devuelve 8 */

  int i;
  buff <<= bitInicio;
  for (i=0; i<8-bitInicio; i++) {
    if ((buff << i) & 128) { // MSB == 1
      return i;
    }
  }
  return i;
}

unsigned int extraerParteUnaria(BYTE* buff, BYTE* indBit, FILE* archivoComprimido) {
  /* Extrae la parte unaria del código de Golomb. *buff* es el byte leido
  en el paso anterior y *indBit* es el índice del primer bit aún no procesado.
  Lee más bytes del archivo en caso de ser necesario. */
  BYTE n;
  unsigned int un = 0;

  // Busca primer 1 en el buffer
  un += leerSubUn(*buff, *indBit);
  if ( un == 8-(*indBit) ) { // Si llegó al final
    *indBit = 0;
    do {
      *buff = fgetc(archivoComprimido);
      n = leerSubUn(*buff, *indBit);
      un += n;
    } while(n==8); // Mientras se recorra el byte completo
    *indBit = n==7 ? 0 : n+1; // Se suma 1 por el 1 del final
  }
  else {
    *indBit += un+1;
  }
  return un;
}

BYTE leerSubBin(BYTE b, BYTE bitInicio, BYTE bitFin) {
  /* Devuelve la sub-cadena de binarios entre bitInicio y bitFin del byte b */
  b <<= bitInicio;
  b >>= bitInicio + 8 - bitFin;
  return b;
}

unsigned int extraerParteBinaria(BYTE* buff, int k, BYTE* indBit,
                                                      FILE* archivoComprimido) {
  /* Extrae la parte binaria del código (largo k bits). *buff* es el byte leido
  en el paso anterior y *indBit* es el índice del primer bit aún no procesado.
  Lee más bytes del archivo en caso de ser necesario. */
  unsigned int bin;

  if (*indBit+k <= 8) { // Si hay suficientes bits en el buffer
    bin = leerSubBin((*buff), (*indBit), (*indBit)+k);
    *indBit += k;
  }
  else {
    // Lee los bits que quedan sin procesar
    bin = leerSubBin(*buff, *indBit, 8);
    k -= 8-(*indBit); // Bits restantes por leer
    for (int n=0; n<(k>>3); n++) { // Entra si se necesita algún byte completo
      // Se concatena el byte entero
      bin <<= 8;
      bin += fgetc(archivoComprimido);
      k -= 8;
    }
    // Se lee nuevo byte y se concatenan bits restantes
    *buff = fgetc(archivoComprimido);
    bin <<= k;
    bin += leerSubBin(*buff, 0, k);
    *indBit = k; // Actualización de índice
  }
  return bin;
}

int deshacerMapeo(unsigned int M) {
  return (M%2==0) ? (M>>1) : -( (M-1)>>1 );
}

void descomprimir( char* pathArchivoEntrada, char* pathArchivoSalida ) {
  FILE * archivoComprimido;
  FILE * archivoPGM;
  imagen img; // Donde se guardan valores de pixel y parámetros de la imagen
  BYTE a,b,c,d, x_p;
  BYTE bcodigo, bitInd;
  unsigned int ip, k, nBbin, bin, unCount;
  Extracto * fC;

  // archivoComprimido = fopen(pathArchivoEntrada, "rb");
  // archivoPGM = fopen(pathArchivoSalida, "wb");
  //
  // crearImagen(archivoComprimido, &img); // Lee parámetros y reserva memoria
  // inicializarExtractos(img.s);
  // bitInd = 0; // Inidce del próximo bit a procesar
  // // Para cada pixel
  // for (int fila=1; fila <= img.alto; fila++) {
  //   for (int col=1; col <= img.ancho; col++) {
  //
  //     ip = fila*img.ancho + col;
  //     contexto(&img, ip, &a,&b,&c,&d);
  //     x_p = predecirX(a,b,c);
  //     fC = determinarExtracto(x_p, a,b,c);
  //     k = determinarGolombK(fC); // También es el largo de la parte binaria
  //     nBbin = k/8 + 1; // Número de bytes de la parte binaria
  //
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
  BYTE buff;
  FILE* acomp;
  imagen img;
  BYTE indBit;

  indBit = atoi(argv[2]);

  buff = '@';

  if ((acomp = fopen("test.txt", "rb")) == NULL){
       printf("Error! opening file");

       // Program exits if the file pointer returns NULL.
       exit(1);
   }

  crearImagen(acomp, &img);

  printf("Parte unaria = 0x%x\n", extraerParteUnaria( &buff, &indBit, acomp ));

  printf("Byte en buffer: 0x%x\n", buff);
  printf("Indice de bit: %d\n", indBit);

  fclose(acomp);
  free(img.datos);
}
