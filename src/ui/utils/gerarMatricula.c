#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "gerarMatricula.h"

// acessando listas globais
#include "src/ui/aluno/arquivoAluno.h"
#include "src/ui/funcionario/arquivoFuncionario.h"

// tipo = "001" para funcionário, "002" para aluno
char *gerarMatricula(const char *tipo)
{
    static char matricula[20];
    bool existe;
    int ano;

    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    ano = tm_info->tm_year + 1900;

    do
    {
        // gera número aleatório de 5 dígitos
        int random = rand() % 100000;

        snprintf(matricula, sizeof(matricula), "%d%s%05d", ano, tipo, random);

        // verifica se já existe
        existe = false;

        // ver alunos
        for (int i = 0; i < total_alunos; i++)
        {
            if (strcmp(lista_alunos[i].id, matricula) == 0)
            {
                existe = true;
                break;
            }
        }

        // checa em funcionarios
        if (!existe)
        {
            for (int i = 0; i < total_funcionarios; i++)
            {
                if (strcmp(lista_funcionarios[i].id, matricula) == 0)
                {
                    existe = true;
                    break;
                }
            }
        }

    } while (existe);

    return matricula;
}
