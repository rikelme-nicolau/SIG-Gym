#include <string.h>

#define true 1
#define false 0

int validaTelefone(char *telefone){
    int tam;
    int ddd;
    tam = strlen(telefone);
    ddd = (telefone[0] - '0')*10 +(telefone[1] - '0');
    if(tam > 11 || tam < 10){
        return false;
    }
    if (tam == 11){
        if(ddd < 11 || ddd > 99){
            return false;
        }
        if(telefone[2]-'0' != 9){
            return false;
        }
    }
    if(tam == 10){
        if(ddd < 11 || ddd > 99){
            return false;
        }
        if((telefone[2] - '0') < 2){
            return false;
        }
        else if((telefone[2] - '0') > 5){
            return false;
        }
    }
    return true;
}