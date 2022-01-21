#ifndef datosCompresion_h
#define datosCompresion_h

#include <stdio.h>

/*
 * Estructura que contiene información de la compresión
 * Ejemplo: Tasa de compresión.
 */
typedef struct _dtCompresion * DatosCompresion;

/*
 * Reserva memoria para la estructura
 */
DatosCompresion crearDatosCompresion( int cantidadPixeles );

/*
 * Actualiza la información actual a medida que se comprimen pixeles
 */
void actualizarDatosCompresion( DatosCompresion dc, int cantidadComprimida );

/*
 * Finaliza la recolección de datos con los obtenidos
 * hasta el momento
 */
void guardarDatos(DatosCompresion dc);

/*
 * Libera la memoria de la estructura
 */
void destruirDatosCompresion(DatosCompresion dc);

/*
 * Getters de la estructura
 */
float obtenerTasaCompresion(DatosCompresion dc);
long obtenerBitsComprimidos(DatosCompresion dc);
long obtenerCantidadPixeles(DatosCompresion dc);

#endif