#include <stdio.h>
#include "telasCheckin.h"

char moduloCheckin(void) {

    char opCheckin;

    do {
        opCheckin = telaCheckin();
        switch(opCheckin) {    
        case '1':       telaCadastrarCheckin();
                        break;
                        
        case '2':       telaVisualizarCheckin();
                        break;
            
        case 
        '3':            telaAtualizarCheckin();
                        break;
            
        case 
        '4':            telaExcluirCheckin();
                        break;

        case '0':
                        break;
                        
        default:
                        opInvalida();
                        break;
        }
    
    } 
    while (opCheckin != '0');
 
    return opCheckin;
}



char telaCheckin(void) {

    char op;

    printf("====================\n");
    printf("CHECK-IN\n");
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

char telaCadastrarCheckin(void) {
    
    char op;
    printf("====================\n");
    printf("CADASTRAR CHECKIN\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    return op;
    
}



char telaVisualizarCheckin(void) {

    char op;
    printf("\n");
    printf("====================\n");
    printf("VISUALIZAR CHECKIN\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    
    scanf("%c", &op);
    getchar();

    return op;
    
}


char telaAtualizarCheckin(void) {

    char op;
    printf("====================\n");
    printf("ATUALIZAR CHECKIN \n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");
    
    scanf("%c", &op);
    getchar();

    return op;


}



char telaExcluirCheckin(void) {
    
    char op;    

    
    printf("====================\n");
    printf("EXCLUIR CHECKIN\n");
    printf("====================\n");
    printf("  ...EM BREVE... \n");
    printf("\n");
    printf("<Aperte 0 para sair!>\n");

    scanf("%c", &op);
    getchar();

    return op;
}