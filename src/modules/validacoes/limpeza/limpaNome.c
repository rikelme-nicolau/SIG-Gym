#include <string.h>
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