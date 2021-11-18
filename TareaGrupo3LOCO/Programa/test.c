
#include <stdio.h>

#include "include/pixelio.h"

#define MAX_PRUEBAS 4

void test1() {
    int input;
    printf("Prueba");
    scanf("%d",&input);
}

int main() {
    int i;
    int input;

    /*  Hardcodear */
    void (*testsFunciones[MAX_PRUEBAS])() = {test1};
    char testsNombres[MAX_PRUEBAS][32] = {"Test 1",""};

    while(1) {
        printf("Ingrese el test que desea ejecutar: \n");
        for(i = 0; i < MAX_PRUEBAS; i++) {
            printf("%d: %s\n",i+1,testsNombres[i]);
        }
        printf("\n");
        scanf("%d",&input);
        printf("\n");
        (testsFunciones[input-1])();
        printf("\n");
        printf("\n");
    }
}

