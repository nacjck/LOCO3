/* 
 * compresor.h
 *
 * Brinda las herramientas necesarias para descomprimir
 * un archivo dados los parámetros necesarios
 * 
 */

#ifndef compresor_h
#define compresor_h

#include "compartido.h"
#include "pixelio.h"
#include "datosCompresion.h"

/*
 * Precondiciones: 
 *     -archivoEntrada y archivoSalida no son nulos.
 *     - 0 <= s <= 10
 */
DatosCompresion comprimir( char* archivoEntrada, char* archivoSaluda, int s, bool run );

#endif
