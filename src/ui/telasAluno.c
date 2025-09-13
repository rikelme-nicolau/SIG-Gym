#include <stdio.h>
#include "telasAluno.h"


char moduloAluno(void) {

    char opAluno;
    
    

    do 
    {
        opAluno = telaAluno();
        switch(opAluno) 
        {    
        case '1':       telaCadastrarAluno();
                        break;
                        
        case
        '2':            telaVisualizarAluno();
                        break;
            
        case 
        '3':            telaAtualizarAluno();
                        break;
            
        case 
        '4':            telaExcluirAluno();
                        break;

        case '0':
                        break;
                        
        default:
                        opInvalida();
                        break;
        }
    } 
    while (opAluno != '0');
 
    return opAluno;
}


char telaAluno(void) {

char op;

    printf("\n");
    
    printf("====================\n");
    printf("ALUNO\n");
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





char telaCadastrarAluno(void) {
    
    char op;

    printf("\n");
    printf("==================\n");
    printf("CADASTRAR ALUNO\n");
    printf("==================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    scanf("%c", &op);
    getchar();

    return op;
    }


char telaVisualizarAluno(void) {
    
        char op;
        printf("\n");
        printf("====================\n");
        printf("VISUALIZAR ALUNO \n");
        printf("====================\n");
        printf("  ...EM BREVE... \n");
        printf("\n");
        printf("<Aperte 0 para sair!>\n");
        scanf("%c", &op);
        getchar();
    
        return op;
    
    
    }



char telaAtualizarAluno(void) {

    char op;

    printf("\n");
    printf("====================\n");
    printf(" ATUALIZAR ALUNO\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    scanf("%c", &op);
    getchar();

    

    return op;

    }




char telaExcluirAluno(void) {

    char op;    

    printf("\n");
    printf("====================\n");
    printf("EXCLUIR ALUNO\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    scanf("%c", &op);
    getchar();

    return op;
}