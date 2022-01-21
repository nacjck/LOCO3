/*
 * descompresor.h
 *
 * Brinda las herramientas necesarias para descomprimir
 * un archivo dados los parámetros necesarios
 *
 */

#ifndef descompresor_h
#define descompresor_h

#include "datosCompresion.h"

/*
 * Descomprime el archivo de entrada en el archivo de salida ingresado
 *
 * Precondiciones: 
 *     -El archivo de entrada contiene los valores de 's' y del modo de run
 *     requeridos
 */
DatosCompresion descomprimir( char* archivoEntrada, char* archivoSalida );

#endif
