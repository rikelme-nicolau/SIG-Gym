#include <ctype.h>
#include <string.h>

#define true 1
#define false 0

int validaNome(char *nome){
    const char *acentos = "áàâãéêíóôõúüçÁÀÂÃÉÊÍÓÔÕÚÜÇ";
    int cont = 0;
    int tam = strlen(nome);
    if(tam < 5){
        return false;
    }
    for(int i = 0; nome[i] != '\0'; i += 1 ){
        if(!(isalpha((unsigned char)nome[i]) || strchr(acentos, nome[i]) || nome[i] == '\'' || nome[i] == '-' || nome[i] == ' ')){
            return false;
        }
        if( nome[i] == '\'' || nome[i] == '-' || nome[i] == ' ' ){
            if(i == 0 || i == tam-1){
                return false;
            }
            if(!(isalpha((unsigned char)nome[i+1])) && !(strchr(acentos, nome[i+1]))){
                return false;
            }
            if(!(isalpha((unsigned char)nome[i-1])) && !(strchr(acentos, nome[i-1]))){
                return false;
            }
        }
        if(nome[i] == ' '){
            cont +=1;
        }
        if((nome[i] == ' ' && nome[i+1] == ' ') || (nome[i] == ' ' && nome[i+1] == '\'') || (nome[i] == ' ' && nome[i+1] == '-')){
            return false;
        }
        if((nome[i] == '\'' && nome[i+1] == ' ') || (nome[i] == '\'' && nome[i+1] == '\'') || (nome[i] == '\'' && nome[i+1] == '-')){
            return false;
        }
        if((nome[i] == '-' && nome[i+1] == ' ') || (nome[i] == '-' && nome[i+1] == '\'') || (nome[i] == '-' && nome[i+1] == '-')){
            return false;
        }
    }
    if(cont == 0){
            return false;
        }
    return true;
}