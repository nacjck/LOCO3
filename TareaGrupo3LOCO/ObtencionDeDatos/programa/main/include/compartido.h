/*
 * compartido.h
 *
 * Aquí se hallan funciones y estructuras utilizadas
 * tanto en el compresor como en el descompresor
 *
 */

#ifndef compartido_h
#define compartido_h

#ifndef BYTE
  #define BYTE unsigned char
#endif

#ifndef PIX
  #define PIX unsigned char
#endif

typedef struct _extracto* Extracto;
typedef struct _extractos* Extractos;

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

// Cabeceras de funciones

// PIX predict(PIX a, PIX b, PIX c); // Predictor
//
// BYTE textura(PIX a, PIX b, PIX c, PIX x); // Calcula bits de textura
//
// uint16_t n_act(PIX a, PIX b, PIX c, PIX x); // Calcula nivel de actividad
//
// uint16_t extract(uint16_t X, BYTE T, BYTE s); // Calcula extracto
//
// uint16_t get_k(unsigned int N, unsigned int A); // Calcula el parámetro k del código Golomb PO2
//
// uint16_t NN_map(int e); // Map de los errores de predicción al rango no negativo
//
// unsigned int get_gPO2(uint16_t k, uint16_t M); // Devuelve el código de Golomb como un entero sin signo
//
// uint16_t get_gPO2_length(uint16_t k, uint16_t M); // Devuelve el largo en bits del código de Golomb PO2
