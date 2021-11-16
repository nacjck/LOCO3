#include "../include/compartido.h"

#include <stdio.h> // Para debugg

PIX predict(PIX a, PIX b, PIX c) {
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


BYTE textura(PIX a, PIX b, PIX c, PIX x) {
  // Devuelve el número T con los bits de textura
  BYTE T;

  T = (c>x) << 2; // MSB
  T += (b>x) << 1;
  T += (a>x); // LSB

  return T;
}

// Nota: La textura y el nivel de actividad se pueden calcular al mismo tiempo

uint16_t n_act(PIX a, PIX b, PIX c, PIX x) {
  // Devuelve el nivel de actividad X
  uint16_t X = 0; // Máximo 10 bits

  X += (c >= x) ? (c-x) : (x-c);
  X += (b >= x) ? (b-x) : (x-b);
  X += (a >= x) ? (a-x) : (x-a);

  return X;
}

uint16_t extract(uint16_t X, BYTE T, BYTE s) {
  // Devuelve el extracto f(C)
  // Máximo s+3 bits

  uint16_t Q, fC;

  Q = (X >> (10-s)); // Cuantización de X
  fC = ((Q<<3) + T); // f(C) = Q*8 + T

  return fC;
}

uint16_t get_k(unsigned int N, unsigned int A) {
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

  unsigned int gPO2, un_arg, bin_arg;

  bin_arg = M & ((1<<k)-1);
  un_arg = M >> k;
  gPO2 = (bin_arg << (un_arg + 1) ) + 1; // gPO2(i) = binary(M mod 2^k)unary(M / 2^k)
  return gPO2;

}

uint16_t get_gPO2_length(uint16_t k, uint16_t M) {
  // Devuelve el largo en bits del código de Golomb PO2

  uint16_t l;

  l = k+1 + (M>>k);

  return l;

}


/*============================================================================*/
// Para test y debugging
int main() {
  // unsigned int a, b, c, hat_x, s;
  // BYTE T;
  // unsigned short X, fC;
  //
  // printf("Ingresar valores de contexto: a, b, c:\n");
  // scanf("%u%u%u", &a, &b, &c);
  // printf("a=%d, b=%d, c=%d\n", a, b, c);
  //
  // hat_x = predict(a, b, c);
  // T = textura(a, b, c, hat_x);
  // X = n_act(a, b, c, hat_x);
  //
  // printf("Ingresar parámetro s:\n");
  // scanf("%u", &s);
  // fC = extract(X, T, s);
  //
  // printf("Prediccion: hat_x=%d\n", hat_x);
  // printf("Textura: T=%x\n", T);
  // printf("Nivel de actividad: X=%u\n", X);
  // printf("Extracto: f(C)=%u", fC);

  unsigned int code;
  uint16_t l, k;

  k = 2;
  for (int i=0; i<16; i++) {
    code = get_gPO2(k, i);
    l = get_gPO2_length(k, i);
    printf("i=%u G(i)=%x l(G(i))=%u\n", i, code, l);
  }

}
