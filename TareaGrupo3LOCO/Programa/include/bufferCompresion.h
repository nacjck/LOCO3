#ifndef bufferCompresion_h
#define bufferCompresion_h

#include <stdio.h>

//Buffer de bits que serán impresos
typedef struct _bufferComprimido * BufferCompresion;

BufferCompresion crearBufferCompresion();

void imprimirCompresion( BufferCompresion buf, int golombBinario, int largoGolombBinario, int largoGolombUnario, FILE * archivoComprimido);

void vaciarBuffer( BufferCompresion buf, FILE * archivoComprimido );

void destruirBufferCompresion( BufferCompresion buf );

#endif