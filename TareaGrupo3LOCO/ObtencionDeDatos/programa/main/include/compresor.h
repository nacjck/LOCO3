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
#include "modalidad.h"

typedef struct {
    //int * tasaCompFila;
    float tasaCompTotal;
    long cantComprimida;
    long cantPixeles;
} DtCompresion;

/*
 * Precondiciones: 
 *     -archivoEntrada y archivoSalida no son nulos.
 *     - 0 <= s <= 10
 */
DtCompresion comprimir( char* archivoEntrada, char* archivoSaluda, int s, Modalidad modalidad );

#endif
