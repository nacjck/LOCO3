/* 
 * TAREA LOCO ATIPI - NTI/FING/UDELAR
 * 
 * Autores:
 * 	Alexis BaladónAG
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

#include "include/compresor.h"
#include "include/modalidad.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void registrarDatosDePrueba( char * direccionArchivoImagenes, char * direccionArchivoResultados ) {
    //obtener nombres de archivos ena rreglo
    FILE * listadoImagenes = fopen(direccionArchivoImagenes,"r");
    FILE * archivoResultados = fopen(direccionArchivoResultados,"w");
    char archivoImagen[100] = {0};
    
    puts("Esto podria tardar un tiempo...");
    
    //TASA DE COMPRESION VARIANDO S,ARCHIVO Y MODALIDAD
    int i,s;
    for(i = 0; i <= 1; i++) {
        
        //Nombre de columnas
        fprintf(archivoResultados,"RUN=%d",i);
        for(s = 0; s <= 10; s++) {
            fprintf(archivoResultados,"% d",s);
        }
        fprintf(archivoResultados,"\n");
        
        //Dato para cada fila
        while (fgets(archivoImagen,100,listadoImagenes)) {
            archivoImagen[strlen(archivoImagen) - 1] = 0; //Se quita fin de linea
            fprintf(archivoResultados,"%s ",archivoImagen);
            for(s = 0; s <= 10; s++) {
                DtCompresion datosComprimidos = comprimir(archivoImagen,"prueba.bin",s,i);
                fprintf(archivoResultados,"%f ",datosComprimidos.tasaCompTotal);
            }
            fprintf(archivoResultados,"\n");
        }
        fprintf(archivoResultados,"\n");
        fseek(listadoImagenes,0,SEEK_SET);
    }
}

typedef enum { COMPRIMIR, DESCOMPRIMIR } Funcionalidad;

typedef struct {
    char            *archivoEntrada;
    char            *archivoSalida;
    int             s;                  /* 0 <= s <= 10 */
    Modalidad     modalidad;
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
    Funcionalidad funcionalidad = COMPRIMIR;
    Parametros parametros = {NULL,NULL,0,NORMAL};
    
    //ARCHIVO DONDE SE ENCUENTRAN DIRECCIONES DE IMAGENES
    char * archivoDireccionImagenes = NULL;
    //ARCHIVO DONDE SE ALMACENARAN RESULTADOS
    char * directorioParaResultados = NULL;
    
    int i = 1;
    while( i < argc ) {
        //ARCHIVO DONDE SE ENCUENTRAN DIRECCIONES DE IMAGENES
        if ( archivoDireccionImagenes == NULL ) {
            archivoDireccionImagenes = argv[i];
            i++;
        } else if ( directorioParaResultados == NULL ) {
            directorioParaResultados = argv[i];
            i++;
        } else {
            abortar("Demasiados parametros.");
        }
    }
    
    /*
     * Control de precondiciones
     */
    if ( archivoDireccionImagenes == NULL ) {
        abortar("Ubicacion de archivo de entrada no especificada.");
    }
    
    if ( directorioParaResultados == NULL) {
        abortar("Ubicacion de archivo de salida no especificada.");
    }

    /*
     * Inicio de compresión/descompresión
     */
    puts("\nObteniendo datos...");
    registrarDatosDePrueba(archivoDireccionImagenes, directorioParaResultados);
    puts("Obtencion de datos finalizada.");
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

