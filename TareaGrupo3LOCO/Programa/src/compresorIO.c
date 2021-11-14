#include "../include/pixelio.h"
#include "../include/compresorIO.h"

#include <string.h>
#include <stdio.h>

#define MAX_BUFFER 32
#define contenidoBuffer_t unsigned char
#define bool int

/*
 * Constantes para manejo de output segun tipos de datos 
 */
static const int SIZE_BLOQUE_CODIFICADO = sizeof(contenidoBuffer_t);
static const int MAX_INDICE_BLOQUE = (int)((sizeof(contenidoBuffer_t) << 3) - 1);

/*
 * Variables para codificacion 
 */
static contenidoBuffer_t bufferCodificado[MAX_BUFFER];   /* Contiene ultimos caracteres a imprimir */
static contenidoBuffer_t * actualBloqueCodificado;       /* Puntero a siguiente bloque a imprimir  */
static int indiceBitCodificado;                         /* posicion binaria                        */

void escribirCabezal( FILE * archivoComprimido, int s, Modalidad modalidad ) {
    //Por verse
}

int determinarAnchoImagen( FILE * archivoOriginal ) {
    int ancho = 0;

    while(getc(archivoOriginal) != '\n');   /* P5 */
    while (getc(archivoOriginal) == '#') {  /* Comentarios */
        while (getc(archivoOriginal) != '\n');
    }
    fseek(archivoOriginal, -1, SEEK_CUR);   /* Se regresa un caracter */
    fscanf(archivoOriginal, "%d", &ancho);
    while(getc(archivoOriginal) != '\n');    /* altura */
    while(getc(archivoOriginal) != '\n');    /* Magic value */

    return ancho;
}

void inicializarBufferCompresion() {
    int i;
    memset(bufferCodificado, 0, MAX_BUFFER);
    actualBloqueCodificado = bufferCodificado;
    indiceBitCodificado = MAX_INDICE_BLOQUE;
}

void actualizarBuffer( unsigned int output, int cantidadBits, FILE * archivoComprimido ) {
    /* Output carga los bits del mas al menos significativo  */
    /* (Little-Endian).                                      */
    /* No se enmascara 'output' porque se asume que los bits */
    /* no utilizados son ceros.                              */
    int distanciaBits;
    contenidoBuffer_t mascara;
    bool hayBits = 1;

    distanciaBits = indiceBitCodificado - cantidadBits + 1;
    /*
     *  Itera un máximo de 
     *  ( sizeof(int) / 8*SIZE_BLOQUE_CODIFICADO ) + 1 veces.
     *  Carga el buffer elemento a elemento hasta vaciar el output.
     *  En caso de llenarse el buffer, se imprimen todos los bits
     *  almacenados hasta el momento en el archivo indicado.
     */
    do {
        if(distanciaBits >= 0) {
            mascara = (contenidoBuffer_t) (output << distanciaBits);
            indiceBitCodificado = distanciaBits - 1;
            hayBits = 0;
        }
        else {
            mascara = (contenidoBuffer_t) (output >> -distanciaBits);
            distanciaBits += indiceBitCodificado + 1;
            indiceBitCodificado = -1;
        }
        *actualBloqueCodificado |= mascara;
        if(indiceBitCodificado < 0) {
            indiceBitCodificado = MAX_INDICE_BLOQUE;
            actualBloqueCodificado++;
            if (actualBloqueCodificado == bufferCodificado + MAX_BUFFER) {
                fwrite(bufferCodificado, SIZE_BLOQUE_CODIFICADO, MAX_BUFFER, archivoComprimido);
                actualBloqueCodificado = bufferCodificado;
            }
        }
    } while (hayBits);
}

void vaciarBuffer( FILE * archivoComprimido ) {
    fwrite(bufferCodificado, SIZE_BLOQUE_CODIFICADO,
          (size_t)(actualBloqueCodificado - bufferCodificado + 1), archivoComprimido);
}

int main() {
    FILE * f = fopen("testCompresion.txt","wb");
    inicializarBufferCompresion();
    int i;
    actualizarBuffer(0,32,f);
    actualizarBuffer(0xff,20,f);
    
    for(i = 0; i < 5; i++) {
        printf("Iteracion: %d \n",i);
        
        actualizarBuffer(0,2,f);
        actualizarBuffer(1,2,f);
       
    }
    
    vaciarBuffer(f);
    fclose(f);

    FILE * f2 = fopen("testCompresion.txt","rb");
    int c;

    while ((c = fgetc(f2)) != EOF) {
        printf("%u \n",(unsigned char)c);
    }
    fclose(f2);
    
    f = fopen("tests/tools.pgm","rb");
    printf("%d \n",determinarAnchoImagen(f));
    printf("fin");
    fclose(f);
}