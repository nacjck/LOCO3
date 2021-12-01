#include "../include/compresor.h"
#include "../include/compresorIO.h"
#include "../include/pixelio.h"

#include <stdio.h>

//RUTINAS AUXILIARES PARA LA RECOLECCION DE DATOS
DtCompresion crearDtCompresion( FILE * archivoOriginal ) {
    DtCompresion res;

    res.tasaCompTotal = 0;
    res.cantPixeles = - ftell(archivoOriginal);
    res.cantComprimida = 0;
    
    return res;
}

void actualizarDatosCompresion( DtCompresion * dtComp, int cantidadComprimida ) {
    dtComp->cantComprimida += cantidadComprimida;
}

void guardarDatos(DtCompresion * dtComp, FILE * archivoOriginal) {
    dtComp->cantPixeles += ftell(archivoOriginal);
    dtComp->tasaCompTotal = ((float)dtComp->cantComprimida) / (dtComp->cantPixeles << 3);
}

DtCompresion comprimir( char* archivoEntrada, char* archivoSalida, int s, Modalidad modalidad ) {
    FILE * archivoComprimido;
    FILE * archivoOriginal;
    int anchoImagen;
    int kGolomb;
    int ultimoCaracterLeido;        /* Promoción temporal de x a entero        */
    unsigned char x;
    unsigned char xPrediccion;      /* x^                                      */
    int errorPrediccion;            /* e = x - x^                              */
    Extracto * fExtracto;           /* f(C)                                    */
    unsigned char a,b,c,d;          /* Contexto                                */
    int mapeoRice;
    unsigned int golombBinario;
    int largoGolombBinario;       /* Cantidad de bits de binary(M) */
    int largoGolombUnario;        /* Cantidad de bits de unary (M) */
    
    archivoComprimido = fopen(archivoSalida, "wb");
    archivoOriginal = fopen(archivoEntrada, "rb");
    
    escribirParametrosCabezal(archivoComprimido, s, modalidad);
    escribirCabezalPGM(archivoOriginal, archivoComprimido, &anchoImagen);
    inicializarExtractos(s);
    inicializarBuffer(anchoImagen);
    inicializarBufferCompresion();
    
    //INFORMACION RETORNADA
    DtCompresion dtComp = crearDtCompresion(archivoOriginal);
    
    if (modalidad == RUN) {
        ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal);
        while (ultimoCaracterLeido != EOF) {
            x = (unsigned char) ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            xPrediccion = predecirX(a, b, c);
            fExtracto = determinarExtracto(xPrediccion, a, b, c);
            errorPrediccion = x - xPrediccion;
            if (a!=b || b!=c || c!=d) {    /* No es modo de run */
                kGolomb = determinarGolombK(fExtracto);
                mapeoRice = determinarMapeoRice(errorPrediccion);
                largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, mapeoRice, &golombBinario);
                largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, mapeoRice);
                actualizarExtracto(fExtracto,errorPrediccion);
                ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal);
            }
            else {
                int l = -1;    /* Ocurrencias del mismo caracter */
                kGolomb = 3;
                determinarContexto(&a, &b, &c, &d);

                do {
                    l++;
                    actualizarExtracto(fExtracto, errorPrediccion);
                    ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal);
                    x = (unsigned char) ultimoCaracterLeido;
                } while (ultimoCaracterLeido != EOF && !esFinDeLinea() && x == a);
                largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, l, &golombBinario);
                largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, l);
            }
            imprimirCompresion(golombBinario,largoGolombBinario,largoGolombUnario,archivoComprimido);
            
            //ACTUALIZACION DATOS
            actualizarDatosCompresion(&dtComp,largoGolombBinario + largoGolombUnario);
        }
    }
    else {
        while ((ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal)) != EOF) {
            x = (unsigned char) ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            xPrediccion = predecirX(a, b, c);
            fExtracto = determinarExtracto(xPrediccion,a , b, c);
            errorPrediccion = x - xPrediccion;
            kGolomb = determinarGolombK(fExtracto);
            mapeoRice = determinarMapeoRice(errorPrediccion);
            largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, mapeoRice, &golombBinario);
            largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, mapeoRice);
            imprimirCompresion(golombBinario,largoGolombBinario,largoGolombUnario,archivoComprimido);
            actualizarExtracto(fExtracto, errorPrediccion);
            
            //ACTUALIZACION DATOS
            actualizarDatosCompresion(&dtComp,largoGolombBinario + largoGolombUnario);
        }
    }
    
    guardarDatos(&dtComp,archivoOriginal);
    liberarExtractos();
    vaciarBuffer(archivoComprimido);
    destruirBuffer();
    fclose(archivoComprimido);
    fclose(archivoOriginal);
    
    return dtComp;
}

