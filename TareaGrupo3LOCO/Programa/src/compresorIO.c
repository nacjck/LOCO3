#include "../include/pixelio.h"
#include "../include/compresorIO.h"

#define MAX_BUFFER 64

/*
 * Constantes para manejo de output segun tipos de datos 
 */
static const int SIZE_BLOQUE_CODIFICADO = sizeof(unsigned int);
static const int MAX_INDICE_BLOQUE = (int)((sizeof(int) << 3) - 1);

/*
 * Variables para codificacion 
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

void inicializarBufferCompresion( int ancho ) {
    bufferCodificado[0] = 0;
    actualBloqueCodificado = bufferCodificado;
    indiceBloqueCodificado = MAX_INDICE_BLOQUE;
}

int obtenerUltimoCaracter( FILE * archivoDescomprimido ) {
    int ultimoCaracter = EOF;
    posicionActualImagen++;
    if (posicionActualImagen <= anchoImagen) {
        ultimoCaracter = bufferImagen[!filaSuperior][posicionActualImagen];
    }
    else {
        if (fgets(bufferImagen[filaSuperior] + 1, anchoImagen + 3, archivoDescomprimido)) {
            posicionActualImagen = 1;
            ultimoCaracter = bufferImagen[filaSuperior][posicionActualImagen];
            filaSuperior = !filaSuperior;  /* filaSuperior = filaSuperior mod 2 */
            bufferImagen[filaSuperior][anchoImagen + 1] = 0; /* Off-bounds */
        }
    }
    return ultimoCaracter;
}

//FALTA TESTEAR
void actualizarBuffer( int output, int cantidadBits, FILE * archivoComprimido ) {
    /* Output carga los bits del mas al menos significativo */
    /* (Little-Endian)                                      */
    int i,j;

    int indiceFinalOutput = MAX_INDICE_BLOQUE - cantidadBits;
    for (i = MAX_INDICE_BLOQUE; i >= indiceFinalOutput; i--) {
        if (indiceBloqueCodificado != -1) {
            int bitOutput = (i > indiceBloqueCodificado) ?
                             output >> (i - indiceBloqueCodificado):
                             output << (i - indiceBloqueCodificado);
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

//FALTA TESTEAR
void vaciarBuffer( FILE * archivoComprimido ) {
    fwrite(bufferCodificado, SIZE_BLOQUE_CODIFICADO,
          (size_t)(actualBloqueCodificado - bufferCodificado) + 1, archivoComprimido);
}