/* 
 * TAREA LOCO ATIPI - NTI/FING/UDELAR
 * 
 * Autores:
 * 	Alexis Baladón
 * 	Juan Ignacio Fernandez
 */


#include "../../../Programa/include/compresor.h"
#include "../../../Programa/include/datosCompresion.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef bool
    #define bool char
#endif

void registrarDatosDePrueba( char * direccionArchivoImagenes, char * direccionArchivoResultados ) {
    //obtener nombres de archivos ena rreglo
    FILE * listadoImagenes   = fopen(direccionArchivoImagenes,"r");
    FILE * archivoResultados = fopen(direccionArchivoResultados,"w");
    char archivoImagen[100]  = {0};
    
    //Tasa de compresion variando s, archivo y modalidad
    int i,s;
    float tasaCompresion = 0;
    float promedioTasaCompresion[11] = {0};
    int cantidadImagenes = 0;

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
                tasaCompresion = obtenerTasaCompresion(datosComprimidos);
                fprintf(archivoResultados,"%f ",tasaCompresion);

                //Modo de run
                if (i == 1) promedioTasaCompresion[s] += tasaCompresion;
            }
            fprintf(archivoResultados,"\n");

            //Modo de run
            if (i == 1) cantidadImagenes++;
        }
        fprintf(archivoResultados,"\n");
        fseek(listadoImagenes,0,SEEK_SET);
    }

    //Promedios de modo de run
    fprintf(archivoResultados,"TasaCompresion ");
    for(s = 0; s <= 10; s++) {
        if (cantidadImagenes > 0) promedioTasaCompresion[s] /= cantidadImagenes;
        fprintf(archivoResultados,"%f ",promedioTasaCompresion[s]);
    }
    
    //Elimina archivo de prueba
    remove("prueba.bin");
    
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
    
    registrarDatosDePrueba(archivoDireccionImagenes, directorioParaResultados);
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

