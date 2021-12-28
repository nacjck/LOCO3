#include "../include/imagen.h"

#include <string.h>

struct _datosCabezal {
    int altura,ancho;
    int maxValue;
};

struct _imagen {
    PIX * filas[2];             /* Ultimas dos filas de imagen             */
    DatosCabezal cabezal;
    int pixelActual;                    /* Si se esta en el borde izquierdo vale 0 */
    bool filaSuperior;                            /* 0 o 1                                   */
};

Imagen crearImagen( DatosCabezal dtCabezal ) {
    Imagen res = malloc(sizeof(struct _imagen));

    res->filaSuperior = 0;
    res->pixelActual = 0;
    res->cabezal = dtCabezal;
    res->filas[0] = malloc(dtCabezal->ancho * sizeof(PIX) + 2);
    res->filas[1] = malloc(dtCabezal->ancho * sizeof(PIX) + 2);
    memset(res->filas[!res->filaSuperior], 0, dtCabezal->ancho + 2);
    memset(res->filas[ res->filaSuperior], 0, dtCabezal->ancho + 2);

    return res;
}

DatosCabezal escribirCabezalPGM( FILE * archivoInput, FILE * archivoOutput ) {
    DatosCabezal res = malloc(sizeof(struct _datosCabezal));
    int c;
    char tipo[2];
    
    //P5
    fscanf(archivoInput, "%s", tipo);
    if (strcmp(tipo, "P5")) {
        puts("Tipo de imagen no soportado.");
        exit(1);
    }
    fprintf(archivoInput, "%s", tipo);

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
    }

    //Se regresa un caracter
    fseek(archivoInput,-1,SEEK_CUR);       
    
    //Ancho y altura
    fscanf(archivoInput, "%d %d", &(res->ancho), &(res->altura));
    fprintf(archivoOutput,"%d %d",res->ancho, res->altura);
    
    do {
        c = getc(archivoInput);
        fprintf(archivoOutput,"%c",c);
    } while(c != '\n');
    
    //MaxValue
    fscanf(archivoInput, "%d", &(res->maxValue));
    fprintf(archivoOutput,"%d", res->maxValue);
    
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

void determinarContexto( Imagen img, PIX * a, PIX * b, PIX * c, PIX * d ) {
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
    free(img);
}

void destruirDatosCabezal( DatosCabezal dtCabezal ) {
    free(dtCabezal);
}

void avanzarPixel( Imagen img ) {
    img->pixelActual++;
    if (img->pixelActual > img->cabezal->ancho) {
        img->pixelActual = 1;
        img->filaSuperior = !img->filaSuperior;
    }
}

/* SOLO COMPRESOR */
int obtenerUltimoCaracter( Imagen img, FILE * archivoOriginal ) {
    int ultimoCaracter;

    if (img->pixelActual == 1) {
        int i;
        for (i = 1; i <= img->cabezal->ancho; i++) {
            img->filas[!img->filaSuperior][i] = (PIX) fgetc(archivoOriginal);
        }
    }
    ultimoCaracter = img->filas[!img->filaSuperior][img->pixelActual];
    return ultimoCaracter;
}

void agregarCaracter( Imagen img, PIX nuevoCaracter ) {
    img->filas[!img->filaSuperior][img->pixelActual] = nuevoCaracter;
}

bool esFinDeLinea( Imagen img ) {
    bool esFinDeLinea = img->pixelActual == img->cabezal->ancho;
    return esFinDeLinea;
}
