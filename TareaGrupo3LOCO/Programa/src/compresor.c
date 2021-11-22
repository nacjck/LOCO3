#include "../include/compresor.h"
#include "../include/compresorIO.h"
#include "../include/pixelio.h"

#include <stdio.h>

static const size_t BITS_INT = (sizeof(int) << 3);

void imprimirCompresion( int golombBinario, int largoGolombBinario, int largoGolombUnario, FILE * archivoComprimido) {
    actualizarBuffer(golombBinario, largoGolombBinario, archivoComprimido);
    while(largoGolombUnario > BITS_INT) {
        actualizarBuffer(0, BITS_INT, archivoComprimido);
        largoGolombUnario -= BITS_INT;
    }
    actualizarBuffer(1 << (largoGolombUnario - 1), largoGolombUnario, archivoComprimido);
}

void comprimir( char* archivoEntrada, char* archivoSalida, int s, Modalidad modalidad ) {
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
    
    escribirCabezal(archivoComprimido, s, modalidad);
    inicializarExtractos(s);
    anchoImagen = determinarAnchoImagen(archivoOriginal);
    inicializarBuffer(anchoImagen);
    inicializarBufferCompresion();
    if (modalidad == RUN) {
        while ((ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal)) != EOF) {
            x = (unsigned char) ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            xPrediccion = predecirX(a, b, c);
            fExtracto = determinarExtracto(xPrediccion, a, b, c);
            errorPrediccion = x - xPrediccion;
            if (a!=b || b!=c || c!=d) {    /* No es modo de run */
                kGolomb = determinarGolombK(fExtracto);
                mapeoRice = determinarMapeoRice(errorPrediccion);
                //Fuera de servicio
                //determinarGolomb(kGolomb, mapeoRice, &cantidadBitsImpresos, &output);
                actualizarExtracto(fExtracto,errorPrediccion);
            }
            else {
                int l = -1;    /* Ocurrencias del mismo caracter */
                unsigned char xAnterior;
                
                kGolomb = 3;
                determinarContexto(&a, &b, &c, &d);
                xPrediccion = predecirX(a, b, c);
                fExtracto = determinarExtracto(xPrediccion, a, b, c);
                do {
                    l++;
                    errorPrediccion = x - xPrediccion;
                    actualizarExtracto(fExtracto, errorPrediccion);
                    ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal);
                    xAnterior = x;
                    x = (unsigned char) ultimoCaracterLeido;
                } while (ultimoCaracterLeido != EOF && x == xAnterior);
                //Fuera de servicio
                //determinarGolomb(kGolomb, l, &cantidadBitsImpresos, &output);
                if (ultimoCaracterLeido == EOF) {
                    break;   /* Fin de archivo */
                }
            }
            //Fuera de servicio
            //actualizarBuffer(output, cantidadBitsImpresos, archivoComprimido);
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
        }
    }
    liberarExtractos();
    vaciarBuffer(archivoComprimido);
    destruirBuffer();
    fclose(archivoComprimido);
    fclose(archivoOriginal);
}

