#include <ctype.h>
#include <string.h>
#include "limpeza.h"

void ApenasDigitos(char *str){
    int j = 0;
    for(int i = 0; str[i] != '\0'; i += 1){
        if(isdigit((unsigned char)str[i])){
            str[j] = str[i];
            j += 1;
        }
    }
    str[j] = '\0';//necessário para encerrar a string, se não fica errado
}



void limpaNome(char *nome){
    int tam;
    int inicio = 0;
    for(int i = 0; nome[i] ==' '; i +=1){
        inicio += 1;
    }
    if(inicio != 0){
        for(int j = 0; nome[j] != '\0'; j += 1){
            nome[j] = nome[inicio];
            inicio += 1;
        }
    }
    tam = strlen(nome);
    for(int k = tam-1; nome[k] == ' '; k -= 1){
        nome[k] = '\0';
    }

}
