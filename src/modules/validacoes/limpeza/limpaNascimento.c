#include <ctype.h>

void limpaNascimento(char *nascimento){
    int j =0;
    for(int i = 0; nascimento[i] != '\0'; i +=1){
        if(isdigit((unsigned char)nascimento[i])){
            nascimento[i] = nascimento[j];
            j +=1;
        }
    }
    nascimento[j] = '\0';
}