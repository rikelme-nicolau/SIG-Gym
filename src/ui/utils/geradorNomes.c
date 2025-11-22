#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geradorNomes.h"

static const char *nomes_masculinos[] = {
    "João", "Pedro", "Lucas", "Gabriel", "Matheus", "Rafael", "Felipe", "Bruno", "Gustavo", "André",
    "Carlos", "Ricardo", "Fernando", "Roberto", "Paulo", "José", "Antônio", "Diego", "Victor", "Thiago",
    "Daniel", "Rodrigo", "Marcos", "Eduardo", "Vinícius", "Leonardo", "Caio", "Guilherme", "Luiz", "Alexandre",
    "Henrique", "Sérgio", "Maurício", "Miguel", "Vitor", "Tiago", "Fábio", "Augusto", "Hugo", "Igor",
    "Samuel", "Renato", "Wellington", "Cláudio", "Jorge", "Renan", "Erick", "Ruan", "César", "Allan"};

static const char *nomes_femininos[] = {
    "Maria", "Ana", "Julia", "Beatriz", "Carolina", "Fernanda", "Camila", "Amanda", "Juliana", "Patricia",
    "Mariana", "Gabriela", "Isabela", "Laura", "Letícia", "Bianca", "Aline", "Paula", "Sofia", "Luana",
    "Larissa", "Raissa", "Bruna", "Tatiane", "Carla", "Priscila", "Natália", "Nicole", "Eduarda", "Alice",
    "Valentina", "Helena", "Renata", "Flávia", "Talita", "Catarina", "Melissa", "Isadora", "Daniela", "Cristina",
    "Elaine", "Michele", "Viviane", "Andressa", "Simone", "Débora", "Regina", "Tânia", "Giovana", "Yasmin"};

static const char *sobrenomes[] = {
    "Silva", "Santos", "Oliveira", "Souza", "Costa", "Ferreira", "Rodrigues", "Almeida", "Nascimento", "Lima",
    "Araújo", "Fernandes", "Carvalho", "Gomes", "Martins", "Rocha", "Ribeiro", "Alves", "Pereira", "Barbosa",
    "Cardoso", "Moura", "Barros", "Dias", "Teixeira", "Correia", "Moreira", "Marques", "Vieira", "Cavalcanti",
    "Monteiro", "Melo", "Medeiros", "Cunha", "Batista", "Freitas", "Farias", "Gonçalves", "Pires", "Ramos",
    "Campos", "Peixoto", "Rezende", "Assis", "Siqueira", "Queiroz", "Tavares", "Sales", "Azevedo", "Xavier",
    "Neves", "Coelho", "Figueiredo", "Coutinho", "Duarte", "Guimarães", "Fonseca", "Braga", "Leal", "Rangel",
    "Aguiar", "Macedo", "Borges", "Castro", "Moraes", "Machado", "Miranda", "Nunes", "Rosa", "Valente",
    "Domingues", "Ventura", "Brito", "Porto", "Prado", "Lacerda", "Lopes", "Torres", "Mendes", "Viana",
    "Serra", "Barreto", "Andrade", "Santiago", "Falcão", "Pontes", "Leme", "Parente", "Antunes", "Pimentel",
    "Guerra", "Dorneles", "Bastos", "Magalhães", "Holanda", "Dutra", "Amorim", "Meireles", "Quevedo", "Linhares"};

void gerarNomeAleatorio(char *destino, size_t tamanho)
{
    if (destino == NULL || tamanho == 0)
    {
        return;
    }

    const char *primeiro_nome;
    if (rand() % 2 == 0)
    {
        primeiro_nome = nomes_masculinos[rand() % (sizeof(nomes_masculinos) / sizeof(nomes_masculinos[0]))];
    }
    else
    {
        primeiro_nome = nomes_femininos[rand() % (sizeof(nomes_femininos) / sizeof(nomes_femininos[0]))];
    }

    int total_sobrenomes = (rand() % 2) + 1;
    int indice_sobrenome1 = rand() % (sizeof(sobrenomes) / sizeof(sobrenomes[0]));
    int indice_sobrenome2 = rand() % (sizeof(sobrenomes) / sizeof(sobrenomes[0]));

    if (total_sobrenomes == 1)
    {
        snprintf(destino, tamanho, "%s %s", primeiro_nome, sobrenomes[indice_sobrenome1]);
    }
    else
    {
        while (indice_sobrenome2 == indice_sobrenome1)
        {
            indice_sobrenome2 = rand() % (sizeof(sobrenomes) / sizeof(sobrenomes[0]));
        }
        snprintf(destino, tamanho, "%s %s %s", primeiro_nome, sobrenomes[indice_sobrenome1], sobrenomes[indice_sobrenome2]);
    }
}

void gerarNomeCompletoAleatorio(char *destino, size_t tamanho)
{
    if (destino == NULL || tamanho == 0)
    {
        return;
    }

    const char *primeiro_nome;
    if (rand() % 2 == 0)
    {
        primeiro_nome = nomes_masculinos[rand() % (sizeof(nomes_masculinos) / sizeof(nomes_masculinos[0]))];
    }
    else
    {
        primeiro_nome = nomes_femininos[rand() % (sizeof(nomes_femininos) / sizeof(nomes_femininos[0]))];
    }

    int indice_sobrenome1 = rand() % (sizeof(sobrenomes) / sizeof(sobrenomes[0]));
    int indice_sobrenome2 = rand() % (sizeof(sobrenomes) / sizeof(sobrenomes[0]));

    while (indice_sobrenome2 == indice_sobrenome1)
    {
        indice_sobrenome2 = rand() % (sizeof(sobrenomes) / sizeof(sobrenomes[0]));
    }

    snprintf(destino, tamanho, "%s %s %s", primeiro_nome, sobrenomes[indice_sobrenome1], sobrenomes[indice_sobrenome2]);
}
