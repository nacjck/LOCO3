/* 
 * descompresor.h
 *
 * Brinda las herramientas necesarias para descomprimir
 * un archivo dados los parámetros necesarios
 * 
 */

#ifndef descompresor_h
#define descompresor_h

#include "compartido.h"
#include "modalidad.h"
#include "pixelio.h"

/*
 * Precondiciones: 
 *     -archivoEntrada y archivoSalida no son nulos.
 */
void descomprimir( char* archivoEntrada, char* archivoSalida );

#endif