/*
 * arregloCircular.h
 *
 * Aquí se encuentra la estructura y funciones
 * correspondientes al TAD arregloCircular
 */

#ifndef arregloCircular_h
#define arregloCircular_h

typedef struct {
    int maxIndice;              /* tamanio - 1 */
    int ini,fin;
    unsigned char * valores;
} ArregloCircular;

/*
 * Inicializa la estructura Cola.
 */
ArregloCircular * crearArregloCircular( int tamanio );

/*
 * Agrega un elemento al inicio de la cola.
 */
void agregarElemento( ArregloCircular * ac, unsigned char elem );

/*
 * Obtiene el elemento de la cola en la
 * posición indicada.
 */
unsigned char obtenerElemento( ArregloCircular * ac, int indice );

/*
 * Obtiene el elemento al final de la cola
 */

/*
unsigned char obtenerUltimoElemento( Cola * cola );
/*
 * Retorna y elimina el elemento al final 
 * de la cola.
 * 

unsigned char desencolar(Cola * cola);*/

/*
 * Libera la memoria almacenada en la estructura
 */
void destruirArregloCircular( ArregloCircular * ac );

#endif