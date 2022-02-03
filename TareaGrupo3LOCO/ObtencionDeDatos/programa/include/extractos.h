/*
 * extractos.h
 *
 * Aquí se hallan funciones y estructuras utilizadas
 * tanto en el compresor como en el descompresor
 *
 */

#ifndef extractos_h
#define extractos_h

#ifndef BYTE
  #define BYTE unsigned char
#endif

#ifndef PIX
  #define PIX unsigned char
#endif

/*
 * Estructura de los valores A y N de los extractos
 * junto con una colección para los mismos.
 */
typedef struct _extracto *Extracto;
typedef struct _extractos *Extractos;

/*
 * Reserva memoria para los extractos
 */
Extractos crearExtractos( int s );

/*
 * Retorna el valor predicho para x dado el contexto (a,b,c)
 */
PIX predecirX( PIX a, PIX b, PIX c );

/*
 * Retorna el indice del extracto correspondiente al x predicho dado
 * el contexto (a,b,c) y a 's'
 */
int determinarIndiceExtracto( PIX xPrediccion, PIX a, PIX b, PIX c, int s );

/*
 * Dado el indice fC y un conjunto de extractos, obtiene los datos
 * correspondientes al mismo
 */
Extracto determinarExtracto( Extractos extractos, int fC );

/*
 * Retorna el parámetro k de Golomb dado un extracto
 */
int determinarGolombK( Extracto extracto );

/*
 * Actualiza los valores del extracto
 */
void actualizarExtracto( Extracto extracto, int errorPrediccion );

/*
 * Libera la memoria de los extractos
 */
void destruirExtractos(Extractos extractos);

#endif