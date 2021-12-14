#include "../include/pixelio.h"

#include <string.h>

struct _datosCabezal {
    int altura,ancho;
    int maxValue;
};

struct _imagen {
    unsigned char * filas[2];             /* Ultimas dos filas de imagen             */
    DatosCabezal cabezal;
    int pixelActual;                    /* Si se esta en el borde izquierdo vale 0 */
    bool filaSuperior;                            /* 0 o 1                                   */
};

Imagen crearImagen( DatosCabezal dtCabezal ) {
    Imagen res = malloc(sizeof(struct _imagen));

    res->filaSuperior = 0;
    res->pixelActual = dtCabezal->ancho + 1;
    res->cabezal = dtCabezal;
    res->filas[0] = malloc(dtCabezal->ancho * sizeof(unsigned char) + 2);
    res->filas[1] = malloc(dtCabezal->ancho * sizeof(unsigned char) + 2);
    memset(res->filas[!res->filaSuperior], 0, dtCabezal->ancho + 2);
    memset(res->filas[ res->filaSuperior], 0, dtCabezal->ancho + 2);

    return res;
}

DatosCabezal escribirCabezalPGM( FILE * archivoInput, FILE * archivoOutput ) {
    DatosCabezal res = malloc(sizeof(struct _datosCabezal));
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
    fscanf(archivoInput,"%d",&(res->ancho));      
    fprintf(archivoOutput,"%d ",res->altura);

    //Altura
    fscanf(archivoInput,"%d",&(res->altura));      
    fprintf(archivoOutput,"%d",res->altura);

    //Resto
    do {
        c = getc(archivoInput);
        fprintf(archivoOutput,"%c",c);
    } while(c != '\n');

    //MAX_VALUE
    do {
        c = getc(archivoInput);
        fprintf(archivoOutput,"%c",c);
    } while(c != '\n');

    return res;
}

int obtenerAncho( Imagen img ) {
    return img->cabezal->ancho;
}

int obtenerAltura( Imagen img ) {
    return img->cabezal->altura;
}

int obtenerMaxValue( Imagen img ) {
    return img->cabezal->maxValue;
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

void destruirDatosCabezal( DatosCabezal dtCabezal ) {
    free(dtCabezal);
}

/* SOLO COMPRESOR */
int obtenerUltimoCaracter( Imagen img, FILE * archivoOriginal ) {
    int ultimoCaracter = EOF;

    img->pixelActual++;
    if (img->pixelActual > img->cabezal->ancho) {
        int pixel;
        if ((pixel = fgetc(archivoOriginal)) != EOF) {
            int i;
            fseek(archivoOriginal, -1, SEEK_CUR);    /*Regresa un caracter */
            for (i = 1; i <= img->cabezal->ancho; i++) {
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

void agregarCaracter( Imagen img, unsigned char nuevoCaracter ) {
    img->pixelActual++;
    if (img->pixelActual > img->cabezal->ancho) {
        int pixel;
            img->pixelActual = 1;
            img->filaSuperior = !img->filaSuperior;
    }
    img->filas[!img->filaSuperior][img->pixelActual] = nuevoCaracter;
}

bool esFinDeLinea( Imagen img ) {
    bool esFinDeLinea = img->pixelActual == 1;
    return esFinDeLinea;
}