/*
 * compartido.h
 *
 * Aquí se hallan funciones y estructuras utilizadas
 * tanto en el compresor como en el descompresor
 *
 */

#ifndef compartido_h
#define compartido_h

#include <stdlib.h>

/*
 * Estructura para extracto f(C)
 */
typedef struct {
    int A;
    int N;
} Extracto;

/*
 * Inicializa los extractos correspondientes a cada caracter con N=1 y A=8.
 */
void inicializarExtractos( int s );

/*
 * Retorna el valor predicho para x dado el contexto (a,b,c)
 */
unsigned char predecirX( unsigned char a, unsigned char b, unsigned char c );

/*
 * Retorna el extracto correspondiente al x predicho dado
 * el contexto (a,b,c)
 */
Extracto * determinarExtracto( unsigned char xPrediccion, unsigned char a, unsigned char b, unsigned char c );

/*
 * Retorna el parámetro k de Golomb dado un extracto
 */
int determinarGolombK( Extracto * extracto );

/*
 * Retorna el mapeo M(e) 
 */
int determinarMapeoRice( int errorPrediccion );

int determinarLargoBinaryGolomb( int k, int M, int * bin_arg );

int determinarLargoUnaryGolomb( int k, int M );
/*
 * Actualiza las variables A y N del extracto
 */
void actualizarExtracto( Extracto * fExtracto, int errorPrediccion );

void liberarExtractos();

#endif

typedef unsigned char PIX; // Alias para valores de pixel
