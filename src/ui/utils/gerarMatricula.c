#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Gera matrícula no formato: AAAATTTxxxxx
// AAAA = ano, TTT = tipo (001, 002), xxxxx = totalmente random
void gerarMatricula(char *saida, const char *tipo)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char ano[5];
    snprintf(ano, sizeof(ano), "%d", tm.tm_year + 1900);

    int numero = rand() % 100000;
    char aleatorio[6];
    snprintf(aleatorio, sizeof(aleatorio), "%05d", numero);

    // monta matricula
    snprintf(saida, 20, "%s%s%s", ano, tipo, aleatorio);
}
