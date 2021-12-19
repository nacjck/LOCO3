/* 
 * TAREA LOCO ATIPI - NTI/FING/UDELAR
 * 
 * Autores:
 * 	Alexis Baladón
 * 	Juan Ignacio Fernandez
 */


#include "include/compresor.h"
#include "include/datosCompresion.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef bool
    #define bool char
#endif

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
                DatosCompresion datosComprimidos = comprimir(archivoImagen,"prueba.bin",s,i);
                fprintf(archivoResultados,"%f ",obtenerTasaCompresion(datosComprimidos));
            }
            fprintf(archivoResultados,"\n");
        }
        fprintf(archivoResultados,"\n");
        fseek(listadoImagenes,0,SEEK_SET);
    }
}

typedef enum { COMPRIMIR, DESCOMPRIMIR } Funcionalidad;

/*
 * Declaraciones para rutinas locales
 */
void abortar( char* mensaje );

int main( int argc, char* argv[] ) {
    
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
    puts("Obtencion de datos finalizada.\n");
    printf("Puede encontrar sus datos en: %s\n",directorioParaResultados);
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

