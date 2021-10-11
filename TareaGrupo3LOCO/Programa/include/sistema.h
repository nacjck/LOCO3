/* sistema.h
 *
 * En este archivo se declaran las funciones principales 
 * del programa principal
 */

#ifndef sistema_h
#define sistema_h

void comprimir( char* archivoEntrada, char* archivoSaluda, 
    int s, modalidad_t modalidad );

void descomprimir( char* archivoEntrada, char* archivoSaluda );

#endif