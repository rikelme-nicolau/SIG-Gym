#include <stdlib.h>
#include <string.h>

#include "distribuicaoAleatoria.h"

#define PERCENT(total, pct) (((total) * (pct) + 50) / 100)

void distribuirAlunosEntrePlanos(int totalAlunos, int totalPlanos, int *resultado)
{
    if (resultado == NULL || totalPlanos <= 0)
    {
        return;
    }

    for (int i = 0; i < totalPlanos; i++)
    {
        resultado[i] = 0;
    }

    if (totalAlunos <= 0)
    {
        return;
    }

    int restante = totalAlunos;

    if (totalPlanos >= 1)
    {
        int qtd = PERCENT(totalAlunos, 15);
        if (qtd > restante)
        {
            qtd = restante;
        }
        resultado[0] = qtd;
        restante -= qtd;
    }

    if (totalPlanos >= 2 && restante > 0)
    {
        int qtd = PERCENT(totalAlunos, 12);
        if (qtd > restante)
        {
            qtd = restante;
        }
        resultado[1] = qtd;
        restante -= qtd;
    }

    if (totalPlanos >= 3 && restante > 0)
    {
        int qtd = PERCENT(totalAlunos, 10);
        if (qtd > restante)
        {
            qtd = restante;
        }
        resultado[2] = qtd;
        restante -= qtd;
    }

    int planos_restantes = totalPlanos - 3;
    if (planos_restantes > 0 && restante > 0)
    {
        int peso_total = planos_restantes * (planos_restantes + 1) / 2; // soma 1..n
        int alocados = 0;

        for (int i = 0; i < planos_restantes; i++)
        {
            int peso = planos_restantes - i; // decrescente
            int qtd = (restante * peso) / peso_total;
            resultado[3 + i] = qtd;
            alocados += qtd;
        }

        int sobra = restante - alocados;
        for (int i = 0; i < planos_restantes && sobra > 0; i++, sobra--)
        {
            resultado[3 + i]++;
        }
        restante = 0;
    }

    if (restante > 0)
    {
        resultado[0] += restante;
    }
}

int selecionarPlanoAleatorioComPeso(void)
{
    int sorteio = rand() % 101; // 0 a 100

    if (sorteio <= 20)
    {
        return 1;
    }
    if (sorteio <= 35)
    {
        return 2;
    }
    if (sorteio <= 45)
    {
        return 3;
    }
    if (sorteio <= 55)
    {
        return 4 + (rand() % 3); // 4-6
    }
    if (sorteio <= 70)
    {
        return 7 + (rand() % 3); // 7-9
    }
    if (sorteio <= 85)
    {
        return 10 + (rand() % 3); // 10-12
    }

    return 0; // sem plano
}
