#include <stdio.h>
#include <string.h>

void inserirDados(char *str, int tamanho) {
    fgets(str, tamanho, stdin);
    int tam = strlen(str);

    if (str[tam-1] == '\n'){
        str[tam-1] = '\0';
    }
}