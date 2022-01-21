/* 
 * compresor.h
 *
 * Brinda las herramientas necesarias para descomprimir
 * un archivo dados los parámetros necesarios
 * 
 */

#ifndef compresor_h
#define compresor_h

#include "extractos.h"
#include "imagen.h"
#include "datosCompresion.h"

/*
 * Comprime la imagen de entrada en el archivo de salida ingresado
 * con los parámetros indicados.
 *
 * Precondiciones: 
 *     -archivoEntrada y archivoSalida no son nulos.
 *     - 0 <= s <= 10
 */
DatosCompresion comprimir( char* archivoEntrada, char* archivoSalida, int s, bool run );

#endif
