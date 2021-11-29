#include "../include/descompresor.h"

void descomprimir( char* pathArchivoEntrada, char* pathArchivoSalida ) {
  FILE * archivoComprimido;
  FILE * archivoDescomprimido;
  imagen img; // Donde se guardan valores de pixel y parámetros de la imagen
  BYTE a,b,c,d, x_p; // Valores de pixel
  unsigned int ip, l, n;
  BYTE* buffer;
  Extracto * fC;

  archivoComprimido = fopen(pathArchivoEntrada, "rb");
  archivoDescomprimido = fopen(pathArchivoSalida, "wb");

  crearImagen(archivoComprimido, img); // Lee parámetros y reserva memoria
  inicializarExtractos();

  // Para cada pixel
  for (int fila=1; i <= img.alto; i++) {
    for (int col=1; j <= img.ancho; j++) {

      ip = i*img.ancho + j;
      contexto(&img, ip, &a,&b,&c,&d);
      x_p = predecirX(a,b,c);
      fC = determinarExtracto(x_p, a,b,c, img.s)
      k = determinarGolombK(fC)
      l = determinarLargoGolomb(k, determinarMapeoRice(x_p, fC)); // Largo del código

      // Lee los bytes que contienen el código del archivo
      n = l/8 + 1;
      buffer = leerNBytes(archivoComprimido, n);
    }
  }

}
