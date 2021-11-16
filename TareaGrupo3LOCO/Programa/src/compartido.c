#include "../include/compartido.h"

#include <stdio.h>

#define R 128

static int s;
/*
 * Este arreglo es el que se inicializa al comienzo del programa con N=1 y A=8
 */
static Extracto ** extractos;
static int cantExtractos; // 1 << (s + 3)

/*
 * Inicializa los extractos correspondientes a cada caracter con N=1 y A=8.
 */
void inicializarExtractos( int _s ) {
    int i;

    s = _s;
    cantExtractos = 1 << (_s + 3);
    extractos = malloc(sizeof(Extracto) << (_s + 3));
    for (i = 0; i < cantExtractos; i++) {
        Extracto * ext = malloc(sizeof(Extracto *));
        ext->N = 1;
        ext->A = 8;
        extractos[i] = ext;
    }
}

/*
 * Retorna el valor predicho para x dado el contexto (a,b,c)
 */
unsigned char predecirX( unsigned char a, unsigned char b, unsigned char c ) {
    /* Calcula la predicción del pixel dado su contexto */
    unsigned char max_ab, min_ab;

    // Máximo y mínimo entre a y b
    if (a >= b) {
      max_ab = a;
      min_ab = b;
    }
    else {
      max_ab = b;
      min_ab = a;
    }

    // Fórmula para hat_x
    unsigned char hat_x;
    if (c >= max_ab) {
      hat_x = min_ab;
    }
    else if (c <= min_ab) {
      hat_x = max_ab;
    }
    else {
      hat_x = a + b - c;
    }

    return hat_x;
}

/*
 * Retorna el extracto correspondiente al x predicho dado
 * el contexto (a,b,c)
 */
Extracto * determinarExtracto( unsigned char xPrediccion, unsigned char a, unsigned char b, unsigned char c ) {
    // Devuelve el extracto f(C)
    // Máximo s+3 bits
    // Nota: La textura y el nivel de actividad se pueden calcular al mismo tiempo

    unsigned short Q, T, X, fC;

    // Devuelve el nivel de actividad X
    X = 0; // Máximo 10 bits
    X += (c >= xPrediccion) ? (c-xPrediccion) : (xPrediccion-c);
    X += (b >= xPrediccion) ? (b-xPrediccion) : (xPrediccion-b);
    X += (a >= xPrediccion) ? (a-xPrediccion) : (xPrediccion-a);

    Q = (xPrediccion >> (10-s)); // Cuantización de X

    T =  (c>xPrediccion) << 2; // MSB
    T += (b>xPrediccion) << 1;
    T += (a>xPrediccion); // LSB

    fC = (Q<<3) + T; // f(C) = Q*8 + T

    return extractos[fC];
}

/*
 * Retorna el parámetro k de Golomb dado un extracto
 */
int determinarGolombK( Extracto * extracto ) {
    // Calcula el parámetro k del código Golomb PO2

    unsigned short k;

    for ( k=0; (extracto->N << k) < extracto->A; k++ ); // La fórmula está en el artículo y en las diapos

    return k;
}

/*
 * Retorna el mapeo M(e)
 */
int determinarMapeoRice( int errorPrediccion ) {
    // Map de los errores de predicción al rango no negativo
    unsigned short M;

<<<<<<< HEAD
    if (errorPrediccion < 0) {
      M = -(errorPrediccion << 1) + 1;
    } else {
      M = errorPrediccion << 1;
    }
=======
uint16_t n_act(PIX a, PIX b, PIX c, PIX x) {
  // Devuelve el nivel de actividad X
  uint16_t X = 0; // Máximo 10 bits
>>>>>>> 185eb18 (Se cambiarona lagunos tipos y se terminó archivo de encabezados.)

    return M;
}

int determinarLargoBinaryGolomb( int k, int M, int * bin_arg ) {
    // Devuelve el largo de la parte binaria de Golomb_k(M)
    // Ademas retorna bin_arg con la parte binaria.
    // El largo del código es l = k+1
    unsigned int bin_length;

    *bin_arg = M & ((1<<k)-1);    /* Binary_k(M)       */
    bin_length = k;               /* Largo Binary_k(M) */

    return bin_length;
}

<<<<<<< HEAD
int determinarLargoUnaryGolomb( int k, int M ) {
    // Devuelve el código de Golomb como un entero sin signo
    // El largo del código es l = M/2^k
    unsigned int un_length;

    un_length = (M>>k) + 1;    /* Largo Unary_k(M) */
=======
uint16_t extract(uint16_t X, BYTE T, BYTE s) {
  // Devuelve el extracto f(C)
  // Máximo s+3 bits

  uint16_t Q, fC;
>>>>>>> 185eb18 (Se cambiarona lagunos tipos y se terminó archivo de encabezados.)

    return un_length;
}

/*
 * Actualiza las variables A y N del extracto
 */
void actualizarExtracto( Extracto * fExtracto, int error ) {
    if (fExtracto->N == R) {
        fExtracto->N >>= 1;
        fExtracto->A >>= 1;
    }
    fExtracto->A += (error < 0) ? -error : error; // ((error << 1) >> 1)
    fExtracto->N++;
}

<<<<<<< HEAD
<<<<<<< HEAD
void liberarExtractos() {
    int i;
    for(i = 0; i < cantExtractos; i++) {
        free(extractos[i]);
    }
    free(extractos);
=======
unsigned short get_kvalue(unsigned int N, unsigned int A) {
=======
uint16_t get_k(unsigned int N, unsigned int A) {
>>>>>>> 185eb18 (Se cambiarona lagunos tipos y se terminó archivo de encabezados.)
  // Calcula el parámetro k del código Golomb PO2

  uint16_t k;

  for ( k=0; (N << k) < A; k++ ); // La fórmula está en el artículo y en las diapos

  return k;
}

uint16_t NN_map(int e) {
  // Map de los errores de predicción al rango no negativo

  uint16_t M;

  if (e < 0) {
    M = -2*e + 1;
  } else {
    M = 2*e;
  }

  return M;
}

unsigned int get_gPO2(uint16_t k, uint16_t M) {
  // Devuelve el código de Golomb como un entero sin signo
  // El largo del código es l = k+1 + M/2^k

  unsigned int gPO2, un, bin;

  bin = M & ((1<<k)-1);
  un = M >> k;
  gPO2 = (un << k) + bin; // gPO2(i) = unary(i)binary(i)

  return gPO2;

}

uint16_t get_gPO2_length(uint16_t k, uint16_t M) {
  // Devuelve el largo en bits del código de Golomb PO2

  uint16_t l;

  l = k+1 + M/(2<<k);

  return l;

}


/*============================================================================*/
// Para test y debugging
>>>>>>> c2cf1ee (Golomb)
int main() {
  unsigned int a, b, c, hat_x, s;
  BYTE T;
  unsigned short X, fC;

  printf("Ingresar valores de contexto: a, b, c:\n");
  scanf("%u%u%u", &a, &b, &c);
  printf("a=%d, b=%d, c=%d\n", a, b, c);

  hat_x = predict(a, b, c);
  T = textura(a, b, c, hat_x);
  X = n_act(a, b, c, hat_x);

  printf("Ingresar parámetro s:\n");
  scanf("%u", &s);
  fC = extract(X, T, s);

  printf("Prediccion: hat_x=%d\n", hat_x);
  printf("Textura: T=%x\n", T);
  printf("Nivel de actividad: X=%u\n", X);
  printf("Extracto: f(C)=%u", fC);
}
>>>>>>> 8509518 (Función de textura terminada.)
