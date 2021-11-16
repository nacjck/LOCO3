#include <stdbool.h>
#include <stdio.h>

#include "../include/pixelio.h"

void write_code(FILE* outFile, unsigned int code, uint16_t l, BYTE &buffer, BYTE &buffBits) {
  // Escribe el siguiente código en el archivo de la imagen comprimida

  BYTE b;

  if (buffBits == 0) { // Si no hay nada en el buffer

    buffBits = l % 8; // Número de bits que sobran
    buffer = code & ((1<<buffBits) - 1); // LSBs

    // Escribe archivo
  }

}
