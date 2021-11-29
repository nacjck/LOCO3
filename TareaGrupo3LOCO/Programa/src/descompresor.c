#include "../include/descompresor.h"

void descomprimir( char* pathArchivoEntrada, char* pathArchivoSalida ) {
  FILE * archivoComprimido;
  FILE * archivoDescomprimido;
  imagen img; // Donde se guardan valores de pixel y parámetros de la imagen
  BYTE a,b,c,d; // Pixeles de contexto
  unsigned int ip; // Indice de pixel

  archivoComprimido = fopen(pathArchivoEntrada, "rb");
  archivoDescomprimido = fopen(pathArchivoSalida, "wb");

  crearImagen(archivoComprimido, img); // Lee parámetros y reserva memoria
  inicializarExtractos();

  // Para cada pixel
  for (int fila=1; i <= img.alto; i++) {
    for (int col=1; j <= img.ancho; j++) {

      ip = i*img.ancho + j;
      contexto(&img, ip, &a,&b,&c,&d);


    }
  }

}
