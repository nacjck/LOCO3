#include "../include/pixelio.h"

#include <string.h>

struct _imagen {
    unsigned char * filas[2];             /* Ultimas dos filas de imagen             */
    int altura,ancho;
    int pixelActual;                    /* Si se esta en el borde izquierdo vale 0 */
    bool filaSuperior;                            /* 0 o 1                                   */
};

Imagen crearImagen( int ancho ) {
    Imagen res = malloc(sizeof(struct _imagen));
    res->filaSuperior = 0;
    res->pixelActual = ancho + 1;
    res->ancho = ancho;
    res->filas[0] = malloc(ancho * sizeof(unsigned char) + 2);
    res->filas[1] = malloc(ancho * sizeof(unsigned char) + 2);
    memset(res->filas[!res->filaSuperior], 0, ancho + 2);
    memset(res->filas[ res->filaSuperior], 0, ancho + 2);
}

void escribirCabezalPGM( Imagen img, FILE * archivoInput, FILE * archivoOutput ) {
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
    fscanf(archivoInput,"%d",&(img->ancho));      
    fprintf(archivoOutput,"%d",img->ancho);

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

void determinarContexto( Imagen img, unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d ) {
    /* Si se está fuera de los límites de la imagen se devuelve 0.  */
    /* Se asume que esto siempre sucede debido a la inicialización  */
    /* del buffer.                                                  */

    *a = img->filas[!img->filaSuperior][img->pixelActual - 1];
    *b = img->filas[ img->filaSuperior][img->pixelActual    ];
    *c = img->filas[ img->filaSuperior][img->pixelActual - 1];
    *d = img->filas[ img->filaSuperior][img->pixelActual + 1];
}

void destruirImagen( Imagen img ) {
    free(img->filas[0]);
    free(img->filas[1]);
}

/* SOLO COMPRESOR */
int obtenerUltimoCaracter( Imagen img, FILE * archivoOriginal ) {
    int ultimoCaracter = EOF;

    img->pixelActual++;
    if (img->pixelActual > img->ancho) {
        int pixel;
        if ((pixel = fgetc(archivoOriginal)) != EOF) {
            int i;
            fseek(archivoOriginal, -1, SEEK_CUR);    /*Regresa un caracter */
            for (i = 1; i <= img->ancho; i++) {
                img->filas[img->filaSuperior][i] = (unsigned char) fgetc(archivoOriginal);
            }
            img->pixelActual = 1;
            img->filaSuperior = !img->filaSuperior;
            ultimoCaracter = img->filas[!img->filaSuperior][img->pixelActual];
        }
    }
    else {
        ultimoCaracter = img->filas[!img->filaSuperior][img->pixelActual];
    }

    return ultimoCaracter;
}

bool esFinDeLinea( Imagen img ) {
    bool esFinDeLinea = img->pixelActual == 1;
    return esFinDeLinea;
}