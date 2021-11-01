#include "../include/compresor.h"
#include "../include/compartido.h"
#include "../include/pixelio.h"
#include "../include/modalidad.h"

#include <stdio.h>

// Ayuda-memoria para saber donde está cada cosa
void leerCabezal(FILE * compressedFile, int *s,Modalidad * modalidad, int * anchoImagen); //FUNCION AUXILIAR DESCOMPRESOR
void inicializarExtractos(); //COMPARTIDO
void inicializarBuffer(); //PIXELIO
char obtenerUltimoCaracter(); //PIXELIO
void determinarContexto(unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d); //PIXELIO
unsigned char predecirX(unsigned char a, unsigned char b, unsigned char c); //COMPARTIDO
Extracto * determinarExtracto(unsigned char x, unsigned char a, unsigned char b, unsigned char c); //COMPARTIDO
int determinarParametroGolombK(Extracto * extracto); //COMPARTIDO
int determinarMapeoRice(int errorPrediccion, Extracto * fExtracto); //COMPARTIDO
int determinarGolomb(int k, int elemento, int * cantidadBitsImpresos, int * output); //COMPARTIDO
void actualizarExtracto(Extracto * fExtracto, int errorPrediccion); //COMPARTIDO
void actualizarBuffer(int output, int cantidadBits, FILE * decompressedFile);
void destruirBuffer(); //PIXELIO

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
    
    archivoComprimido = fopen(archivoEntrada, "rb");
    archivoDescomprimido = fopen(archivoSalida, "wb");

    escribirCabezal(archivoComprimido, s, modalidad);
    inicializarExtractos();
    anchoImagen = determinarAnchoImagen(archivoDescomprimido);
    inicializarBuffer(anchoImagen);
    if (modalidad == RUN) {
        while (ultimoCaracterLeido = obtenerUltimoCaracter() != EOF) {
            x = (unsigned char) ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            xPrediccion = predecirX(a, b, c);
            fExtracto = determinarExtracto(xPrediccion, a, b, c);
            errorPrediccion = x - xPrediccion;
            if (a != b || b!=c || c!=d) {    /* No es modo de run */
                kGolomb = determinarParametroGolombK(fExtracto);
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
                fExtracto = determinarExtracto(xPrediccion, a, b, c);
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
        while (ultimoCaracterLeido = obtenerUltimoCaracter() != EOF) {
            x = (unsigned char)ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            xPrediccion = predecirX(a, b, c);
            fExtracto = determinarExtracto(xPrediccion,a , b, c);
            errorPrediccion = x - xPrediccion;
            kGolomb = determinarParametroGolombK(fExtracto);
            mapeoRice = determinarMapeoRice(errorPrediccion, fExtracto);
            determinarGolomb(kGolomb, mapeoRice, &cantidadBitsImpresos, &output);
            actualizarExtracto(fExtracto, errorPrediccion);
            actualizarBuffer(output, cantidadBitsImpresos, archivoComprimido);
        }
    }
    vaciarBuffer(archivoComprimido);
    destruirBuffer();
    fclose(archivoComprimido);
    fclose(archivoDescomprimido);
}