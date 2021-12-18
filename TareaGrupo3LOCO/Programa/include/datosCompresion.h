#include <stdio.h>

typedef struct _dtCompresion * DatosCompresion;

DatosCompresion crearDatosCompresion( int cantidadPixeles );
void actualizarDatosCompresion( DatosCompresion dtComp, int cantidadComprimida );
void guardarDatos(DatosCompresion dtComp);
void destruirDatosCompresion(DatosCompresion dc);

float obtenerTasaCompresion(DatosCompresion dc);
float obtenerBitsComprimidos(DatosCompresion dc);
float obtenerCantidadPixeles(DatosCompresion dc);
