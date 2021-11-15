/* 
 * compartido.h
 *
 * Aquí se hallan funciones y estructuras utilizadas
 * tanto en el compresor como en el descompresor
 * 
 */

#ifndef compartido_h
#define compartido_h

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
void inicializarExtractos();

/*
 * Retorna el valor predicho para x dado el contexto (a,b,c)
 */
unsigned char predecirX( unsigned char a, unsigned char b, unsigned char c );

/*
 * Retorna el extracto correspondiente al x predicho dado
 * el contexto (a,b,c)
 */
Extracto * determinarExtracto( unsigned char xPrediccion, unsigned char a, unsigned char b, unsigned char c, int s );

/*
 * Retorna el parámetro k de Golomb dado un extracto
 */
int determinarParametroGolombK( Extracto * extracto );

/*
 * Retorna el mapeo M(e) 
 */
int determinarMapeoRice( int errorPrediccion, Extracto * extracto );

/*
 * Retorna una tira de bits conteniendo los bits a imprimir de Golomb
 * (Puse int provisorio, fijate que puede ser lo mejor para hacerlo)
 */
void determinarGolomb( int k, int elemento, int * cantidadBitsImpresos, int * output );

/*
 * Actualiza las variables A y N del extracto
 */
void actualizarExtracto( Extracto * fExtracto, int errorPrediccion );

void liberarExtractos();

#endif
