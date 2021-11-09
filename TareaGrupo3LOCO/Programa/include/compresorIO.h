#ifndef compresorIO_h
#define compresorIO_h

void escribirCabezal( FILE * archivoComprimido, int s, Modalidad modalidad );

int determinarAnchoImagen( FILE * archivoDescomprimido );

void inicializarBufferCompresion( int ancho );

int obtenerUltimoCaracter();

void actualizarBuffer( int output, int cantidadBits, FILE * decompressedFile );

void vaciarBuffer( FILE * archivoComprimido );

#endif