#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "geradorDados.h"
#include "validarNascimento.h"
#include "../aluno/cadastrarAluno.h"
#include "../funcionario/cadastrarFuncionario.h"

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))
#define SEGUNDOS_DIA 86400

static int calcularDigitoCPF(const int numeros[], size_t tamanho_base, int peso_inicial)
{
    int soma = 0;
    for (size_t i = 0; i < tamanho_base; ++i)
    {
        soma += numeros[i] * (peso_inicial - (int)i);
    }

    int digito = (soma * 10) % 11;
    return (digito == 10) ? 0 : digito;
}

static char normalizarCaractere(unsigned char c)
{
    switch (c)
    {
    case 0xC0:
    case 0xC1:
    case 0xC2:
    case 0xC3:
    case 0xC4:
    case 0xE0:
    case 0xE1:
    case 0xE2:
    case 0xE3:
    case 0xE4:
        return 'a';
    case 0xC7:
    case 0xE7:
        return 'c';
    case 0xC8:
    case 0xC9:
    case 0xCA:
    case 0xCB:
    case 0xE8:
    case 0xE9:
    case 0xEA:
    case 0xEB:
        return 'e';
    case 0xCC:
    case 0xCD:
    case 0xCE:
    case 0xCF:
    case 0xEC:
    case 0xED:
    case 0xEE:
    case 0xEF:
        return 'i';
    case 0xD2:
    case 0xD3:
    case 0xD4:
    case 0xD5:
    case 0xD6:
    case 0xF2:
    case 0xF3:
    case 0xF4:
    case 0xF5:
    case 0xF6:
        return 'o';
    case 0xD9:
    case 0xDA:
    case 0xDB:
    case 0xDC:
    case 0xF9:
    case 0xFA:
    case 0xFB:
    case 0xFC:
        return 'u';
    case 0xD1:
    case 0xF1:
        return 'n';
    default:
        break;
    }

    if (isalnum(c))
    {
        return (char)c;
    }

    return 0;
}

static int diasNoMes(int mes, int ano)
{
    static const int dias_padrao[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dias = dias_padrao[mes];

    int bissexto = ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0));
    if (mes == 2 && bissexto)
    {
        dias = 29;
    }

    return dias;
}

static int preencherData(const char *data_str, struct tm *saida)
{
    int dia, mes, ano;
    if (sscanf(data_str, "%2d/%2d/%4d", &dia, &mes, &ano) != 3)
    {
        return 0;
    }

    if (mes < 1 || mes > 12)
    {
        return 0;
    }

    if (dia < 1 || dia > diasNoMes(mes, ano))
    {
        return 0;
    }

    memset(saida, 0, sizeof(struct tm));
    saida->tm_mday = dia;
    saida->tm_mon = mes - 1;
    saida->tm_year = ano - 1900;
    saida->tm_isdst = -1;

    return 1;
}

static void adicionarDias(struct tm *data, int dias)
{
    data->tm_mday += dias;
    mktime(data);
}

static long sortearIntervalo(long max_inclusivo)
{
    if (max_inclusivo <= 0)
    {
        return 0;
    }

    double fator = (double)rand() / ((double)RAND_MAX + 1.0);
    return (long)(fator * (double)(max_inclusivo + 1));
}

bool verificarCPFUnico(const char *cpf,
                       const struct aluno *alunos, int totalAlunos,
                       const struct funcionario *funcs, int totalFuncs)
{
    if (cpf == NULL)
    {
        return false;
    }

    if (alunos != NULL)
    {
        for (int i = 0; i < totalAlunos; i++)
        {
            if (strcmp(alunos[i].cpf, cpf) == 0)
            {
                return false;
            }
        }
    }

    if (funcs != NULL)
    {
        for (int i = 0; i < totalFuncs; i++)
        {
            if (strcmp(funcs[i].cpf, cpf) == 0)
            {
                return false;
            }
        }
    }

    return true;
}

