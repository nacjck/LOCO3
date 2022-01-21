/*
 * imagen.h
 *
 * Aquí se encuentran funciones encargadas de la entrada y salida
 * de pixeles
 *
 */

#ifndef imagen_h
#define imagen_h

#include <stdio.h>
#include <stdlib.h>

#ifndef BYTE
  #define BYTE unsigned char
#endif

#ifndef PIX
  #define PIX unsigned char
#endif

#ifndef bool
  #define bool int
#endif

/*
 * Datos de la imagen a comprimir y pixeles necesarios
 * para llevar a cabo la compresión/descompresión
 */
typedef struct _datosCabezal * DatosCabezal;
typedef struct _imagen * Imagen;

/*
 * Reserva memoria para la imagen
 */
Imagen crearImagen( DatosCabezal dtCabezal );

/*
 * Escribe el cabezal PGM del archivo de entrada en el de salida 
 */
DatosCabezal escribirCabezalPGM( FILE * archivoInput, FILE * archivoOutput );

/*
 * Getters de la estructura
 */
int obtenerAncho( Imagen img );
int obtenerAltura( Imagen img );
int obtenerMaxValue( Imagen img );

/*
 * Almacena en (a,b,c,d) los valores correspondientes al contexto
 * del pixel actualmente recorrido de la imagen
 */
void determinarContexto( Imagen img, PIX * a, PIX * b, PIX * c, PIX * d );

/*
 * Determina si el pixel actual es el último de la fila
 */
bool esFinDeLinea( Imagen img );

/*
 * Libera la memoria de la imagen y los datos del cabezal
 */
void destruirImagen( Imagen img );
void destruirDatosCabezal( DatosCabezal );

/*
 * Se avanza al siguiente pixel de la imagen
 */
void avanzarPixel( Imagen img );

/* COMPRESOR
 *
 * Obtiene el siguiente caracter de la imagen. Si es necesario
 * se obtendrá el mismo del archivo indicado.
 */
int obtenerUltimoCaracter( Imagen img, FILE * archivoComprimido );

/* DESCOMPRESOR
 *
 * Se agrega el caracter ingresado al buffer de la imagen
 */
void agregarCaracter( Imagen img, PIX c );

#endif
