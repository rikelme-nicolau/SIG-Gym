#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "resetarDados.h"
#include "../aluno/arquivoAluno.h"
#include "../plano/arquivoPlano.h"
#include "../funcionario/arquivoFuncionario.h"
#include "../equipamento/arquivoEquipamento.h"
#include "../aluno/cadastrarAluno.h"
#include "../plano/cadastrarPlano.h"
#include "../funcionario/cadastrarFuncionario.h"
#include "../equipamento/cadastrarEquipamento.h"
#include "../utils/logGeracao.h"

bool resetarTodosDados(void)
{
    bool sucesso = true;

    const char *arquivos[] = {"alunos.dat", "planos.dat", "funcionarios.dat", "equipamentos.dat"};

    for (size_t i = 0; i < sizeof(arquivos) / sizeof(arquivos[0]); i++)
    {
        if (remove(arquivos[i]) != 0)
        {
            FILE *f = fopen(arquivos[i], "rb");
            if (f != NULL)
            {
                fclose(f);
                sucesso = false;
                fprintf(stderr, "Falha ao remover arquivo de dados: %s\n", arquivos[i]);
            }
        }
    }

    if (!sucesso)
    {
        return false;
    }

    memset(lista_alunos, 0, sizeof(lista_alunos));
    memset(lista_planos, 0, sizeof(lista_planos));
    memset(lista_funcionarios, 0, sizeof(lista_funcionarios));
    memset(lista_equipamentos, 0, sizeof(lista_equipamentos));

    total_alunos = 0;
    total_planos = 0;
    total_funcionarios = 0;
    total_equipamentos = 0;

    return sucesso;
}

void regerarDadosFicticios(void)
{
    if (!resetarTodosDados())
    {
        printf("Falha ao resetar dados.\n");
        return;
    }

    logInicioGeracao();

    total_planos = carregarPlanos(lista_planos);
    total_equipamentos = carregarEquipamentos(lista_equipamentos);
    total_funcionarios = carregarFuncionarios(lista_funcionarios);
    total_alunos = carregarAlunos(lista_alunos);

    printf("Dados regenerados com sucesso!\n");

    logFimGeracao();
}