bool verificarEmailUnico(const char *email,
                         const struct aluno *alunos, int totalAlunos,
                         const struct funcionario *funcs, int totalFuncs)
{
    if (email == NULL)
    {
        return false;
    }

    if (alunos != NULL)
    {
        for (int i = 0; i < totalAlunos; i++)
        {
            if (strcmp(alunos[i].email, email) == 0)
            {
                return false;
            }
        }
    }

    if (funcs != NULL)
    {
        for (int i = 0; i < totalFuncs; i++)
        {
            if (strcmp(funcs[i].email, email) == 0)
            {
                return false;
            }
        }
    }

    return true;
}

void gerarCPFAleatorio(char *destino)
{
    int numeros[11];

    for (size_t i = 0; i < 9; ++i)
    {
        numeros[i] = rand() % 10;
    }

    numeros[9] = calcularDigitoCPF(numeros, 9, 10);
    numeros[10] = calcularDigitoCPF(numeros, 10, 11);

    snprintf(destino, 15, "%d%d%d.%d%d%d.%d%d%d-%d%d",
             numeros[0], numeros[1], numeros[2],
             numeros[3], numeros[4], numeros[5],
             numeros[6], numeros[7], numeros[8],
             numeros[9], numeros[10]);
}

void gerarTelefoneAleatorio(char *destino)
{
    int ddd = 11 + (rand() % 89);
    int usar_celular = rand() % 2;

    if (usar_celular)
    {
        int prefixo = 90000 + (rand() % 10000);
        int sufixo = rand() % 10000;
        snprintf(destino, 20, "(%02d) %05d-%04d", ddd, prefixo, sufixo);
        return;
    }

    int primeiro_digito = 2 + (rand() % 7);
    int prefixo = (primeiro_digito * 1000) + (rand() % 1000);
    int sufixo = rand() % 10000;
    snprintf(destino, 20, "(%02d) %04d-%04d", ddd, prefixo, sufixo);
}

void gerarEmailAleatorio(const char *nome, char *destino)
{
    static const char *dominios[] = {
        "@gmail.com",
        "@hotmail.com",
        "@outlook.com",
        "@yahoo.com"};

    char base[128] = {0};
    size_t j = 0;

    for (size_t i = 0; nome != NULL && nome[i] != '\0' && j < sizeof(base) - 1; ++i)
    {
        unsigned char c = (unsigned char)nome[i];
        if (c == ' ' || c == '\t')
        {
            if (j > 0 && base[j - 1] != '.')
            {
                base[j++] = '.';
            }
            continue;
        }

        char normalizado = normalizarCaractere(c);
        if (normalizado == 0)
        {
            continue;
        }

        base[j++] = (char)tolower((unsigned char)normalizado);
    }

    while (j > 0 && base[j - 1] == '.')
    {
        base[--j] = '\0';
    }

    if (j == 0)
    {
        strcpy(base, "usuario");
    }

    int sufixo = 10 + (rand() % 9990);
    const char *dominio = dominios[rand() % ARRAY_LENGTH(dominios)];

    snprintf(destino, 256, "%s%d%s", base, sufixo, dominio);
}

