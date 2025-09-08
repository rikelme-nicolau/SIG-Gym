#include <ctype.h>
#include <string.h>

#define true 1
#define false 0

int validarCPF(char *cpf) {//código de validação feito por Flávia Carvalho
    int tam;
    int cont = 0;
    int soma = 0;
    int resto = 0;
    int verifica1 = 0;
    int verifica2 = 0;
    tam = strlen(cpf);
    if(tam != 11){
        return false;
    }
    for(int i = 0; i < 11; i +=1){
        if(cpf[0] == cpf[i]){
            cont += 1;
        }
    }
    if (cont >= 11){
        return false;
    }

    for(int i = 0; i<10; i +=1){
        soma += (cpf[i] - '0')*(10 - i);// pega o valor do código '0' e subtrai pelo valor do codigo de cpf[i]
    }

    resto = soma %11;

    if(resto<2){
        verifica1 = 0;
    }
    else if(resto>=2){
        verifica1 = 11 - resto;
    }

    if((cpf[9] - '0') != verifica1){
        return false;
    }

    soma = 0;
    resto = 0;

    for(int i = 0; i < 10; i += 1){
        soma += (cpf[i] - '0')*(11-i);
    }

    resto = soma%11;

    if(resto<2){
        verifica2 = 0;
    }
    else if(resto>=2){
        verifica2 = 11 - resto;
    }
    if(cpf[10] - '0' != verifica2){
        return false;
    }
    return true;

    }