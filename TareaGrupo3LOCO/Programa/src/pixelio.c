#include "../include/pixelio.h"

#include <string.h>

/*
 * Variables para imagen sin comprimir
 */
static unsigned char * bufferImagen[2];             /* Ultimas dos filas de imagen             */
static size_t anchoImagen;
static int posicionActualImagen;                    /* Si se esta en el borde izquierdo vale 0 */
static int filaSuperior;                            /* 0 o 1                                   */

void inicializarBuffer( int ancho ) {
    filaSuperior = 0;
    posicionActualImagen = ancho + 1;
    anchoImagen = ancho;
    bufferImagen[0] = malloc(anchoImagen * sizeof(unsigned char) + 4);
    bufferImagen[1] = malloc(anchoImagen * sizeof(unsigned char) + 4);
    memset(bufferImagen[!filaSuperior], 0, ancho + 4);
    memset(bufferImagen[ filaSuperior], 0, ancho + 4);
}

void determinarContexto( unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d ) {
    /* Si se está fuera de los límites de la imagen se devuelve 0.  */
    /* Se asume que esto siempre sucede debido a la inicialización  */
    /* del buffer.                                                  */

    *a = bufferImagen[!filaSuperior][posicionActualImagen - 1];
    *b = bufferImagen[filaSuperior][posicionActualImagen];
    *c = bufferImagen[filaSuperior][posicionActualImagen - 1];
    *d = bufferImagen[filaSuperior][posicionActualImagen + 1];
}

void destruirBuffer() {
    free(bufferImagen[0]);
    free(bufferImagen[1]);
}

/* SOLO COMPRESOR */
int obtenerUltimoCaracter( FILE * archivoOriginal ) {
    int ultimoCaracter = EOF;
    posicionActualImagen++;
    if (posicionActualImagen <= anchoImagen) {
        ultimoCaracter = bufferImagen[!filaSuperior][posicionActualImagen];
    }
    else {
        if (fgets(bufferImagen[filaSuperior] + 1, anchoImagen + 1, archivoOriginal)) {
            /* Se reinician los punteros */
            posicionActualImagen = 1;
            ultimoCaracter = bufferImagen[filaSuperior][posicionActualImagen];
            
            /* Off-Bounds */
            bufferImagen[filaSuperior][anchoImagen + 1] = 0; /* Off-bounds */

            /* filaSuperior = filaSuperior mod 2 */    
            filaSuperior = !filaSuperior;
        }
    }
    return ultimoCaracter;
}

/*
int main() {
    int a,b,c,d;
    int ch = 0;
    FILE * f = fopen("test.txt","rb");

    inicializarBuffer(4);
    while((ch = obtenerUltimoCaracter(f)) != EOF) {
        determinarContexto(&a,&b,&c,&d);
        printf("%d  %d  %d \n%d  %d\n\n",c,b,d,a,ch);
    }

    void destruirBuffer();
}*/
