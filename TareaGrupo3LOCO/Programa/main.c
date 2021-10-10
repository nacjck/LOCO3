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
 * Para correr el archivo 'main.c' desde la consola usar:
 * ./main -c/d -s valorS inputFile outputFile
 * 
 * Ejemplo: ./main -k -c 8 "Archivo" "ArchivoComprimido"
 *
 * Donde:
 * -c habilita la compresión y -d la descompresión
 * -s <number>: parámetro s del sesgo
 * inputFile: archivo de entrada
 * outputFile: archivo de salida
 */

#include "include/sistema.h"
#include "include/modalidad.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef enum { Comprimir, Descomprimir } funcionalidad_t;

typedef struct {
    char         *archivoEntrada;
    char         *archivoSalida;
    uint8_t      s;                 /* 0 <= s <= 10 */
    modalidad_t  modalidad;
} parametros_t;


/*
 * Declaraciones para rutinas locales
 */
void comprimir( char* archivoEntrada, char* archivoSaluda, 
    uint8_t s, modalidad_t modalidad );
void descomprimir( char* archivoEntrada, char* archivoSaluda );
void abortar( char* mensaje );

int main( int argc, char* argv[] ) {
    /*
     * Por defecto los parámetros del programa son los siguientes:
     *     (funcionalidad,s,modalidad) = (Comprimir,0,Normal)
     * En caso de no presentarse nombres de archivos ya sean
     * de entrada como de salida se devolverá error.
     */
    funcionalidad_t funcionalidad = Comprimir;
    parametros_t parametros = {NULL,NULL,0,Normal};
    
    int i = 1;
    while( i < argc ) {
        if ( argv[i][0] == '-' ) {
            switch ( argv[i][1] ) {
                case 's':
                    i++;
                    if ( sscanf(argv[i], "%d", &parametros.s) != 1 ) {
                        abortar("Valor de 's' invalido.");
                    }
                    i++;
                    break;
                case 'c':
                    funcionalidad = Comprimir;
                    i++;
                    break;
                case 'd':
                    funcionalidad = Descomprimir;
                    i++;
                    break;
                default:
                    abortar("Flag invalida");
                    break;
            }
        } else {
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

        /*
         * Control de precondiciones
         */
        if( parametros.archivoEntrada == NULL ) {
            abortar("Ubicacion de archivo de entrada no especificada.");
        }
        if( parametros.archivoSalida == NULL ) {
            abortar("Ubicacion de archivo de salida no especificada.");
        }
        if( parametros.s < 0 || parametros.s > 10 ) {
            abortar("el parametro 's' debe estar entre 0 y 10'");
        }

    }
    
    if( funcionalidad == Comprimir )
    {
        comprimir(parametros.archivoEntrada,parametros.archivoSalida,
            parametros.modalidad,parametros.s);
    }
    else
    {
        descomprimir(parametros.archivoEntrada,parametros.archivoSalida);
    }
    return EXIT_SUCCESS;
}

/*
 * Rutina para errores
*/
void abortar( char *mensaje )
{
    puts( "Error: " );
    puts( mensaje );
    puts( "\n ");
    exit( EXIT_FAILURE );
}

