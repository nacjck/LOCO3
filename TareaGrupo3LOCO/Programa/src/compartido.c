#include "../include/compartido.h"

#define NUMERO_EXTRACTOS 10 //Hay que calcular cuantos extractos distintos se pueden hallar
#define R 128

typedef unsigned char PIX; // Alias para valores de pixel
typedef unsigned char BYTE; //

/*
 * Este arreglo es el que se inicializa al comienzo del programa con N=1 y A=8
 */
static Extracto * extractos[NUMERO_EXTRACTOS];

/*
 * Inicializa los extractos correspondientes a cada caracter con N=1 y A=8.
 */
void inicializarExtractos() {
    int i;
    for(i = 0; i < NUMERO_EXTRACTOS; i++) {
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
    PIX max_ab;
    PIX min_ab;

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
    PIX hat_x;
    if (c >= max_ab) {
      hat_x = min_ab;
    }
    else if (c <= min_ab) {
      hat_x = max_ab;
    }
    else {
      hat_x = a + b -c;
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

    unsigned short Q, fC;

    Q = (xPrediccion >> (10-s)); // Cuantización de X
    fC = ((Q<<3) + textura(a,b,c,xPrediccion)); // f(C) = Q*8 + T   //cambio (x es X???)

    return fC;
}

/*
 * Retorna el parámetro k de Golomb dado un extracto
 */
int determinarParametroGolombK( Extracto * extracto ) {
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

/*
 * Retorna una tira de bits conteniendo los bits a imprimir de Golomb
 * (Puse int provisorio, fijate que puede ser lo mejor para hacerlo)
 */
void determinarGolomb( int k, int error, int * cantidadBitsImpresos, int * output ) {
    // Devuelve el código de Golomb como un entero sin signo
  // El largo del código es l = k+1 + M/2^k

    unsigned int gPO2, un, bin;
    int M = NN_map(error);  //cambio

    bin = M & ((1<<k)-1);
    un = M >> k;
    gPO2 = (un << k) + bin;
    *output = gPO2;
    *cantidadBitsImpresos = get_gPO2_length(k,M);   //cambio
}

/*
 * Actualiza las variables A y N del extracto
 */
void actualizarExtracto( Extracto * fExtracto, int errorPrediccion ) {

}


BYTE textura(PIX a, PIX b, PIX c, PIX x) {
  // Devuelve el número T con los bits de textura
  BYTE T;

  T = (c>x) << 2; // MSB
  T += (b>x) << 1;
  T += (a>x); // LSB

  return T;
}

// Nota: La textura y el nivel de actividad se pueden calcular al mismo tiempo

unsigned short n_act(PIX a, PIX b, PIX c, PIX x) {
  // Devuelve el nivel de actividad X
  unsigned short X = 0; // Máximo 10 bits

  X += (c >= x) ? (c-x) : (x-c);
  X += (b >= x) ? (b-x) : (x-b);
  X += (a >= x) ? (a-x) : (x-a);

  return X;
}

unsigned short get_gPO2_length(unsigned short k, unsigned short M) {
  // Devuelve el largo del código de Golomb gPO2

  unsigned int l;

  l = k+1 + M/(2<<k);

  return l;
}

void liberarExtractos() {
    int i;
    for(i = 0; i < NUMERO_EXTRACTOS; i++) {
        free(extractos[i]);
    }
}