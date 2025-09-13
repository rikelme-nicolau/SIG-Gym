#include <ctype.h>
#include <string.h>
#include <time.h>
#include "validacoes.h"

#define true 1
#define false 0

int validaCPF(char *cpf) {//feito por Flávia Carvalho
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

    for(int i = 0; i<9; i +=1){
        soma += (cpf[i] - '0')*(10 - i);// pega o valor do código ascii de '0' e subtrai pelo valor do codigo ascii de cpf[i]
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






int validaTelefone(char *telefone){//feito por Flávia Carvalho
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







int validaNome(char *nome){//feito por Flávia Carvalho
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





int validaNascimento(char *nascimento){//feito por Flávia Carvalho
    time_t tempoAtual = time(NULL);        
    struct tm *dataAtual = localtime(&tempoAtual); 

    int diaA = dataAtual->tm_mday;        
    int mesA = dataAtual->tm_mon + 1;     
    int anoA = dataAtual->tm_year + 1900;

    int tam = strlen(nascimento);
    if(tam != 8){
        //data de nascimento incompleta, pedir para o usário colocar no modelo 00/00/0000
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
        //idade insuficiente
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
            if(!((anoN % 4 == 0 && anoN %100 != 0) || (anoN % 400 == 0))){//verificação ano bissexto
                return false;
            }
        }
    }
return true;
}







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

