#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cadastrarAluno.h"
#include "arquivoAluno.h"
#include "../utils/geradorNomes.h"
#include "../utils/geradorDados.h"
#include "../funcionario/cadastrarFuncionario.h"
#include "../utils/logGeracao.h"

#define ALUNOS_FILE "alunos.dat"
#define TMP_FILE "alunos.tmp"
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))
#define TOTAL_ALUNOS_FICTICIOS 80

static void preencherPlanosSorteados(char destinos[][12], int total_destinos)
{
    const struct
    {
        const char *id;
        int quantidade;
    } distribuicao_planos[] = {
        // Planos mais baratos (10 cada)
        {"PLAN-012", 10},
        {"PLAN-008", 10},
        {"PLAN-004", 10},
        {"PLAN-010", 10},
        // Planos intermediarios (5-10 cada)
        {"PLAN-007", 6},
        {"PLAN-002", 6},
        {"PLAN-001", 5},
        {"PLAN-005", 6},
        {"PLAN-006", 6},
        // Planos caros (2-5 cada)
        {"PLAN-003", 2},
        {"PLAN-009", 2},
        {"PLAN-011", 2},
        // Sem plano
        {"0", 5},
    };

    int pos = 0;
    for (size_t i = 0; i < ARRAY_LENGTH(distribuicao_planos) && pos < total_destinos; i++)
    {
        for (int j = 0; j < distribuicao_planos[i].quantidade && pos < total_destinos; j++)
        {
            strncpy(destinos[pos], distribuicao_planos[i].id, sizeof(destinos[pos]) - 1);
            destinos[pos][sizeof(destinos[pos]) - 1] = '\0';
            pos++;
        }
    }

    // Embaralha para distribuir aleatoriamente
    for (int i = pos - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        char tmp[12];
        memcpy(tmp, destinos[i], sizeof(tmp));
        memcpy(destinos[i], destinos[j], sizeof(destinos[i]));
        memcpy(destinos[j], tmp, sizeof(tmp));
    }
}

static int preencherAlunosFicticios(struct aluno lista_alunos[])
{
    char planos_sorteio[TOTAL_ALUNOS_FICTICIOS][12];
    preencherPlanosSorteados(planos_sorteio, TOTAL_ALUNOS_FICTICIOS);

    int total = 0;
    for (int i = 0; i < TOTAL_ALUNOS_FICTICIOS && total < MAX_ALUNOS; i++)
    {
        struct aluno novo;
        memset(&novo, 0, sizeof(novo));

        snprintf(novo.id, sizeof(novo.id), "ALU-%04d", i + 1);

        gerarNomeAleatorio(novo.nome, sizeof(novo.nome));

        int tentativas_cpf = 0;
        do
        {
            gerarCPFAleatorio(novo.cpf);
            tentativas_cpf++;
            if (tentativas_cpf > 100)
            {
                printf("ERRO: Nao conseguiu gerar CPF unico para aluno.\n");
                break;
            }
        } while (!verificarCPFUnico(novo.cpf, lista_alunos, total, lista_funcionarios, total_funcionarios));

        gerarTelefoneAleatorio(novo.telefone);
        gerarEnderecoAleatorio(novo.endereco, sizeof(novo.endereco));
        int tentativas_email = 0;
        do
        {
            gerarEmailAleatorio(novo.nome, novo.email);
            tentativas_email++;
            if (tentativas_email > 100)
            {
                printf("ERRO: Nao conseguiu gerar email unico para aluno.\n");
                break;
            }
        } while (!verificarEmailUnico(novo.email, lista_alunos, total, lista_funcionarios, total_funcionarios));
        gerarDataNascimentoAleatoria(novo.idade, 16, 70);

        strncpy(novo.plano_id, planos_sorteio[i], sizeof(novo.plano_id) - 1);
        novo.plano_id[sizeof(novo.plano_id) - 1] = '\0';

        novo.ativo = true;

        lista_alunos[total++] = novo;
    }

    int qtd_inativos = 5 + (rand() % 4); // 5 a 8
    int marcados = 0;
    while (marcados < qtd_inativos && marcados < total)
    {
        int idx = rand() % total;
        if (lista_alunos[idx].ativo)
        {
            lista_alunos[idx].ativo = false;
            marcados++;
        }
    }

    logEtapaGeracao("ALUNOS", total);

    return total;
}

