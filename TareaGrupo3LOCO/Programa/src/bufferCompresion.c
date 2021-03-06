#include "../include/imagen.h"
#include "../include/bufferCompresion.h"

#include <string.h>
#include <stdio.h>

#define MAX_BUFFER 256

/*
 * Constantes para manejo de output segun tipos de datos.
 */
static const int BITS_INT = (sizeof(int) << 3);
static const int BYTES_UCHAR = sizeof(unsigned char);
static const int MAX_BIT_INDEX_UCHAR = (int)((sizeof(unsigned char) << 3) - 1);

struct _bufferComprimido {
    unsigned char buffer[MAX_BUFFER];   /* Contiene ultimos caracteres a imprimir  */
    unsigned char * finBuffer;          /* Puntero a siguiente bloque a imprimir   */
    int bitActual;                      /* posicion binaria                        */
};

BufferCompresion crearBufferCompresion() {
    BufferCompresion res = malloc(sizeof(struct _bufferComprimido));
    
    memset(res->buffer, 0, MAX_BUFFER);
    res->finBuffer = res->buffer;
    res->bitActual = MAX_BIT_INDEX_UCHAR;

    return res;
}

/* AUXILIAR
 * Comprueba que el buffer no esté lleno y
 * en caso de estarlo imprime sus caracteres.
 */
void controlarBuffer( BufferCompresion buf, FILE * archivoComprimido ) {
    if (buf->finBuffer == (buf->buffer + MAX_BUFFER)) {
        fwrite(buf->buffer, BYTES_UCHAR, MAX_BUFFER, archivoComprimido);
        buf->finBuffer = buf->buffer;
        memset(buf->buffer, 0, MAX_BUFFER);
    }
}

/* AUXILIAR
 * Carga el buffer elemento a elemento hasta vaciar el buffer.
 * En caso de llenarse, se imprimen todos los bits
 * almacenados hasta el momento en el archivo indicado.
 *
 *  Output carga los bits del más al menos significativo (Little-Endian).
 *  Se asume que los bits no utilizados son ceros.
 */
void actualizarBuffer( BufferCompresion buf, unsigned int output, int cantidadBits, FILE * archivoComprimido ) {
    int i;
    int distanciaBits;      /* Distancia entre ultimo bit codificado y bit a imprimir */
    unsigned char mascara;  /* Mascara utilizada para ingresar nuevo caracter         */
    int bitOutput = 0;      /* Nuevo bit                                              */

    /* Coloca en el buffer cada bit uno por uno */
    for (i = cantidadBits; i > 0; i--) {
        distanciaBits = buf->bitActual - i + 1;
        if (distanciaBits < 0) {
            mascara =  (output >> -distanciaBits);
            bitOutput = mascara & (1 << i + distanciaBits + 1);
        }
        else {
            mascara =  (output << distanciaBits);
            bitOutput = mascara & (1 << buf->bitActual);
        }
        *(buf->finBuffer) |= mascara;
        buf->bitActual--;

        /* Si no quedan bits por colocar en el caracter actual */
        if (buf->bitActual < 0) {
            buf->finBuffer++;
            buf->bitActual = MAX_BIT_INDEX_UCHAR;
            controlarBuffer(buf, archivoComprimido);
        }
    }
}

/*
 * Almacena los bits de la parte binaria de golomb,
 * luego todos los ceros de la parte unaria 
 * y finalmente almacena un 1 (binario).
 */
void imprimirCompresion( BufferCompresion buf, int golombBinario, int largoGolombBinario, int largoGolombUnario, FILE * archivoComprimido) {
    actualizarBuffer(buf, golombBinario, largoGolombBinario, archivoComprimido);
    while(largoGolombUnario > BITS_INT) {
        actualizarBuffer(buf, 0, BITS_INT, archivoComprimido);
        largoGolombUnario -= BITS_INT;
    }
    actualizarBuffer(buf, 1, largoGolombUnario, archivoComprimido);
}

/*
 * Imprime todos los bits aun no impresos (Si la cantdad de bits
 * no es múltiplo de 8 se agregarán bits con valor indefinido hasta
 * que lo sea).
 */
void vaciarBuffer( BufferCompresion buf, FILE * archivoComprimido ) {
    fwrite(buf->buffer, BYTES_UCHAR,
          (size_t)(buf->finBuffer - buf->buffer + (buf->bitActual != MAX_BIT_INDEX_UCHAR)),
           archivoComprimido);
}

void destruirBufferCompresion( BufferCompresion buf ) {
    free(buf);
}