void gerarEnderecoAleatorio(char *destino, size_t tamanho)
{
    static const char *tipos[] = {
        "Rua", "Avenida", "Travessa", "Alameda", "Praca",
        "Rodovia", "Estrada", "Viela", "Largo", "Beco"};

    static const char *nomes_rua[] = {
        "das Flores", "das Palmeiras", "Principal", "do Comercio",
        "Sao Joao", "Sao Paulo", "Sete de Setembro", "XV de Novembro",
        "Presidente Vargas", "Dom Pedro II", "Santos Dumont", "Rio Branco",
        "Tiradentes", "Marechal Deodoro", "das Acacias", "dos Coqueiros",
        "das Andorinhas", "do Sol", "da Paz", "da Liberdade",
        "da Independencia", "dos Girassois", "das Laranjeiras", "das Hortensias",
        "dos Jacarandas", "das Orquideas", "dos Jasmins", "das Azaleias",
        "dos Pinheiros", "dos Cedros", "das Oliveiras", "das Tulipas",
        "das Cerejeiras", "das Nogueiras", "das Araras", "dos Bem-te-vis",
        "dos Bandeirantes", "das Estrelas", "do Farol", "da Praia",
        "da Serra", "do Horizonte", "das Rochas", "das Fontes",
        "dos Ventos", "da Lagoa", "do Bosque", "Industrial",
        "da Colina", "das Pedras"};

    static const char *bairros[] = {
        "Centro", "Jardim Europa", "Vila Nova", "Bela Vista",
        "Cidade Jardim", "Alto da Boa Vista", "Lagoa", "Copacabana",
        "Ipanema", "Leblon", "Botafogo", "Flamengo", "Tijuca",
        "Vila Mariana", "Moema", "Pinheiros", "Perdizes",
        "Saude", "Santana", "Tatuape", "Penha",
        "Lapa", "Barra da Tijuca", "Recreio", "Campo Grande",
        "Grajaú", "Vila Prudente", "Brooklin", "Morumbi", "Aclimacao", "Pacaembu"};

    if (destino == NULL || tamanho == 0)
    {
        return;
    }

    const char *tipo = tipos[rand() % 10];
    const char *nome_rua = nomes_rua[rand() % 50];
    const char *bairro = bairros[rand() % 30];
    int numero = 10 + (rand() % 9990);

    snprintf(destino, tamanho, "%s %s, %d - %s", tipo, nome_rua, numero, bairro);
}

void gerarDataNascimentoAleatoria(char *destino, int idadeMin, int idadeMax)
{
    if (destino == NULL)
    {
        return;
    }

    if (idadeMin < 0)
    {
        idadeMin = 0;
    }

    if (idadeMax < idadeMin)
    {
        int tmp = idadeMin;
        idadeMin = idadeMax;
        idadeMax = tmp;
    }

    time_t agora = time(NULL);
    struct tm inicio = *localtime(&agora);
    struct tm fim = inicio;

    inicio.tm_year -= idadeMax;
    fim.tm_year -= idadeMin;
    time_t t_inicio = mktime(&inicio);
    time_t t_fim = mktime(&fim);

    if (t_inicio > t_fim)
    {
        time_t tmp = t_inicio;
        t_inicio = t_fim;
        t_fim = tmp;
    }

    long dias_intervalo = (long)((t_fim - t_inicio) / SEGUNDOS_DIA);
    if (dias_intervalo < 0)
    {
        dias_intervalo = 0;
    }

    int tentativas = 0;
    do
    {
        long deslocamento = sortearIntervalo(dias_intervalo);
        time_t selecionada = t_inicio + (deslocamento * SEGUNDOS_DIA);
        struct tm data_escolhida = *localtime(&selecionada);

        strftime(destino, 11, "%d/%m/%Y", &data_escolhida);
        tentativas++;
    } while (!validarNascimento(destino) && tentativas < 5);

    if (!validarNascimento(destino))
    {
        strcpy(destino, "01/01/2000");
    }
}

void gerarDataManutencaoAleatoria(char *destino, int diasMin, int diasMax)
{
    if (destino == NULL)
    {
        return;
    }

    if (diasMin > diasMax)
    {
        int tmp = diasMin;
        diasMin = diasMax;
        diasMax = tmp;
    }

    int amplitude = diasMax - diasMin;
    long sorteio = sortearIntervalo(amplitude);
    int deslocamento = diasMin + (int)sorteio;

    time_t agora = time(NULL);
    struct tm data = *localtime(&agora);
    adicionarDias(&data, deslocamento);

    strftime(destino, 11, "%d/%m/%Y", &data);
}

void calcularProximaManutencaoAleatoria(const char *ultima, char *proxima)
{
    if (proxima == NULL)
    {
        return;
    }

    struct tm data_tm;
    if (!preencherData(ultima, &data_tm))
    {
        strcpy(proxima, "Invalida");
        return;
    }

    int adicionar = 30 + (int)sortearIntervalo(150);
    adicionarDias(&data_tm, adicionar);

    strftime(proxima, 11, "%d/%m/%Y", &data_tm);
}
