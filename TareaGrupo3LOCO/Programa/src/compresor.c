#include "../include/compresor.h"
#include "../include/compresorIO.h"
#include "../include/pixelio.h"
#include "../include/datosCompresion.h"

#include <stdio.h>

/* Rutinas auxiliares */

void escribirParametrosCabezal( FILE * archivoComprimido, int s, bool run ) {
    fprintf(archivoComprimido,"%d\n",s);
    fprintf(archivoComprimido,"%d\n",run);
}

/*
 * Retorna el mapeo M(e)
 */
int determinarMapeoRice( int errorPrediccion ) {
    // Map de los errores de predicción al rango no negativo
    int M;

    if (errorPrediccion < 0) {
      M = -(errorPrediccion << 1) + 1;
    } else {
      M = errorPrediccion << 1;
    }

    return M;
}

unsigned int determinarLargoBinaryGolomb( int k, int M, int * bin_arg ) {
    // Devuelve el largo de la parte binaria de Golomb_k(M)
    // Ademas retorna bin_arg con la parte binaria.

    unsigned int bin_length;

    *bin_arg = M & ((1<<k)-1);    /* Binary_k(M)       */
    bin_length = k;               /* Largo Binary_k(M) */

    return bin_length;
}

unsigned int determinarLargoUnaryGolomb( int k, int M ) {
    // Devuelve el código de Golomb como un entero sin signo
    unsigned int un_length;

    un_length = (M>>k) + 1;    /* Largo Unary_k(M) */

    return un_length;
}

void comprimirNormal( int s, PIX x, Imagen img, Extractos extractos, BufferCompresion bufCompresion, DatosCompresion datosCompresion, FILE * archivoComprimido) {
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
    actualizarDatosCompresion(datosCompresion,largoGolombBinario + largoGolombUnario);
}

void comprimirRun(int l, PIX x, BufferCompresion bufCompresion, DatosCompresion datosCompresion, FILE * archivoComprimido) {
    int kGolomb = 3;
    int golombBinario;
    int largoGolombBinario, largoGolombUnario;

    largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, l, &golombBinario);
    largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, l);
    imprimirCompresion(bufCompresion,golombBinario,largoGolombBinario,largoGolombUnario,archivoComprimido);
    actualizarDatosCompresion(datosCompresion,largoGolombBinario + largoGolombUnario);
}

DatosCompresion comprimir( char* archivoEntrada, char* archivoSalida, int s, bool run ) {
    FILE * archivoComprimido, * archivoOriginal;
    int ancho, altura;
    int ultimoCaracterLeido;        /* Promoción temporal de x a entero        */
    PIX x;
    PIX a,b,c,d;          /* Contexto                                */
    BufferCompresion bufCompresion;
    Extractos extractos;
    DatosCabezal datosCabezal;
    Imagen img;
    DatosCompresion datosCompresion;

    archivoComprimido = fopen(archivoSalida, "wb");
    archivoOriginal = fopen(archivoEntrada, "rb");

    bufCompresion = crearBufferCompresion();
    extractos = crearExtractos(s);
    escribirParametrosCabezal(archivoComprimido, s, run);
    datosCabezal = escribirCabezalPGM(archivoOriginal, archivoComprimido);
    img = crearImagen(datosCabezal);
    altura = obtenerAltura(img);
    ancho = obtenerAncho(img);
    datosCompresion = crearDatosCompresion(altura*ancho);
    
    int fila,col;
    if (run) {
        avanzarPixel(img);
        for(fila = 1; fila <= altura; fila++) {
            for(col = 1; col <= ancho; col++) {
                ultimoCaracterLeido = obtenerUltimoCaracter(img, archivoOriginal);
                x = (PIX) ultimoCaracterLeido;
                determinarContexto(img, &a, &b, &c, &d);

                if (a!=b || b!=c || c!=d) {
                    comprimirNormal(s,x,img,extractos,bufCompresion,datosCompresion,archivoComprimido);
                    avanzarPixel(img);
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
                    comprimirRun(l,x,bufCompresion,datosCompresion,archivoComprimido);
                    comprimirNormal(s,x,img,extractos,bufCompresion,datosCompresion,archivoComprimido);
                    avanzarPixel(img);
                    col += l;
                }
            }
        }
    }
    else {
        for(fila = 0; fila < altura; fila++) {
            for(col = 0; col < ancho; col++) {
                avanzarPixel(img);
                ultimoCaracterLeido = obtenerUltimoCaracter(img, archivoOriginal);
                x = (PIX) ultimoCaracterLeido;
                comprimirNormal(s,x,img,extractos,bufCompresion,datosCompresion,archivoComprimido);
            }
        }
    }
    vaciarBuffer(bufCompresion,archivoComprimido);
    guardarDatos(datosCompresion);
    fclose(archivoComprimido);
    fclose(archivoOriginal);
    destruirExtractos(extractos);
    destruirImagen(img);
    destruirDatosCabezal(datosCabezal);
    destruirBufferCompresion(bufCompresion);
    
    return datosCompresion;
}