#ifndef bufferCompresion_h
#define bufferCompresion_h

#include <stdio.h>

/*
 * Buffer de bits que serán impresos
 */
typedef struct _bufferComprimido * BufferCompresion;

/*
 * Reserva memoria para el buffer
 */
BufferCompresion crearBufferCompresion();

/*
 * Ingresa al buffer la información de una codificación de golomb.
 * En caso de llenarse el buffer imprime en el archivo ingresado.
 */
void imprimirCompresion( BufferCompresion buf, 
    int golombBinario, int largoGolombBinario, int largoGolombUnario,
    FILE * archivoComprimido);

/*
 * Fuerza al buffer a imprimir todo su contenido en el archivo ingresado (flush)
 */
void vaciarBuffer( BufferCompresion buf, FILE * archivoComprimido );

/*
 * Libera la memoria del buffer
 */
void destruirBufferCompresion( BufferCompresion buf );

#endif