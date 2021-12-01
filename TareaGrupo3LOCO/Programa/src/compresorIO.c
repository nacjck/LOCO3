#include "../include/pixelio.h"
#include "../include/compresorIO.h"

#include <string.h>
#include <stdio.h>

#define MAX_BUFFER 256
#define contenidoBuffer_t unsigned char
#define bool int

/*
 * Constantes para manejo de output segun tipos de datos
 */

static const size_t BITS_INT = (sizeof(int) << 3);
static const int SIZE_BLOQUE_CODIFICADO = sizeof(contenidoBuffer_t);
static const int MAX_INDICE_BLOQUE = (int)((sizeof(contenidoBuffer_t) << 3) - 1);

/*
 * Variables para codificacion
 */
static contenidoBuffer_t bufferCodificado[MAX_BUFFER];   /* Contiene ultimos caracteres a imprimir  */
static contenidoBuffer_t * actualBloqueCodificado;       /* Puntero a siguiente bloque a imprimir   */
static int indiceBitCodificado;                          /* posicion binaria                        */

void escribirParametrosCabezal( FILE * archivoComprimido, int s, Modalidad modalidad ) {
    fprintf(archivoComprimido,"%d\n",s);
    fprintf(archivoComprimido,"%d\n",modalidad);
}

void inicializarBufferCompresion() {
    memset(bufferCodificado, 0, MAX_BUFFER);
    actualBloqueCodificado = bufferCodificado;
    indiceBitCodificado = MAX_INDICE_BLOQUE;
}

/* AUXILIAR                                */
/* Comprueba que el buffer no esté lleno y */
/* en caso de estarlo lo imprime           */
void controlarBuffer(FILE * archivoComprimido) {
    if (actualBloqueCodificado == (bufferCodificado + MAX_BUFFER)) {
        fwrite(bufferCodificado, SIZE_BLOQUE_CODIFICADO, MAX_BUFFER, archivoComprimido);
        actualBloqueCodificado = bufferCodificado;
        memset(bufferCodificado, 0, MAX_BUFFER);
    }
}

/* AUXILIAR
 * Carga el buffer elemento a elemento hasta vaciar el output.
 * En caso de llenarse el buffer, se imprimen todos los bits
 * almacenados hasta el momento en el archivo indicado.
 *
 *  Output carga los bits del más al menos significativo (Little-Endian).
 *  Se asume que los bits no utilizados son ceros.
 */
void actualizarBuffer( unsigned int output, int cantidadBits, FILE * archivoComprimido ) {
    int i;
    int distanciaBits;    /* Distancia entre ultimo bit codificado y bit a imprimir */
    contenidoBuffer_t mascara;
    int bitOutput = 0;

    /* Coloca en el buffer cada bit uno por uno */
    for (i = cantidadBits; i > 0; i--) {
        distanciaBits = indiceBitCodificado - i + 1;
        if (distanciaBits < 0) {
            mascara =  (output >> -distanciaBits);
            bitOutput = mascara & (1 << i + distanciaBits + 1);
        }
        else {
            mascara =  (output << distanciaBits);
            bitOutput = mascara & (1 << indiceBitCodificado);
        }
        *actualBloqueCodificado |= mascara;
        indiceBitCodificado--;

        /* Si no quedan bits por colocar en el bloque actual */
        if (indiceBitCodificado < 0) {
            actualBloqueCodificado++;
            indiceBitCodificado = MAX_INDICE_BLOQUE;
            controlarBuffer(archivoComprimido);
        }
    }
}

void imprimirCompresion( int golombBinario, int largoGolombBinario, int largoGolombUnario, FILE * archivoComprimido) {
    actualizarBuffer(golombBinario, largoGolombBinario, archivoComprimido);
    while(largoGolombUnario > BITS_INT) {
        actualizarBuffer(0, BITS_INT, archivoComprimido);
        largoGolombUnario -= BITS_INT;
    }
    actualizarBuffer(1 , largoGolombUnario, archivoComprimido);
}

void vaciarBuffer( FILE * archivoComprimido ) {
    fwrite(bufferCodificado, SIZE_BLOQUE_CODIFICADO,
          (size_t)(actualBloqueCodificado - bufferCodificado + (indiceBitCodificado != MAX_INDICE_BLOQUE)),
           archivoComprimido);
}


/*int main() {
    FILE * f = fopen("testCompresion.bin","wb");
    inicializarBufferCompresion();
    int i;

    /*for( i = 0; i < 8*416/32; ++i) {
        actualizarBuffer(1,32,f);
        /*actualizarBuffer(1,2,f);
        actualizarBuffer(0,2,f);
        actualizarBuffer(0,2,f);
    }*/

    //actualizarBuffer(1,32,f);
    /*for( i = 0; i < 16; ++i) {
        actualizarBuffer(1,1,f);
        actualizarBuffer(0,1,f);
    }
    actualizarBuffer(1,1,f);
    actualizarBuffer((1 << 31) | (1 << 17) | (1 << 15) | (1 << 9) | (1 << 8),32,f);
    printf("asda %d\n",bufferCodificado[6]);
    actualizarBuffer(1,1,f);
    //actualizarBuffer((1 << 15) | (1 << 9) | (1 << 8),16,f);
    //actualizarBuffer(1,1,f);
    //actualizarBuffer(0xff,31,f);

    /*for(i = 0; i < 5; i++) {
        printf("Iteracion: %d \n",i);

        actualizarBuffer(0,2,f);
        actualizarBuffer(1,2,f);

    }*/

    /*vaciarBuffer(f);
    fclose(f);

    FILE * f2 = fopen("testCompresion.bin","rb");
    int c;

    printf("IMPRESION \n");
    while ((c = fgetc(f2)) != EOF) {
        printf("%u \n",(unsigned char)c);
    }
    fclose(f2);

    f = fopen("tests/tools.pgm","rb");
    printf("%d \n",determinarAnchoImagen(f));
    printf("fin");
    fclose(f);
}*/
