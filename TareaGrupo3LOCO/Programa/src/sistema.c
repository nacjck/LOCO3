#include "../include/sistema.h"
#include "../include/compresor.h"
#include "../include/descompresor.h"
#include "../include/modalidad.h"

void comprimir( char* archivoEntrada, char* archivoSaluda, 
    int s, modalidad_t modalidad ) {
        /*
         * Esta es una idea mia jaja no significa que tengamos que hacerlo asi
         *
         * La idea es que estas funciones sean lo mas cortas posibles
         * y que se ayuden de funciones definidas en comprimir.h o descomprimir.h
         * o alguna libreria que podamos usar de ambas
         * 
         * ejemplo:
         * if(modo de run) {
         * bool fin = false;
         *     while( quedan pixeles ) {
         *         hacerCosas()
         *         hacerMasCosas()
         *     }
         * }
         * else {
         *     
         *     while( quedan pixeles ) {
         *         hacerCosas()
         *     }
         * }
         */
}

void descomprimir( char* archivoEntrada, char* archivoSaluda ) {

}