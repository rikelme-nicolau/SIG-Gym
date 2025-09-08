#include <ctype.h>
#include <string.h>
#include <time.h>

#define true 1
#define false 0

int validaNascimento(char *nascimento){
    time_t tempoAtual = time(NULL);        
    struct tm *dataAtual = localtime(&tempoAtual); 

    int diaA = dataAtual->tm_mday;        
    int mesA = dataAtual->tm_mon + 1;     
    int anoA = dataAtual->tm_year + 1900;

    int tam = strlen(nascimento);
    if(tam != 8){
        //mensagem de data de nascimento incompleta, pedir para o usáario colocar no modelo 00/00/0000
        return false;
    }
    int diaN = ((nascimento[0] - '0')*10 + (nascimento[1] - '0'));
    int mesN = ((nascimento[2] - '0')*10 + (nascimento[3] - '0'));
    int anoN = ((nascimento[4] - '0')*1000 + (nascimento[5] - '0')*100 + (nascimento[6] - '0')*10 + (nascimento[7] - '0'));
    int idade = (anoA - anoN);
    if (mesN > mesA || (mesN == mesA && diaN > diaA)){
        idade -= 1;
    }
    if(idade < 10){
        //mensagem de idade insuficiente
        return false;
    }

    if(diaN > 31 || diaN < 1){
        return false;
    }
    if(mesN > 12 || mesN < 1){
        return false;
    }
    if(anoN > anoA || anoN < anoA - 100){
        return false;
    }
    
    if(mesN ==  4 || mesN == 6 || mesN == 9 || mesN == 11){
        if(diaN > 30){
            return false;
        }
    }
    if(mesN == 2){
        if(diaN > 29){
            return false;
        }
        if(diaN == 29){
            if(!((anoN % 4 == 0 && anoN %100 != 0) || (anoN % 400 == 0))){
                return false;
            }
        }
    }
return true;
}