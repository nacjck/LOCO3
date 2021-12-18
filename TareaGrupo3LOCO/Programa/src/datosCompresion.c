#include "../include/datosCompresion.h"

#include <stdlib.h>

struct _dtCompresion {
    float tasaCompresion;
    long bitsComprimidos;
    long cantidadPixeles;
};

DatosCompresion crearDatosCompresion( int cantidadPixeles ) {
    DatosCompresion res = malloc(sizeof(struct _dtCompresion));

    res->tasaCompresion = 0;
    res->cantidadPixeles = cantidadPixeles;
    res->bitsComprimidos = 0;
    
    return res;
}

void actualizarDatosCompresion( DatosCompresion dtComp, int cantidadComprimida ) {
    dtComp->bitsComprimidos += cantidadComprimida;
}

void guardarDatos(DatosCompresion dtComp) {
    if (dtComp->cantidadPixeles > 0) {
        dtComp->tasaCompresion = ((float)dtComp->bitsComprimidos) / (dtComp->cantidadPixeles << 3);
    }
}

void destruirDatosCompresion(DatosCompresion dc) {
    free(dc);
}

float obtenerTasaCompresion(DatosCompresion dc) {
    return dc->tasaCompresion;
}

long obtenerBitsComprimidos(DatosCompresion dc) {
    return dc->bitsComprimidos;
}

long obtenerCantidadPixeles(DatosCompresion dc) {
    return dc->cantidadPixeles;
}