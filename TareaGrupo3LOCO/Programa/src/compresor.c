#include "../include/compresor.h"
#include "../include/compresorIO.h"
#include "../include/pixelio.h"

#include <stdio.h>

void comprimir( char* archivoEntrada, char* archivoSalida, int s, Modalidad modalidad ) {
    FILE * archivoComprimido, * archivoOriginal;
    int anchoImagen;
    int kGolomb;
    int ultimoCaracterLeido;        /* Promoción temporal de x a entero        */
    unsigned char x, xPrediccion;
    int errorPrediccion;            /* e = x - x^                              */
    int fC;
    Extracto fExtracto;             /* f(C)                                    */
    unsigned char a,b,c,d;          /* Contexto                                */
    int mapeoRice;
    unsigned int golombBinario;
    int largoGolombBinario;       /* Cantidad de bits de binary(M) */
    int largoGolombUnario;        /* Cantidad de bits de unary (M) */

    BufferCompresion bufCompresion = crearBufferCompresion();
    Extractos extractos = crearExtractos(s);

    archivoComprimido = fopen(archivoSalida, "wb");
    archivoOriginal = fopen(archivoEntrada, "rb");

    escribirParametrosCabezal(archivoComprimido, s, modalidad);
    escribirCabezalPGM(archivoOriginal, archivoComprimido, &anchoImagen);
    inicializarBuffer(anchoImagen);
    if (modalidad == RUN) {
        while (( ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal)) != EOF) {
            x = (unsigned char) ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            
            if (a!=b || b!=c || c!=d) {    /* No es modo de run */
                xPrediccion = predecirX(a, b, c);
                fC = determinarIndiceExtracto(xPrediccion, a, b, c, s);
                fExtracto = determinarExtracto(extractos, fC);
                errorPrediccion = x - xPrediccion;
                kGolomb = determinarGolombK(fExtracto);
                mapeoRice = determinarMapeoRice(errorPrediccion);
                largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, mapeoRice, &golombBinario);
                largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, mapeoRice);
            }
            else {
                int l = 0;    /* Ocurrencias del mismo caracter */
                kGolomb = 3;

                //Contador de ocurrencias repetidas
                while (ultimoCaracterLeido != EOF && !esFinDeLinea() && x == a) {
                    l++;
                    ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal);
                    x = (unsigned char) ultimoCaracterLeido;
                }
                
                //Impresion de largo de run
                largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, l, &golombBinario);
                largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, l);
                imprimirCompresion(bufCompresion,golombBinario,largoGolombBinario,largoGolombUnario,archivoComprimido);
                
                //Impresion de proximo pixel al de run (provisorio)
                determinarContexto(&a, &b, &c, &d);
                xPrediccion = predecirX(a, b, c);
                fC = determinarIndiceExtracto(xPrediccion, a, b, c, s);
                fExtracto = determinarExtracto(extractos, fC);
                errorPrediccion = x - xPrediccion;
                kGolomb = determinarGolombK(fExtracto);
                mapeoRice = determinarMapeoRice(errorPrediccion);
                largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, mapeoRice, &golombBinario);
                largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, mapeoRice);
                
            }
            imprimirCompresion(bufCompresion,golombBinario,largoGolombBinario,largoGolombUnario,archivoComprimido);
            actualizarExtracto(fExtracto,errorPrediccion);
        }
    }
    else {
        while ((ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal)) != EOF) {
            x = (unsigned char) ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            xPrediccion = predecirX(a, b, c);
            fC = determinarIndiceExtracto(xPrediccion, a, b, c, s);
            fExtracto = determinarExtracto(extractos, fC);
            errorPrediccion = x - xPrediccion;
            kGolomb = determinarGolombK(fExtracto);
            mapeoRice = determinarMapeoRice(errorPrediccion);
            largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, mapeoRice, &golombBinario);
            largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, mapeoRice);
            imprimirCompresion(bufCompresion,golombBinario,largoGolombBinario,largoGolombUnario,archivoComprimido);
            actualizarExtracto(fExtracto, errorPrediccion);
        }
    }
    vaciarBuffer(bufCompresion,archivoComprimido);
    fclose(archivoComprimido);
    fclose(archivoOriginal);
    destruirExtractos(extractos);
    destruirBuffer();
    destruirBufferCompresion(bufCompresion);
}
