#include <ctype.h>

void limpaTelefone(char *telefone){
    int j = 0;
    for(int i = 0; telefone[i] != '\0'; i += 1){
        if(isdigit((unsigned char)telefone[i])){
            telefone[j] = telefone[i];
            j += 1;
        }
    }
    telefone[j] = '\0';//necessário para encerrar a string, se não fica errado
}