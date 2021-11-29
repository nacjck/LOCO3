#include "../include/pixelio.h"

#include <string.h>

void leerImgComp(FILE* archivoComprimido, imagen* img);
BYTE* leerNBytes(FILE* archivoComprimido, unsigned int N);
void contexto(imagen* img, int ind, BYTE* a, BYTE* b, BYTE* c, BYTE* d);
