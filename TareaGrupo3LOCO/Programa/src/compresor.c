#include "../include/compresor.h"
#include "../include/compresorIO.h"
#include "../include/pixelio.h"

#include <stdio.h>

void comprimir( char* archivoEntrada, char* archivoSalida, int s, Modalidad modalidad ) {
    FILE * archivoComprimido;
    FILE * archivoDescomprimido;
    int anchoImagen;
    int kGolomb;
    int ultimoCaracterLeido;        /* Promoción temporal de x a entero        */
    unsigned char x;
    unsigned char xPrediccion;      /* x^                                      */
    int errorPrediccion;            /* e = x - x^                              */
    Extracto * fExtracto;           /* f(C)                                    */
    unsigned char a,b,c,d;          /* Contexto                                */
    int mapeoRice;
    int cantidadBitsImpresos;       /* Cantidad de bits impresos por iteración */
    int output;                     /* Salida al archivo descomprimido         */
    
    archivoComprimido = fopen(archivoSalida, "wb");
    archivoDescomprimido = fopen(archivoEntrada, "rb");

    escribirCabezal(archivoComprimido, s, modalidad);
    inicializarExtractos();
    anchoImagen = determinarAnchoImagen(archivoDescomprimido);
    inicializarBuffer(anchoImagen);
    if (modalidad == RUN) {
        while ((ultimoCaracterLeido = obtenerUltimoCaracter()) != EOF) {
            x = (unsigned char) ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            xPrediccion = predecirX(a, b, c);
            fExtracto = determinarExtracto(xPrediccion, a, b, c, s);
            errorPrediccion = x - xPrediccion;
            if (a!=b || b!=c || c!=d) {    /* No es modo de run */
                kGolomb = determinarGolombK(fExtracto);
                mapeoRice = determinarMapeoRice(errorPrediccion,fExtracto);
                determinarGolomb(kGolomb, mapeoRice, &cantidadBitsImpresos, &output);
                actualizarExtracto(fExtracto,errorPrediccion);
            }
            else {
                int l = -1;    /* Ocurrencias del mismo caracter */
                unsigned char xAnterior;
                
                kGolomb = 3;
                determinarContexto(&a, &b, &c, &d);
                xPrediccion = predecirX(a, b, c);
                fExtracto = determinarExtracto(xPrediccion, a, b, c, s);
                do {
                    l++;
                    errorPrediccion = x - xPrediccion;
                    actualizarExtracto(fExtracto, errorPrediccion);
                    ultimoCaracterLeido = obtenerUltimoCaracter();
                    xAnterior = x;
                    x = (unsigned char) ultimoCaracterLeido;
                } while (ultimoCaracterLeido != EOF && x == xAnterior);
                if (ultimoCaracterLeido == EOF) {
                    determinarGolomb(kGolomb, l, &cantidadBitsImpresos, &output);
                    break;   /* Fin de archivo */
                }
            }
            actualizarBuffer(output, cantidadBitsImpresos, archivoComprimido);
        }
    }
    else {
        while ((ultimoCaracterLeido = obtenerUltimoCaracter()) != EOF) {
            x = (unsigned char)ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            xPrediccion = predecirX(a, b, c);
            fExtracto = determinarExtracto(xPrediccion,a , b, c, s);
            errorPrediccion = x - xPrediccion;
            kGolomb = determinarGolombK(fExtracto);
            mapeoRice = determinarMapeoRice(errorPrediccion, fExtracto);
            determinarGolomb(kGolomb, mapeoRice, &cantidadBitsImpresos, &output);
            actualizarExtracto(fExtracto, errorPrediccion);
            actualizarBuffer(output, cantidadBitsImpresos, archivoComprimido);
        }
    }
    liberarExtractos();
    vaciarBuffer(archivoComprimido);
    destruirBuffer();
    fclose(archivoComprimido);
    fclose(archivoDescomprimido);
}
