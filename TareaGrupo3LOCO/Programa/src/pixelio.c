#include "../include/modalidad.h"
#include "../include/pixelio.h"
#include "../include/compartido.h"

#include <stdio.h>

#define MAX_BUFFER 64

/*
 * Constantes para manejo de output segun tipos de datos 
 */
static const int SIZE_BLOQUE_CODIFICADO =
                 sizeof(unsigned int);
static const int MAX_INDICE_BLOQUE =
                 (int)((sizeof(int) << 3) - 1);

/*
 * Variables para imagen sin comprimir
 */
static unsigned char * bufferImagen[2];             /* Ultimas dos filas de imagen             */
static size_t anchoImagen;                  
static int posicionActualImagen;                    /* Si se esta en el borde izquierdo vale 0 */
static int filaSuperior;                            /* 0 o 1                                   */

/*
 * Variables para codificacion 
 * (SEGURAMENTE VA A IR EN OTRO ARCHIVO)
 */
static unsigned int bufferCodificado[MAX_BUFFER];   /* Contiene ultimos caracteres a imprimir  */
static unsigned int * actualBloqueCodificado;       /* Puntero a siguiente bloque a imprimir   */
static int indiceBloqueCodificado;                  /* posicion binaria                        */

void escribirCabezal( FILE * archivoComprimido, int s, Modalidad modalidad ) {
    //Por verse
}

int determinarAnchoImagen( FILE * archivoDescomprimido ) {
    //Recorrer cabezal
    return 0;
}

void inicializarBuffer( int ancho ) {
    bufferCodificado[0] = 0;
    actualBloqueCodificado = bufferCodificado;
    indiceBloqueCodificado = MAX_INDICE_BLOQUE;
    filaSuperior = 0;
    posicionActualImagen = -1;
    anchoImagen = ancho;
    bufferImagen[0] = malloc(anchoImagen * sizeof(unsigned char));
    bufferImagen[1] = malloc(anchoImagen * sizeof(unsigned char));
    int i;
    for(i = 0; i < anchoImagen; i++) {  /* Para contextos iniciales */
        bufferImagen[filaSuperior] = 0;
    }
}

char obtenerUltimoCaracter( FILE * archivoDescomprimido ) {
    char ultimoCaracter;
    posicionActualImagen++;
    if (posicionActualImagen != anchoImagen) {
        ultimoCaracter = bufferImagen[!filaSuperior][posicionActualImagen];
    }
    else {
        posicionActualImagen = 0;
        fgets(bufferImagen[filaSuperior], anchoImagen + 1, archivoDescomprimido);
        ultimoCaracter = bufferImagen[filaSuperior][posicionActualImagen];
        filaSuperior = !filaSuperior;  /* filaSuperior = filaSuperior mod 2 */
    }
    return ultimoCaracter;
}

void determinarContexto( unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d ) {
    /* Si se está fuera de los límites de la imagen se devuelve 0,  */
    /* si no se obtiene el pixel del buffer,                        */
    /* Se asume que esto solo sucede en los laterales de x (a,d)    */
    /* debido al diseño del buffer.                                 */

    *a = (posicionActualImagen != 0) ? 
         bufferImagen[!filaSuperior][posicionActualImagen - 1] : 0;
    *b = bufferImagen[filaSuperior][posicionActualImagen - 1];
    *c = (posicionActualImagen != 0) ? 
         bufferImagen[filaSuperior][posicionActualImagen - 1] : 0;
    *d = (posicionActualImagen != anchoImagen - 1) ? 
         bufferImagen[filaSuperior][posicionActualImagen + 1] : 0;
}

void actualizarBuffer( int output, int cantidadBits, FILE * archivoComprimido ) {
    /* Output carga los bits del mas al menos significativo */
    /* (Little-Endian)                                      */
    int i,j;

    int indiceFinalOutput = MAX_INDICE_BLOQUE - cantidadBits;
    for (i = MAX_INDICE_BLOQUE; i >= indiceFinalOutput; i--) {
        if (indiceBloqueCodificado != -1) {
            int bitOutput = (i > indiceBloqueCodificado) ?
                             output >> (i - indiceBloqueCodificado):
                             output << (indiceBloqueCodificado - i);
            int mascara = 1 << indiceBloqueCodificado;
            *actualBloqueCodificado |= mascara & bitOutput;
            indiceBloqueCodificado--;
        }
        else {
            *actualBloqueCodificado = 0;
            actualBloqueCodificado++;
            indiceBloqueCodificado = MAX_INDICE_BLOQUE;
            if (actualBloqueCodificado == bufferCodificado + MAX_BUFFER) {
                fwrite(bufferCodificado, SIZE_BLOQUE_CODIFICADO,
                       MAX_BUFFER, archivoComprimido);
                actualBloqueCodificado = bufferCodificado;
            }
        }
    }
}

void vaciarBuffer( FILE * archivoComprimido ) {
    fwrite(bufferCodificado, SIZE_BLOQUE_CODIFICADO,
          (size_t)(actualBloqueCodificado - bufferCodificado) + 1, archivoComprimido);
}

void destruirBuffer() {
    free(bufferImagen[0]);
    free(bufferImagen[1]);
}