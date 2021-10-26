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

unsigned short n_act(PIX a, PIX b, PIX c, PIX x) {
  // Devuelve el nivel de actividad X
  unsigned short X = 0; // Máximo 10 bits

  X += (c >= x) ? (c-x) : (x-c);
  X += (b >= x) ? (b-x) : (x-b);
  X += (a >= x) ? (a-x) : (x-a);

  return X;
}

// Para test y debugg
int main() {
  unsigned int a, b, c, hat_x;
  BYTE T;
  unsigned short X;

  printf("Ingresar valores de contexto: a, b, c:\n");
  scanf("%u%u%u", &a, &b, &c);
  printf("a=%d, b=%d, c=%d\n", a, b, c);

  hat_x = predict(a, b, c);
  T = textura(a, b, c, hat_x);
  X = n_act(a, b, c, hat_x);

  printf("Prediccion: hat_x=%d\n", hat_x);
  printf("Textura: T=%x\n", T);
  printf("Nivel de actividad: X=%u\n", X);
}
