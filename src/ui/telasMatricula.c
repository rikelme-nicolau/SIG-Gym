#include <stdio.h>
#include "telasMatricula.h"


char moduloMatricula(void) {

    char opMatricula;

    do {
        opMatricula = telaMatricula();
        switch(opMatricula) {    
        case '1':       telaCadastrarMatricula();
                        break;
                        
        case '2':       telaVisualizarMatricula();
                        break;
            
        case 
        '3':            telaAtualizarMatricula();
                        break;
            
        case 
        '4':            telaExcluirMatricula();
                        break;
                        
        case '0':
                        break;

        default:
                        opInvalida();
                        break;

        }
    
    } 
    while (opMatricula != '0');
 
    return opMatricula;
}


char telaMatricula(void) {

    char op;

    printf("====================\n");
    printf("MATRICULAS\n");
    printf("====================\n");
    printf("(1) CADASTRAR\n");
    printf("(2) VISUALIZAR\n");
    printf("(3) ATUALIZAR\n");
    printf("(4) EXCLUIR\n");
    printf("(0) SAIR\n");

    scanf("%c", &op);
    getchar();

    return op;
}




char telaCadastrarMatricula(void){
    
    char op;

    printf("====================\n");
    printf("CADASTRAR MATRICULA\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    return op;
    
}



char telaVisualizarMatricula(void) {

    char op;
    
    printf("====================\n");
    printf("VISUALIZAR MATRICULAS\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    
    scanf("%c", &op);
    getchar();

    return op;
    
}


char telaAtualizarMatricula(void) {

    char op;


    printf("====================\n");
    printf("ATUALIZAR MATRICULA\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    
    scanf("%c", &op);
    getchar();

    return op;

    
}


char telaExcluirMatricula(void) {

    char op;    

    printf("====================\n");
    printf("EXCLUIR MATRICULA\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    return op;
}



