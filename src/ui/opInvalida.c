#include <stdio.h>
#include "limparTela.h"

void opInvalida(void){
    
    printf("====================\n");
    printf("OPÇÃO INVÁLIDA\n");
    printf("====================\n");
    printf("A opção que você digitou é inválida\n");
    printf("< pressione <enter> >\n");
   
    getchar();
    limparTela();
}