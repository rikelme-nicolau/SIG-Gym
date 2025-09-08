#include <ctype.h>
#include <string.h>

#define true 1
#define false 0

int validaEmail(char *email){//validação feita por Flávia Carvalho
    int cont = 0;
    int cont2 = 0;
    int tam = 0;
    char letra;
    for(int i = 0; email[i] != '\0'; i += 1){
        if(email[i] == '@'){
            cont += 1;//conta quantos @ tem no email
            if(i == 0){
                return false;//se o @ estiver na posição inicial, retorna falso
            }
            if((!isalnum(email[i+1])) || (!isalnum(email[i-1]))){//se os caracteres imediatamente antes ou apos o @ não forem letra ou número, invalido
                return false;
            }
            for(int j = i; email[j] != '\0'; j +=1){
                if(email[j] == '.'){
                    cont2 += 1;
                }
            }
        }
        if(email[i] == '.' && email[i+1] == '.'){//se um ponto vier seguido de outro, retorna falso
                return false;
            }
        letra = email[i];
        if (!(isalnum(letra) || letra == '.' || letra == '-' || letra == '_' || letra == '@')){ //se o caracter não for nenhuma dessas alternativas, retorna falso
            return false;
        }
    }
    if(cont != 1){//se a contagem de @ for diferente de 1
        return false;
    }
    if(cont2 < 1){//se acontagem de . for menor que 1, retorna falso
        return false;
    }

    cont = 0;

    tam = strlen(email);//recebe o tamanho da string começando a contagem por 1
    if(!isalnum(email[tam - 1])){//se o caracter não for letra ou numero, invalido
        return false;
    }
    if(!isalnum(email[0])){//se o primeiro caracter não for letra ou número, invalido
        return false;
    }
    for(int j = tam-1; email[j] != '.'; j -= 1){//enquanto o caracter do email na posição j não for um . conta quantos caracteres tem
        cont +=1;
    }
    if(cont < 2){ //se for menor que 2, não aceita
        return false;
    }
    cont = 0;
    for(int k = 0; email[k] != '@'; k += 1){//na mesma ideia, só que conta o tamanho da parte local do email
        cont +=1;
    }
    if(cont < 3 || cont > 64){//se for menor que 3 ou maior que 64, invalido (minimo e maximo permitido que encontrei que algumas empresas permitem, Outlook/Hotmail por exemplo, varia 3 a 6 o minimo)
        return false;
    }
return true;
}