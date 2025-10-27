#include <stdbool.h>
#include <stdio.h>  // Para sscanf
#include <string.h> // Para strlen
#include <time.h>   // Para obter a data atual

bool validarNascimento(const char *data_nasc)
{
    // 1. Verificação de ponteiro nulo e formato básico (DD/MM/AAAA)
    if (data_nasc == NULL || strlen(data_nasc) != 10)
    {
        return false;
    }
    if (data_nasc[2] != '/' || data_nasc[5] != '/')
    {
        return false;
    }

    // 2. Extrai dia, mês e ano da string
    int dia, mes, ano;
    if (sscanf(data_nasc, "%d/%d/%d", &dia, &mes, &ano) != 3)
    {
        return false; // Garante que foram lidos 3 números
    }

    // 3. Valida o intervalo do ano e do mês
    if (ano < 1900 || mes < 1 || mes > 12)
    {
        return false;
    }

    // 4. Valida o dia, considerando o mês e anos bissextos
    int dias_no_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Lógica do ano bissexto
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
    {
        dias_no_mes[2] = 29; // Fevereiro tem 29 dias
    }

    if (dia < 1 || dia > dias_no_mes[mes])
    {
        return false;
    }

    // 5. Verifica se a data de nascimento não é uma data futura
    time_t tempo_atual = time(NULL);
    struct tm *data_atual = localtime(&tempo_atual);

    int ano_atual = data_atual->tm_year + 1900;
    int mes_atual = data_atual->tm_mon + 1;
    int dia_atual = data_atual->tm_mday;

    if (ano > ano_atual)
    {
        return false;
    }
    if (ano == ano_atual && mes > mes_atual)
    {
        return false;
    }
    if (ano == ano_atual && mes == mes_atual && dia > dia_atual)
    {
        return false;
    }

    // Se passou por todas as verificações, a data é válida
    return true;
}

int calcularIdade(const char *nascimento)
{
    int dia, mes, ano;
    if (sscanf(nascimento, "%2d/%2d/%4d", &dia, &mes, &ano) != 3)
        return 0; // retorno 0 se não conseguir ler

    time_t t = time(NULL);
    struct tm tm_atual = *localtime(&t);

    int idade = tm_atual.tm_year + 1900 - ano;

    if (tm_atual.tm_mon + 1 < mes || (tm_atual.tm_mon + 1 == mes && tm_atual.tm_mday < dia))
    {
        idade--; // ainda não fez aniversário
    }

    return idade;
}
