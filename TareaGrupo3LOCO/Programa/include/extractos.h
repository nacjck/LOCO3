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

typedef struct _extracto *Extracto;
typedef struct _extractos *Extractos;

/*
 * Inicializa los extractos correspondientes a cada caracter con N=1 y A=8.
 */
Extractos crearExtractos( int s );

/*
 * Retorna el valor predicho para x dado el contexto (a,b,c)
 */
PIX predecirX( PIX a, PIX b, PIX c );

/*
 * Retorna el extracto correspondiente al x predicho dado
 * el contexto (a,b,c)
 */
int determinarIndiceExtracto( PIX xPrediccion, PIX a, PIX b, PIX c, int s );

Extracto determinarExtracto( Extractos extractos, int fC );
/*
 * Retorna el parámetro k de Golomb dado un extracto
 */
int determinarGolombK( Extracto extracto );

/*
 * Actualiza las variables A y N del extracto
 */
void actualizarExtracto( Extracto extracto, int errorPrediccion );

void destruirExtractos(Extractos extractos);

#endif