#include "../include/compresor.h"
#include "../include/bufferCompresion.h"
#include "../include/imagen.h"
#include "../include/datosCompresion.h"

#include <stdio.h>

/* AUXILIAR
 * Guarda en el archivo comprimido la información que requiere el descompresor
 */
void escribirParametrosCabezal( FILE * archivoComprimido, int s, bool run ) {
    fprintf(archivoComprimido,"%d\n",s);
    fprintf(archivoComprimido,"%d\n",run);
}

/* AUXILIAR
 * Retorna el mapeo M(e)
 */
int determinarMapeoRice( int errorPrediccion ) {
    /* Map de los errores de predicción al rango no negativo */
    int M;

    if (errorPrediccion < 0) {
      M = -(errorPrediccion << 1) + 1;
    } else {
      M = errorPrediccion << 1;
    }

    return M;
}

/* AUXILIAR
 * Devuelve el largo de la parte binaria de Golomb_k(M).
 * Además retorna bin_arg con la parte binaria.       
 */
unsigned int determinarLargoBinaryGolomb( int k, int M, int * bin_arg ) {

    unsigned int bin_length;

    *bin_arg = M & ((1<<k)-1);    /* Binary_k(M)       */
    bin_length = k;               /* Largo Binary_k(M) */

    return bin_length;
}

/* AUXILIAR
 * Devuelve el código de Golomb como un entero sin signo
 */
unsigned int determinarLargoUnaryGolomb( int k, int M ) {
    unsigned int un_length;

    un_length = (M>>k) + 1;    /* Largo Unary_k(M) */

    return un_length;
}

/* AUXILIAR
 * Comprime sin modo de run
 */
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

/* AUXILIAR
 * Comprime en modo de run
 */
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
    FILE * archivoComprimido,
         * archivoOriginal;
    int ancho, altura;               /* Dimensiones de imagen            */
    int ultimoCaracterLeido;         /* Promoción temporal de x a entero */
    PIX x;                           /* Valor del pixel actual           */
    PIX a,b,c,d;                     /* Contexto                         */
    BufferCompresion bufCompresion;
    Extractos extractos;
    DatosCabezal datosCabezal;
    Imagen img;
    DatosCompresion datosCompresion; /* Información final de compresión    */

    // Inicialización de estructuras
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
    
    //Comienzo de algoritmo
    int fila,col;
    if (run) {
        avanzarPixel(img);
        for(fila = 1; fila <= altura; fila++) {
            for(col = 1; col <= ancho; col++) {
                ultimoCaracterLeido = obtenerUltimoCaracter(img, archivoOriginal);
                x = (PIX) ultimoCaracterLeido;  /* Promoción a unsigned char */
                determinarContexto(img, &a, &b, &c, &d);

                if (a!=b || b!=c || c!=d) {
                    comprimirNormal(s,x,img,extractos,bufCompresion,datosCompresion,archivoComprimido);
                    avanzarPixel(img);
                }
                else {
                    int l = 0;  /* Contador de ocurrencias repetidas */
                    while (x == a && !esFinDeLinea(img)) {  /* Condiciones de modo de run */
                        l++;
                        avanzarPixel(img);
                        ultimoCaracterLeido = obtenerUltimoCaracter(img, archivoOriginal);
                        x = (PIX) ultimoCaracterLeido;  /* Promoción a unsigned char */
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
                x = (PIX) ultimoCaracterLeido;  /* Promoción a unsigned char */
                comprimirNormal(s,x,img,extractos,bufCompresion,datosCompresion,archivoComprimido);
            }
        }
    }

    //Se libera memoria
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