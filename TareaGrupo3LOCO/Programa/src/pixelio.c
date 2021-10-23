#include "../include/arregloCircular.h"
#include "../include/pixelio.h"
#include "../include/compartido.h"

#include <stdio.h>

static unsigned char * bufferCodificado;    /* Contiene los últimos bits codificados.                 */
static int indiceBufferCodificado;          /* Indica cual fue el último bit agregado.                */

static ArregloCircular * bufferImagen;      /* Contiene los últimos pixeles recibidos, los cuales.    */
static int tamanioBufferImagen;             /* serán necesarios para obtener a,b,c y d.               */

static int distanciaXyBordeImagen;          /* Si vale 0 el pixel actual está en el borde izquierdo   */
static int anchoImagen;                     /* de la imagen. El ancho ayuda a actualizar su valor     */


void inicializarBuffer( int ancho ) {
    distanciaXyBordeImagen = 0;
    bufferCodificado = 0x00;
    anchoImagen = ancho;
    tamanioBufferImagen = ancho + 2;    /* Caracteres entre c y x */
    bufferImagen = crearCola(tamanioBufferImagen);
}

char obtenerUltimoCaracter( FILE * archivoDescomprimido ) {
    char ultimoCaracter;

    fscanf(archivoDescomprimido, &ultimoCaracter);
    encolar(bufferImagen, (unsigned char) ultimoCaracter);
    distanciaXyBordeImagen++;
    if (distanciaXyBordeImagen >= anchoImagen) {
        distanciaXyBordeImagen = 0;
    }
    return ultimoCaracter;
}

void determinarContexto( unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d ) {
    /* Si se está fuera de los límites de la imagen se devuelve 0,  */
    /* si no se obtiene el pixel del buffer,                        */
    /* Se asume que esto solo sucede en los laterales de x (a,d)    */
    /* debido al diseño del buffer.                                 */

    *a = (distanciaXyBordeImagen == 0) ? 0 : obtenerElemento(bufferImagen, tamanioBufferImagen - 2);
    *c = obtenerElemento(bufferImagen, 0);
    *b = obtenerElemento(bufferImagen, 1);
    *d = (distanciaXyBordeImagen == anchoImagen - 1) ? 0 : obtenerElemento(bufferImagen, 2);
}

void actualizarBuffer( int output, int cantidadBits, FILE * archivoComprimido ) {
    char nuevoCaracter = 0;
    int i,j;
    int cantCaracteresAImprimir = (cantidadBits + indiceBufferCodificado) >> 3; /* a << k = a div 2^k */
    
    for (i = 0; i < cantCaracteresAImprimir; i++) {
        for (j = indiceBufferCodificado; j >= 0; j--) {
            //En esta linea se leeria el nuevo caracter (falta implementar)
        }
        fprintf(nuevoCaracter, &nuevoCaracter);
        encolar(bufferCodificado, (unsigned char) nuevoCaracter);
    }
}

void destruirBuffer() {
    destruirArregloCircular(bufferImagen);
}