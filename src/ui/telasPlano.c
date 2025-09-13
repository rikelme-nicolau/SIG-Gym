#include <stdio.h>
#include "telasPlano.h"

char moduloPlano(void) {

    char opPlano;

    do 
    {
        opPlano = telaPlano();
        switch(opPlano) 
        {    
        case '1':       telaCadastrarPlano();
                        break;
                        
        case '2':       telaVisualizarPlano();
                        break;
            
        case 
        '3':            telaAtualizarPlano();
                        break;
            
        case 
        '4':            telaExcluirPlano();
                        break;
                        
        case '0':
                        break;

        default:
                        opInvalida();
                        break;
        }
    
    } 
    while (opPlano != '0');
 
    return opPlano;
}


#include <stdio.h>

char telaPlano(void) {

char op;

    printf("===================\n");
    printf("PLANO\n");
    printf("===================\n");
    printf("(1) CADASTRAR\n");
    printf("(2) VISUALIZAR\n");
    printf("(3) ATUALIZAR\n");
    printf("(4) EXCLUIR\n");
    printf("(0) SAIR\n");

    scanf("%c", &op);
    getchar();

    return op;
}



char telaCadastrarPlano(void) {
    
    char op;

    printf("====================\n");
    printf("CADASTRAR PLANO\n");
    printf("====================\n");
  
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    return op;
    
}

char telaVisualizarPlano(void) {

    char op;
    
    printf("====================\n");
    printf("VISUALIZAR PLANO\n");
    printf("====================\n");
    printf("<Aperte 0 para sair!>\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    scanf("%c", &op);
    getchar();

    return op;
    
}

char telaAtualizarPlano(void) {

    char op;

    printf("====================\n");
    printf("ATUALIZAR PLANO\n");
    printf("====================\n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    
    scanf("%c", &op);
    getchar();

    return op;
    
}

char telaExcluirPlano(void) {

    char op;    

    printf("====================\n");
    printf("EXCLUIR PLANO\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    scanf("%c", &op);
    getchar();

    return op;
    
}