char moduloAluno(void) {

    printf("=================================\n");
    printf("|         MÓDULO ALUNO          |\n");
    printf("=================================\n");
    printf("| 1. Cadastrar Novo Aluno       |\n");
    printf("| 2. Atualizar Dados do Aluno   |\n");
    printf("| 3. Visualizar Dados do Aluno  |\n");
    printf("| 4. Deletar Aluno              |\n");
    printf("| 0. Sair                       |\n");
    printf("=================================\n");

    char op_aluno;
    printf("O que deseja fazer? Escolha uma das opções acima: ");
    scanf(" %c", &op_aluno); 

    return op_aluno;
}
