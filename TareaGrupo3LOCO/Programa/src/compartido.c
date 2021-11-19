#include "../include/compartido.h"

#include <stdio.h>

#define R 128

/*
 * Este arreglo es el que se inicializa al comienzo del programa con N=1 y A=8
 */
static Extracto ** extractos;
static int cantExtractos; // 1 << (s + 3)

/*
 * Inicializa los extractos correspondientes a cada caracter con N=1 y A=8.
 */
void inicializarExtractos( int s ) {
    int i;
    
    cantExtractos = 1 << (s + 3);
    extractos = malloc(sizeof(Extracto) << (s + 3));
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
void determinarGolomb( int k, int M, int * lgPO2, int * gPO2 ) {
    // Devuelve el código de Golomb como un entero sin signo
  // El largo del código es l = k+1 + M/2^k

    unsigned int un_arg, bin_arg;

    bin_arg = M & ((1<<k)-1);
    un_arg = M >> k;
    
    *gPO2 = (bin_arg << (un_arg+1)) | 1;    /* Golomb PO2 */
    *lgPO2 = (k+1) + (M>>k);    /* Largo Golomb */
}

/*
 * Actualiza las variables A y N del extracto
 */
void actualizarExtracto( Extracto * fExtracto, int error ) {
    if (fExtracto->N == R) {
        fExtracto->N >>= 1;
        fExtracto->A >>= 1;
    }
    fExtracto->A += (error < 0) ? -error : error;
    fExtracto->N++;
}

void liberarExtractos() {
    int i;
    for(i = 0; i < cantExtractos; i++) {
        free(extractos[i]);
    }
    free(extractos);
}
