#ifndef datosCompresion_h
#define datosCompresion_h

#include <stdio.h>

typedef struct _dtCompresion * DatosCompresion;

DatosCompresion crearDatosCompresion( int cantidadPixeles );
void actualizarDatosCompresion( DatosCompresion dc, int cantidadComprimida );
void guardarDatos(DatosCompresion dc);
void destruirDatosCompresion(DatosCompresion dc);

float obtenerTasaCompresion(DatosCompresion dc);
long obtenerBitsComprimidos(DatosCompresion dc);
long obtenerCantidadPixeles(DatosCompresion dc);

#endif