static int gerarAlunosPadrao(struct aluno lista_alunos[])
{
    int total = preencherAlunosFicticios(lista_alunos);
    salvarAlunos(lista_alunos, total);
    return total;
}

// Salva todos os alunos ativos no arquivo binario
void salvarAlunos(struct aluno lista_alunos[], int total_alunos)
{
    FILE *fp = fopen(TMP_FILE, "wb");
    if (!fp)
    {
        perror("Erro ao criar arquivo temporario");
        return;
    }

    for (int i = 0; i < total_alunos; i++)
    {
        if (lista_alunos[i].ativo)
        {
            if (fwrite(&lista_alunos[i], sizeof(struct aluno), 1, fp) != 1)
            {
                perror("Erro ao gravar aluno no arquivo temporario");
                fclose(fp);
                remove(TMP_FILE);
                return;
            }
        }
    }

    if (fflush(fp) != 0 || ferror(fp))
    {
        perror("Erro ao finalizar gravacao do arquivo temporario");
        fclose(fp);
        remove(TMP_FILE);
        return;
    }

    fclose(fp);

    if (rename(TMP_FILE, ALUNOS_FILE) != 0)
    {
        perror("Erro ao substituir arquivo de alunos");
        remove(TMP_FILE);
        return;
    }
}

// Carrega todos os alunos do arquivo binario
int carregarAlunos(struct aluno lista_alunos[])
{
    FILE *fp = fopen(ALUNOS_FILE, "rb");
    if (!fp)
    {
        return gerarAlunosPadrao(lista_alunos);
    }

    long file_size = 0;
    if (fseek(fp, 0, SEEK_END) == 0)
    {
        file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
    }

    if (file_size < 0 || (file_size > 0 && (file_size % (long)sizeof(struct aluno)) != 0))
    {
        fprintf(stderr, "Aviso: arquivo de alunos corrompido; gerando dados padrao.\n");
        fclose(fp);
        return gerarAlunosPadrao(lista_alunos);
    }

    if (file_size == 0)
    {
        fclose(fp);
        return gerarAlunosPadrao(lista_alunos);
    }

    int total = 0;

    while (total < MAX_ALUNOS && fread(&lista_alunos[total], sizeof(struct aluno), 1, fp) == 1)
    {
        total++;
    }

    if (ferror(fp))
    {
        fprintf(stderr, "Erro ao ler arquivo de alunos; gerando dados padrao.\n");
        fclose(fp);
        return gerarAlunosPadrao(lista_alunos);
    }

    fclose(fp);

    if (total == 0)
    {
        return gerarAlunosPadrao(lista_alunos);
    }

    return total;
}

// Atualiza um aluno especifico no arquivo
void atualizarAlunoNoArquivo(struct aluno aluno)
{
    struct aluno alunos[MAX_ALUNOS];
    int total = carregarAlunos(alunos);

    for (int i = 0; i < total; i++)
    {
        if (strcmp(alunos[i].id, aluno.id) == 0)
        {
            alunos[i] = aluno;
            break;
        }
    }

    salvarAlunos(alunos, total);
}

// Marca um aluno como excluido (exclusao logica)
void excluirAluno(char *id)
{
    struct aluno alunos[MAX_ALUNOS];
    int total = carregarAlunos(alunos);

    for (int i = 0; i < total; i++)
    {
        if (strcmp(alunos[i].id, id) == 0)
        {
            alunos[i].ativo = false;
            break;
        }
    }

    salvarAlunos(alunos, total);
}
