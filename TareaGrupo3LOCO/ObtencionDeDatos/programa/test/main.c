/* 
 * TAREA LOCO ATIPI - NTI/FING/UDELAR
 * 
 * Autores:
 * 	Alexis Baladón
 * 	Juan Ignacio Fernandez
 */

#include "../../../Programa/include/compresor.h"
#include "../../../Programa/include/datosCompresion.h"
#include "../../../Programa/include/descompresor.h"

#include <stdlib.h>
#include <stdio.h>

#ifndef bool
    #define bool char
#endif

#define DEFAULT_S 8
#define DEFAULT_MODE 1 //Modo de run activado

typedef enum { COMPRIMIR, DESCOMPRIMIR } Funcionalidad;

typedef struct {
    char            *archivoEntrada;
    char            *archivoSalida;
    int             s;                  /* 0 <= s <= 10                      */
    bool            run;
    bool            mute;               /* El programa imprime en la consola */
} Parametros;

// Declaraciones para rutinas locales
void abortar( char* mensaje );

int main( int argc, char* argv[] ) {
    /*
     * Por defecto los parámetros del programa son los siguientes:
     *     (funcionalidad,s,run,mute) = (Comprimir,DEFAULT_S,true,false)
     * En caso de no presentarse nombres de archivos ya sean
     * de entrada como de salida se devolverá error.
     */
    Funcionalidad funcionalidad = COMPRIMIR;
    Parametros parametros = {NULL,NULL,DEFAULT_S,DEFAULT_MODE,0};
    
    if (argc < 4) abortar("Numero de parametros insuficiente");

    int i = 1;
    while( i < argc ) {
        if ( argv[i][0] == '-' ) {
            switch ( argv[i][1] ) {
                case 'm':
                    parametros.mute = 1;
                    i++;
                    break;
                case 's':
                    i++;
                    if ( sscanf(argv[i], "%d", &(parametros.s)) != 1 ) {
                        abortar("Valor de 's' invalido.");
                    }
                    i++;
                    break;
                case 'c':
                    funcionalidad = COMPRIMIR;
                    i++;
                    int run;
                    if ( sscanf(argv[i], "%d", &(run)) != 1 ) {
                        parametros.run = DEFAULT_MODE;
                        i--;
                    }
                    else if (run != 0 && run != 1) {
                        abortar("Valor de modalidad inválido.");
                    }
                    else {
                        parametros.run = run;
                    }
                    i++;
                    break;
                case 'd':
                    funcionalidad = DESCOMPRIMIR;
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
    }
    
    // Control de precondiciones
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
    if (parametros.s < 0 || parametros.s > 10) {
        abortar("el parametro 's' debe estar entre 0 y 10.");
    }
    
    // Inicio de compresión/descompresión
    DatosCompresion datosCompresion;
    if (!parametros.mute) { puts("\nIniciando programa..."); }
    if (funcionalidad == COMPRIMIR) {
        datosCompresion = comprimir(parametros.archivoEntrada,
            parametros.archivoSalida,parametros.s,parametros.run);
    }
    else {
        datosCompresion = descomprimir(parametros.archivoEntrada,
            parametros.archivoSalida);
    }

    // Impresion de informacion en caso de no haberse silenciado
    if (!parametros.mute) {
        long cantidadPixeles = obtenerCantidadPixeles(datosCompresion);
        long cantidadComprimida = obtenerBitsComprimidos(datosCompresion);
        float tasaCompresion = obtenerTasaCompresion(datosCompresion);

        puts("Ejecucion finalizada.\n");
        puts("-------------------------------------------------");
        puts("Resumen:");
        printf("Cantidad de pixeles en archivo original: %ld\n",cantidadPixeles);
        printf("Cantidad de bytes en archivo comprimido: %ld\n",cantidadComprimida >> 3);
        printf("Tasa de compresion (bytes/pixeles): %f\n",tasaCompresion);
        puts("-------------------------------------------------");
        puts("");
    }
    destruirDatosCompresion(datosCompresion);

    return EXIT_SUCCESS;
}

// Rutina para errores
void abortar( char *mensaje ) {
    puts("Error: ");
    puts(mensaje);
    puts("\n ");
    exit(EXIT_FAILURE);
}

