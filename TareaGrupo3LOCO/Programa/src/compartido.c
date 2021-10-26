#include "../include/compartido.h"

#include <stdio.h> // Para debugg

/* Calcula la predicción del pixel dado su contexto */
PIX predict(PIX a, PIX b, PIX c) {

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

// Para test y debugg
int main() {
  printf("hat_x=%d", predict(0,11,5));
}
