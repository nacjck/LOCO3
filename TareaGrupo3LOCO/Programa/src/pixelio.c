#include "../include/pixelio.h"

/*
 * Variables para imagen sin comprimir
 */
unsigned char * bufferImagen[2];             /* Ultimas dos filas de imagen             */
size_t anchoImagen;                  
int posicionActualImagen;                    /* Si se esta en el borde izquierdo vale 0 */
int filaSuperior;                            /* 0 o 1                                   */

void inicializarBuffer( int ancho ) {
    filaSuperior = 0;
    posicionActualImagen = ancho + 1;
    anchoImagen = ancho;
    bufferImagen[0] = malloc(anchoImagen * sizeof(unsigned char) + 2);
    bufferImagen[1] = malloc(anchoImagen * sizeof(unsigned char) + 2);

    /* Para contextos iniciales */
    int i;
    for(i = 0; i <= anchoImagen + 1; i++) {
        bufferImagen[!filaSuperior][i] = 0;
    }
    bufferImagen[filaSuperior][0] = 0;
    bufferImagen[filaSuperior][anchoImagen + 1] = 0;
}

void determinarContexto( int * a, int * b, int * c, int * d ) {
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