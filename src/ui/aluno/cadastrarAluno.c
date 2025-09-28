#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limparTela.h"

char* telaCadastrarAluno(void) {
    
    char buffer_nome[1024];
    char buffer_idade[1024];
    char buffer_cpf[1024];
    char buffer_telefone[1024];
    char buffer_endereco[1024];
    char buffer_email[1024];

    printf("\n");
    
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=== Por favor, digite o nome:                                         ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");
    
    fgets(buffer_nome, sizeof(buffer_nome), stdin);
    buffer_nome[strcspn(buffer_nome, "\n")] = '\0';
    char *nome = malloc(strlen(buffer_nome) + 1);
    strcpy(nome, buffer_nome);

    limparTela();
    
    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=== Por favor, digite a data de nascimento:                           ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");
    
    fgets(buffer_idade, sizeof(buffer_idade), stdin);
    buffer_idade[strcspn(buffer_idade, "\n")] = '\0';
    char *idade =  malloc(strlen(buffer_idade) + 1); 
    strcpy(idade, buffer_idade);

    limparTela();

    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=== Por favor, digite o cpf:                                          ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");
    
    fgets(buffer_cpf, sizeof(buffer_cpf), stdin);
    buffer_cpf[strcspn(buffer_cpf, "\n")] = '\0';
    char *cpf =  malloc(strlen(buffer_cpf) + 1); 
    strcpy(cpf, buffer_cpf);

    limparTela();

    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=== Por favor, digite o telefone:                                     ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");
    
    fgets(buffer_telefone, sizeof(buffer_telefone), stdin);
    buffer_telefone[strcspn(buffer_telefone, "\n")] = '\0';
    char *telefone =  malloc(strlen(buffer_telefone) + 1); 
    strcpy(telefone, buffer_telefone);

    limparTela();

    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=== Por favor, digite o endereço:                                     ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");
    
    fgets(buffer_endereco, sizeof(buffer_endereco), stdin);
    buffer_endereco[strcspn(buffer_endereco, "\n")] = '\0';
    char *endereco =  malloc(strlen(buffer_endereco) + 1); 
    strcpy(endereco, buffer_endereco);

    limparTela();

    printf("=========================================================================\n");
    printf("===                        CADASTRAR ALUNO                            ===\n");
    printf("=========================================================================\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=== Por favor, digite o email:                                        ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("===                                                                   ===\n");
    printf("=========================================================================\n");
    printf("=========================================================================\n");
    
    fgets(buffer_email, sizeof(buffer_email), stdin);
    buffer_email[strcspn(buffer_email, "\n")] = '\0';
    char *email =  malloc(strlen(buffer_email) + 1); 
    strcpy(email, buffer_email);

    limparTela();

    return nome, idade, cpf, telefone, endereco, email;
}
