/* 
 * TAREA LOCO ATIPI - NTI/FING/UDELAR
 * 
 * Autores:
 * 	Alexis Baladón
 * 	Juan Ignacio Fernandez
 */

/* 
 * main.c
 *
 * Para correr el archivo 'main.c' desde la consola, usar:
 * ./main -d/[c modalidad] -r/n -[s valorS] inputFile outputFile
 * 
 * Ejemplos:
 *   ./main -c 1 -s 10 "imagen.pgm" "imagen.bin"
 *   ./main -d -s 10 "imagen.bin" "imagen.pgm"
 *
 * Donde:
 * -c <number> habilita la compresión,
 * si recibe 1 comprime en modo de run,
 * si recibe un 0 lo hace en modo normal.
 * -d habilita la compresión
 * -s <number>: parámetro s del sesgo
 * -n habilita la compr
 * inputFile: archivo de entrada
 * outputFile: archivo de salida
 */

#include "include/descompresor.h"
#include "include/modalidad.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char            *archivoEntrada;
    char            *archivoSalida;
} Parametros;

/*
 * Declaraciones para rutinas locales
 */
void abortar( char* mensaje );

int main( int argc, char* argv[] ) {
    /*
     * Por defecto los parámetros del programa son los siguientes:
     *     (funcionalidad,s,modalidad) = (Comprimir,0,Normal)
     * En caso de no presentarse nombres de archivos ya sean
     * de entrada como de salida se devolverá error.
     */
    Parametros parametros = {NULL,NULL};
    
    int i = 1;
    while( i < argc ) {
        if ( argv[i][0] == '-' ) {
            if( parametros.archivoEntrada == NULL ) {
                parametros.archivoEntrada = argv[i];
                i++;
            } else if ( parametros.archivoSalida == NULL ) {
                parametros.archivoSalida = argv[i];
                i++;
            } else {
                abortar("Demasiados parametros.");
            }
        }
    }
    
    /*
     * Control de precondiciones
     */
    if( parametros.archivoEntrada == NULL ) {
        abortar("Ubicacion de archivo de entrada no especificada.");
    }
    else {
        FILE * f = NULL;
        if ((f = fopen(parametros.archivoEntrada,"rb")) == NULL) {
            abortar("problema al abrir archivo de entrada.");
        }
        fclose(f);
    }
    if (parametros.archivoSalida == NULL) {
        abortar("Ubicacion de archivo de salida no especificada.");
    }
    
    /*
     * Inicio de descompresión
     */
    puts("\n Iniciando programa...");
    descomprimir(parametros.archivoEntrada,parametros.archivoSalida);
    
    return EXIT_SUCCESS;
}

/*
 * Rutina para errores
*/
void abortar( char *mensaje ) {
    puts("Error: ");
    puts(mensaje);
    puts("\n ");
    exit(EXIT_FAILURE);
}

