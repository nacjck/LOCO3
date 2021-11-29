#include "../include/compartido.h"

#define NUMERO_EXTRACTOS (1 << (s+3)) //Hay que calcular cuantos extractos distintos se pueden hallar
#define R 128

/*
 * Este arreglo es el que se inicializa al comienzo del programa con N=1 y A=8
 */
static Extracto * extractos[NUMERO_EXTRACTOS];

/*
 * Inicializa los extractos correspondientes a cada caracter con N=1 y A=8.
 */
void inicializarExtractos() {
    int i;
    for (i = 0; i < NUMERO_EXTRACTOS; i++) {
        Extracto * ext = malloc(sizeof(Extracto));
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
Extracto * determinarExtracto( unsigned char xPrediccion, unsigned char a, unsigned char b, unsigned char c, int s) {
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

    T = (c>xPrediccion) << 2; // MSB
    T += (b>xPrediccion) << 1;
    T += (a>xPrediccion); // LSB

    fC = (Q<<3) + T; // f(C) = Q*8 + T   //cambio (x es X???)

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
int determinarMapeoRice( int errorPrediccion, Extracto * extracto ) {
    // Map de los errores de predicción al rango no negativo
    unsigned short M;

    if (errorPrediccion < 0) {
      M = -(errorPrediccion << 1) + 1;
    } else {
      M = errorPrediccion << 1;
    }

    return M;
}

unsigned short determinarLargoGolomb(unsigned short k, unsigned short M) {
  // Devuelve el largo del código de Golomb gPO2

  unsigned int l;

  l = (k+1) + (M>>k) // k+1 + M/2^k;

  return l;
}

/*
 * Retorna una tira de bits conteniendo los bits a imprimir de Golomb
 * (Puse int provisorio, fijate que puede ser lo mejor para hacerlo)
 */
void determinarGolomb( int k, int error, int * cantidadBitsImpresos, int * output ) {
    // Devuelve el código de Golomb como un entero sin signo
  // El largo del código es l = k+1 + M/2^k

    unsigned int gPO2, un_arg, bin_arg;
    int M = (error<0) ? (-(error<<1) + 1) : (error<<1);

    bin_arg = M & ((1<<k)-1);
    un_arg = M >> k;
    gPO2 = (bin_arg << (un_arg+1)) & 1;
    *output = gPO2;
    *cantidadBitsImpresos = (k+1) + (M>>k);    /* Largo Golomb */
}

/*
 * Actualiza las variables A y N del extracto
 */
void actualizarExtracto( Extracto * fExtracto, int errorPrediccion ) {
  //sumar 1 a N y sumar errorPrediccion a A?
}

void liberarExtractos() {
    int i;
    for(i = 0; i < NUMERO_EXTRACTOS; i++) {
        free(extractos[i]);
    }
}
