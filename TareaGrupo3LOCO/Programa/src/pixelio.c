<<<<<<< HEAD
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
    bufferImagen[0] = malloc(anchoImagen * sizeof(unsigned char) + 2);
    bufferImagen[1] = malloc(anchoImagen * sizeof(unsigned char) + 2);
    memset(bufferImagen[!filaSuperior], 0, ancho + 2);
    memset(bufferImagen[ filaSuperior], 0, ancho + 2);
}

void escribirCabezalPGM( FILE * archivoInput, FILE * archivoOutput, int * ancho) {
    int c;

    //P5
    do {
        c = getc(archivoInput);
        fprintf(archivoOutput,"%c",c);
    } while(c != '\n');

    //Comentarios
    while ((c = getc(archivoInput)) == '#') {      
        fprintf(archivoOutput,"%c",c);
        do {
            c = getc(archivoInput);
            fprintf(archivoOutput,"%c",c);
        } while(c != '\n');
        c = getc(archivoInput);
    }

    //Se regresa un caracter
    fseek(archivoInput,-1,SEEK_CUR);       
    
    //Ancho
    fscanf(archivoInput,"%d",ancho);      
    fprintf(archivoOutput,"%d",*ancho);

    //Altura
    do {
        c = getc(archivoInput);
        fprintf(archivoOutput,"%c",c);
    } while(c != '\n');

    //MAX_VALUE
    do {
        c = getc(archivoInput);
        fprintf(archivoOutput,"%c",c);
    } while(c != '\n');
}

void determinarContexto( unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d ) {
    /* Si se está fuera de los límites de la imagen se devuelve 0.  */
    /* Se asume que esto siempre sucede debido a la inicialización  */
    /* del buffer.                                                  */

    *a = bufferImagen[!filaSuperior][posicionActualImagen - 1];
    *b = bufferImagen[ filaSuperior][posicionActualImagen    ];
    *c = bufferImagen[ filaSuperior][posicionActualImagen - 1];
    *d = bufferImagen[ filaSuperior][posicionActualImagen + 1];
}

void destruirBuffer() {
    free(bufferImagen[0]);
    free(bufferImagen[1]);
}

/* SOLO COMPRESOR */
int obtenerUltimoCaracter( FILE * archivoOriginal ) {
    int ultimoCaracter = EOF;

    posicionActualImagen++;
    if (posicionActualImagen > anchoImagen) {
        int pixel;
        if ((pixel = fgetc(archivoOriginal)) != EOF) {
            int i;
            fseek(archivoOriginal, -1, SEEK_CUR);    /*Regresa un caracter */
            for (i = 1; i <= anchoImagen; i++) {
                bufferImagen[filaSuperior][i] = (unsigned char) fgetc(archivoOriginal);
            }
            posicionActualImagen = 1;
            filaSuperior = !filaSuperior;
            ultimoCaracter = bufferImagen[!filaSuperior][posicionActualImagen];
        }
    }
    else {
        ultimoCaracter = bufferImagen[!filaSuperior][posicionActualImagen];
    }

    return ultimoCaracter;
}

bool esFinDeLinea() {
    bool esFinDeLinea = posicionActualImagen == 1;
    return esFinDeLinea;
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
=======
#include <stdbool.h>
#include <stdio.h>

#include "../include/pixelio.h"
<<<<<<< HEAD
>>>>>>> 185eb18 (Se cambiarona lagunos tipos y se terminó archivo de encabezados.)
=======

void write_code(FILE* outFile, unsigned int code, uint16_t l, BYTE &buffer, BYTE &buffBits) {
  // Escribe el siguiente código en el archivo de la imagen comprimida

  BYTE b;

  if (buffBits == 0) { // Si no hay nada en el buffer

    buffBits = l % 8; // Número de bits que sobran
    buffer = code & ((1<<buffBits) - 1); // LSBs

    // Escribe archivo
  }

}
>>>>>>> c073309 (Corregidos errores de sintaxis.)
