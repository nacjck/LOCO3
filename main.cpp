#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>

#include "include/coder.h"
#include "include/bitio.h"

void compress( int k, char* nombre_archivo_descomprimido, char* nombre_archivo_comprimido );
void expand( int k, char *nombre_archivo_comprimido, char *nombre_archivo_descomprimido );


/* 
Si el archivo se llama main.cpp: 
Uso: ./main -k kvalue -c/d infile outfile
Ejemplo: ./main -k -c 8 "A.pbm" "A.bin"

donde:
-k <number>: orden del modelo de markov
infile: archivo de entrada
outfile: archivo de salida
*/

int main(int argc, char* argv[]) {

    // Inicializo variables.
    char * filename = NULL;
    char * outfile_name = NULL;
    int k;
    bool comprimir;

    //Lectura de parámetros
    int i = 1;
    while(i < argc) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'k': 
                    i++;
                    if (sscanf(argv[i], "%d", &k) != 1) {
                        printf("Incorrect k value. \n");
                        return -1;
                    }
                    i++;
                    break;
                case 'c':
                    comprimir = 1;
                    i++;
                    break;
                case 'd':
                    comprimir = 0;
                    i++;
                    break;
                default:
                    printf("Invalid flag.\n");
                    break;
            }
        } else {
            if(filename == NULL) {
                filename = argv[i];
                i++;
            } else if (outfile_name == NULL) {
                outfile_name = argv[i];
                i++;
            } else {
                printf("Too many parameters.\n");
                return -1;
            }
        }
    }

    if(comprimir)
    {
        compress(k,filename,outfile_name);
    }
    else
    {
        expand(k,filename,outfile_name);
    }
    return 0;
}
