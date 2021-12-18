#include "../include/compresor.h"
#include "../include/compresorIO.h"
#include "../include/pixelio.h"

#include <stdio.h>

/* Rutinas auxiliares */
void escribirParametrosCabezal( FILE * archivoComprimido, int s, bool run );
void comprimirNormal( int s, PIX x, Imagen img, Extractos extractos, BufferCompresion bufCompresion, FILE * archivoComprimido);
void comprimirRun(int l, BYTE x, Imagen img, BufferCompresion bufCompresion, FILE * archivoComprimido);

void comprimir( char* archivoEntrada, char* archivoSalida, int s, bool run ) {
    FILE * archivoComprimido, * archivoOriginal;
    int anchoImagen, alturaImagen;
    int ultimoCaracterLeido;        /* Promoción temporal de x a entero        */
    PIX x;
    PIX a,b,c,d;          /* Contexto                                */
    BufferCompresion bufCompresion;
    Extractos extractos;
    DatosCabezal dtCabezal;
    Imagen img;

    archivoComprimido = fopen(archivoSalida, "wb");
    archivoOriginal = fopen(archivoEntrada, "rb");

    bufCompresion = crearBufferCompresion();
    extractos = crearExtractos(s);
    escribirParametrosCabezal(archivoComprimido, s, run);
    dtCabezal = escribirCabezalPGM(archivoOriginal, archivoComprimido);
    img = crearImagen(dtCabezal);
    alturaImagen = obtenerAltura(img);
    anchoImagen = obtenerAncho(img);
    
    int fila,col;
    if (run) {
        for(fila = 0; fila < alturaImagen; fila++) {
            for(col = 0; col < anchoImagen; col++) {

                ultimoCaracterLeido = obtenerUltimoCaracter(img, archivoOriginal);
                x = (PIX) ultimoCaracterLeido;
                determinarContexto(img, &a, &b, &c, &d);

                if (a!=b || b!=c || c!=d) {
                    comprimirNormal(s,x,img,extractos,bufCompresion,archivoComprimido);
                }
                else {
                    int l = 0;
                    /* Contador de ocurrencias repetidas */
                    while (x == a && !esFinDeLinea(img)) {
                        l++;
                        avanzarPixel(img);
                        ultimoCaracterLeido = obtenerUltimoCaracter(img, archivoOriginal);
                        x = (PIX) ultimoCaracterLeido;
                    }
                    comprimirRun(l,x,img,bufCompresion,archivoComprimido);
                    comprimirNormal(s,x,img,extractos,bufCompresion,archivoComprimido);

                    col+=l-1;
                }
            }
        }
    }
    else {
        for(fila = 0; fila < alturaImagen; fila++) {
            for(col = 0; col < anchoImagen; col++) {
                ultimoCaracterLeido = obtenerUltimoCaracter(img, archivoOriginal);
                x = (PIX) ultimoCaracterLeido;
                comprimirNormal(s,x,img,extractos,bufCompresion,archivoComprimido);
            }
        }
    }
    vaciarBuffer(bufCompresion,archivoComprimido);
    fclose(archivoComprimido);
    fclose(archivoOriginal);
    destruirExtractos(extractos);
    destruirImagen(img);
    destruirDatosCabezal(dtCabezal);
    destruirBufferCompresion(bufCompresion);
}

/* Rutinas auxiliares */

void escribirParametrosCabezal( FILE * archivoComprimido, int s, bool run ) {
    fprintf(archivoComprimido,"%d\n",s);
    fprintf(archivoComprimido,"%d\n",run);
}

void comprimirNormal( int s, PIX x, Imagen img, Extractos extractos, BufferCompresion bufCompresion, FILE * archivoComprimido) {
    PIX a,b,c,d;
    PIX xPrediccion;
    int fC;
    Extracto fExtracto;
    int errorPrediccion;
    int mapeoRice;
    int kGolomb;
    int golombBinario;
    int largoGolombBinario, largoGolombUnario;

    determinarContexto(img, &a, &b, &c, &d);
    avanzarPixel(img);
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

void comprimirRun(int l, PIX x, Imagen img, BufferCompresion bufCompresion, FILE * archivoComprimido) {
    int kGolomb = 3;
    int golombBinario;
    int largoGolombBinario, largoGolombUnario;

    largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, l, &golombBinario);
    largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, l);
    imprimirCompresion(bufCompresion,golombBinario,largoGolombBinario,largoGolombUnario,archivoComprimido);
}