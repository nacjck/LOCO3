#ifndef compresorIO_h
#define compresorIO_h

#include <stdio.h>

#define MAX_BUFFER 256

//Buffer de bits que serán impresos
typedef struct _bufferComprimido * BufferCompresion;

BufferCompresion crearBufferCompresion();

void imprimirCompresion( BufferCompresion buf, int golombBinario, int largoGolombBinario, int largoGolombUnario, FILE * archivoComprimido);

void vaciarBuffer( BufferCompresion buf, FILE * archivoComprimido );

void destruirBufferCompresion( BufferCompresion buf );

#endif