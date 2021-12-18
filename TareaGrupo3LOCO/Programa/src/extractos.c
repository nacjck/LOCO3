#include "../include/extractos.h"

#include <stdlib.h>
#include <stdio.h>

#define R 128

/*
 * Estructura para extracto f(C)
 */
struct _extracto {
    int A;
    int N;
};

struct _extractos {
    Extracto * extractos;
    int cantidadExtractos;  
};

/*
 * Inicializa los extractos correspondientes a cada caracter con N=1 y A=8.
 */
Extractos crearExtractos( int s ) {
    Extractos extractos;
    int cantExtractos;
    int i;

    extractos = malloc(sizeof(struct _extractos));
    extractos->extractos = malloc(sizeof(Extracto) << (s + 3));
    extractos->cantidadExtractos = 1 << s + 3;

    for (i = 0; i < extractos->cantidadExtractos; i++) {
        Extracto ext = malloc(sizeof(struct _extracto));
        ext->N = 1;
        ext->A = 8;
        extractos->extractos[i] = ext;
    }

    return extractos;
}

/*
 * Retorna el valor predicho para x dado el contexto (a,b,c)
 */
PIX predecirX( PIX a, PIX b, PIX c ) {
    /* Calcula la predicción del pixel dado su contexto */
    PIX maxAB, minAB;

    // Máximo y mínimo entre a y b
    if (a >= b) {
      maxAB = a;
      minAB = b;
    }
    else {
      maxAB = b;
      minAB = a;
    }

    // Fórmula para hat_x
    PIX hatX;
    if (c >= maxAB) {
      hatX = minAB;
    }
    else if (c <= minAB) {
      hatX = maxAB;
    }
    else {
      hatX = a + b - c;
    }

    return hatX;
}

/*
 * Retorna el extracto correspondiente al x predicho dado
 * el contexto (a,b,c)
 */
int determinarIndiceExtracto( PIX xPrediccion, PIX a, PIX b, PIX c , int s) {
    // Devuelve el extracto f(C)
    // Máximo s+3 bits
    // Nota: La textura y el nivel de actividad se pueden calcular al mismo tiempo

    int Q, T, X, fC;

    // Devuelve el nivel de actividad X
    X = 0; // Máximo 10 bits
    X += (c >= xPrediccion) ? (c-xPrediccion) : (xPrediccion-c);
    X += (b >= xPrediccion) ? (b-xPrediccion) : (xPrediccion-b);
    X += (a >= xPrediccion) ? (a-xPrediccion) : (xPrediccion-a);

    Q = (X >> (10-s)); // Cuantización de X

    T =  (c>xPrediccion) << 2; // MSB
    T += (b>xPrediccion) << 1;
    T += (a>xPrediccion); // LSB

    fC = (Q<<3) + T; // f(C) = Q*8 + T

    return fC;
}

Extracto determinarExtracto( Extractos extractos, int fC ) {
  return extractos->extractos[fC];
}

/*
 * Retorna el parámetro k de Golomb dado un extracto
 */
int determinarGolombK( Extracto extracto ) {
    // Calcula el parámetro k del código Golomb PO2

    int k;

    for ( k=0; (extracto->N << k) < extracto->A; k++ );

    return k;
}

/*
 * Actualiza las variables A y N del extracto
 */
void actualizarExtracto( Extracto extracto, int error ) {
    if (extracto->N == R) {
        extracto->N >>= 1;
        extracto->A >>= 1;
    }
    extracto->A += (error < 0) ? -error : error;
    extracto->N++;
}

void destruirExtractos(Extractos extractos) {
    int i;
    for(i = 0; i < extractos->cantidadExtractos; i++) {
        free(extractos->extractos[i]);
    }
    free(extractos->extractos);
    free(extractos);
}
