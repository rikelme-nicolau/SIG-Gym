#include <ctype.h>

void limpaCPF(char *cpf){
    int j = 0;
    for(int i = 0; cpf[i] != '\0'; i += 1){
        if(isdigit((unsigned char)cpf[i])){
            cpf[j] = cpf[i];
            j += 1;
        }
    }
    cpf[j] = '\0';//necessário para encerrar a string, se não fica errado
}