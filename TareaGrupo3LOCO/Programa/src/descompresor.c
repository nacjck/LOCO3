#include "../include/descompresor.h"
#include "../include/compartido.h"
#include "../include/pixelio.h"

#include <string.h>

BYTE* leerNBytes(FILE* archivoComprimido, int N) {
  /* Lee N bytes del archivo comprimido. Devuelve un puntero a un arreglo. */

  BYTE* bytesLeidos;

  bytesLeidos = malloc(N * sizeof(BYTE));

  fread(bytesLeidos, 1, N, archivoComprimido);
  return bytesLeidos;

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

unsigned int decodificarParteUnaria(BYTE* buff, BYTE* indBit, FILE* archivoComprimido) {
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
    if (n==7) {*buff = fgetc(archivoComprimido); *indBit=0;}
    else {*indBit=n+1;};
    // *indBit = n==7 ? 0 : n+1; // Se suma 1 por el 1 del final
  }
  else {
    if ( *indBit+un==7 ) { // 1 de fin de parte unaria en el último bit
      *buff = fgetc(archivoComprimido);
      *indBit = 0;
    }
    else {
      *indBit += un+1;
    }
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
  return (M&1) ? -( (M-1)>>1 ) : (M>>1);
}

int decodificarGPO2(BYTE* buff, int k, BYTE* indBit, FILE* archivoComprimido) {
  /* Decodifica el código de Golomb y devuelve el error de predicción o
  la cantidad de repeticiones en modo run. */
  unsigned int bin, unCount;
  bin = extraerParteBinaria(buff, k, indBit, archivoComprimido);
  unCount = decodificarParteUnaria(buff, indBit, archivoComprimido);
  return ( (unCount<<k) + bin );
}

void descomprimirRun(int n, int s, Imagen img, Extractos extractos, FILE * archivoPGM) {
  unsigned char x_p;
  unsigned char a,b,c,d;
  int fC;
  Extracto fExtracto;
  int k;

  determinarContexto(img, &a,&b,&c,&d);
  x_p = predecirX(a,b,c);
  fC = determinarIndiceExtracto(x_p, a,b,c, s);
  fExtracto = determinarExtracto(extractos, fC);

  for (int m=0; m<n; m++) {
    fwrite(&a, 1, 1, archivoPGM);
    agregarCaracter(img, a);
    avanzarPixel(img);
  }
}

void descomprimirNormal(BYTE * buff, BYTE * indBit, int s, Imagen img, Extractos extractos, FILE * archivoPGM, FILE * archivoComprimido) {
  unsigned char x_p, x_r;
  unsigned char a,b,c,d;
  int fC;
  Extracto fExtracto;
  int k;
  int e;

  determinarContexto(img, &a,&b,&c,&d);
  x_p = predecirX(a,b,c);
  fC = determinarIndiceExtracto(x_p, a,b,c, s);
  fExtracto = determinarExtracto(extractos, fC);
  k = determinarGolombK(fExtracto); // También es el largo de la parte binaria
  e = deshacerMapeo(decodificarGPO2(buff, k, indBit, archivoComprimido));
  // Pixel recuperado
  x_r = e + x_p;
  agregarCaracter(img, x_r);
  avanzarPixel(img);
  fwrite(&x_r, 1, 1, archivoPGM);
  // Actualización de estadísticas
  actualizarExtracto(fExtracto, e);
}

// Macro para evaluar las condiciones de entrada al modo run.
#define RUN ( run && (a==b && b==c && c==d) )

void descomprimir( char* pathArchivoEntrada, char* pathArchivoSalida ) {
  /* */

  FILE * archivoComprimido;
  FILE * archivoPGM;
  Imagen img;
  BYTE a,b,c,d, x_p, x_r ;
  BYTE buff, indBit;
  unsigned int k;
  Extracto fExtracto;
  Extractos extractos;
  int fC;
  int e, n;
  bool run;
  int s;
  int ancho, altura, maxValue;
  DatosCabezal dtCabezal;

  archivoComprimido = fopen(pathArchivoEntrada, "rb");
  archivoPGM = fopen(pathArchivoSalida, "wb");

  leerParametrosCabezal(archivoComprimido, &s,&run);
  dtCabezal = escribirCabezalPGM(archivoComprimido, archivoPGM);
  img = crearImagen(dtCabezal);
  extractos = crearExtractos(s);

  ancho = obtenerAncho(img);
  altura = obtenerAltura(img);
  maxValue = obtenerMaxValue(img);
  indBit = 0; // Inidce del próximo bit a procesar
  buff = fgetc(archivoComprimido); // Primer byte del archivo

  // Para cada pixel
  for (int fila=1; fila <= altura; fila++) {
    for (int col=1; col <= ancho; col++) {

      determinarContexto(img, &a,&b,&c,&d);
      // Nro de repeticiones no nulo
      if ( RUN && (n = decodificarGPO2(&buff, 3, &indBit, archivoComprimido)) ) {
        descomprimirRun(n, s, img, extractos, archivoPGM);
        descomprimirNormal(&buff, &indBit, s, img, extractos, archivoPGM,archivoComprimido);

        // Actualizar indice de columna
        col += n-1;
      }
      else {
        descomprimirNormal(&buff, &indBit, s, img, extractos, archivoPGM,archivoComprimido);
      }
    }
  }

  destruirExtractos(extractos);
  destruirImagen(img);
  destruirDatosCabezal(dtCabezal);
  fclose(archivoComprimido);
  fclose(archivoPGM);
}