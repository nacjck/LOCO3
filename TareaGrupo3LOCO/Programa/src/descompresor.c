#include "../include/descompresor.h"
#include "../include/compartido.h"
#include "../include/modalidad.h"
#include "../include/pixelio.h"

#include <stdio.h>

// Ayuda-memoria para saber donde está cada cosa
void leerCabezal(FILE * compressedFile, int *s,Modalidad * modalidad); //FUNCION AUXILIAR DESCOMPRESOR
void inicializarExtractos(); //COMPARTIDO
void inicializarBuffer(); //PIXELIO
unsigned char obtenerUltimoCaracter(); //PIXELIO
void determinarContexto(unsigned char * a, unsigned char * b, unsigned char * c, unsigned char * d); //PIXELIO
unsigned char predecirX(unsigned char a, unsigned char b, unsigned char c); //COMPARTIDO
Extracto * determinarExtracto(unsigned char x, unsigned char a, unsigned char b, unsigned char c); //PIXELIO
int determinarParametroGolombK(Extracto * extracto); //COMPARTIDO
int determinarMapeoRice(int errorPrediccion, Extracto * fExtracto); //COMPARTIDO
int determinarGolomb(int k, int mapeoRice); //COMPARTIDO
void actualizarExtracto(Extracto * fExtracto, int errorPrediccion); //COMPARTIDO
void actualizarBuffer(int output, FILE * decompressedFile); //PIXELIO

void descomprimir( char* archivoEntrada, char* archivoSalida ) {
    FILE * compressedFile;
    FILE * decompressedFile;
    int s;
    Modalidad modalidad; 
    int k;
    int ultimoCaracterLeido;    /* Promoción temporal de x a entero */
    unsigned char x;
    unsigned char xPrediccion;  /* x^                               */
    int errorPrediccion;        /* e = x - x^                       */
    Extracto * fExtracto;       /* f(C)                             */
    unsigned char a,b,c,d;      /* Contexto                         */
    int mapeoRice;
    int output;                 /* Salida al archivo descomprimido  */
    
    compressedFile = fopen(archivoEntrada,"rb");
    decompressedFile = fopen(archivoSalida,"wb");

    leerCabezal(compressedFile,&s,&modalidad);
    inicializarExtractos();
    inicializarBuffer();
    if(modalidad == RUN) {
        while(ultimoCaracterLeido = obtenerUltimoCaracter() != EOF) {
            x = (unsigned char)ultimoCaracterLeido;
            determinarContexto(&a,&b,&c,&d);
            xPrediccion = predecirX(a,b,c);
            fExtracto = determinarExtracto(xPrediccion,a,b,c);
            errorPrediccion = x - xPrediccion;
            if(a != b || b!=c || c!=d) {    /* No es modo de run */
                k = determinarParametroGolombK(fExtracto);
                mapeoRice = determinarMapeoRice(errorPrediccion,fExtracto);
                output = determinarGolomb(k,mapeoRice);
                actualizarExtracto(fExtracto,errorPrediccion);
            }
            else {
                int l = -1;    /* Ocurrencias del mismo caracter */
                unsigned char xAnterior;
                
                k = 3;
                determinarContexto(&a,&b,&c,&d);
                xPrediccion = predecirX(a,b,c);
                fExtracto = determinarExtracto(xPrediccion,a,b,c);
                do {
                    l++;
                    errorPrediccion = x - xPrediccion;
                    actualizarExtracto(fExtracto,errorPrediccion);
                    ultimoCaracterLeido = obtenerUltimoCaracter();
                    xAnterior = x;
                    x = (unsigned char)ultimoCaracterLeido;
                } while(ultimoCaracterLeido != EOF && x == xAnterior);
                if(x != xAnterior) break; //Fin de archivo
                output = determinarGolomb(k,l);
            }
            actualizarBuffer(output,decompressedFile);
        }
    }
    else {
        while(ultimoCaracterLeido = obtenerUltimoCaracter() != EOF) {
            x = (unsigned char)ultimoCaracterLeido;
            determinarContexto(&a,&b,&c,&d);
            xPrediccion = predecirX(a,b,c);
            fExtracto = determinarExtracto(xPrediccion,a,b,c);
            errorPrediccion = x - xPrediccion;
            k = determinarParametroGolombK(fExtracto);
            mapeoRice = determinarMapeoRice(errorPrediccion,fExtracto);
            output = determinarGolomb(k,mapeoRice);
            actualizarExtracto(fExtracto,errorPrediccion);
            imprimirSecuencia(output,decompressedFile);
        }
    }
}