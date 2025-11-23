#include <stdio.h>
#include <stdlib.h>

#include "geradorNomes.h"

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

static const char *nomes_masculinos[] = {
    "João",     "Pedro",     "Lucas",      "Gabriel",   "Matheus",   "Rafael",     "Felipe",      "Bruno",    "Gustavo", "André",
    "Carlos",   "Ricardo",   "Fernando",   "Roberto",   "Paulo",     "José",       "Antônio",     "Diego",    "Victor",  "Thiago",
    "Daniel",   "Rodrigo",   "Marcos",     "Eduardo",   "Vinícius",  "Leonardo",   "Caio",        "Guilherme","Luiz",    "Alexandre",
    "Henrique", "Sérgio",    "Maurício",   "Miguel",    "Vitor",     "Tiago",      "Fábio",       "Augusto",  "Hugo",    "Igor",
    "Samuel",   "Renato",    "Wellington", "Cláudio",   "Jorge",     "Renan",      "Erick",       "Ruan",     "César",   "Allan"};

static const char *nomes_femininos[] = {
    "Maria",     "Ana",       "Júlia",    "Beatriz",  "Carolina",  "Fernanda", "Camila",   "Amanda",   "Juliana",  "Patrícia",
    "Mariana",   "Gabriela",  "Isabela",  "Laura",    "Letícia",   "Bianca",   "Aline",    "Paula",    "Sofia",    "Luana",
    "Larissa",   "Raíssa",    "Bruna",    "Tatiane",  "Carla",     "Priscila", "Natália",  "Nicole",   "Eduarda",  "Alice",
    "Valentina", "Helena",    "Renata",   "Flávia",   "Talita",    "Catarina", "Melissa",  "Isadora",  "Daniela",  "Cristina",
    "Elaine",    "Michele",   "Viviane",  "Andressa", "Simone",    "Débora",   "Regina",   "Tânia",    "Giovana",  "Yasmin"};

static const char *sobrenomes[] = {
    "Silva",      "Santos",     "Oliveira",   "Souza",       "Costa",        "Ferreira",   "Rodrigues", "Almeida",   "Nascimento", "Lima",
    "Araújo",     "Fernandes",  "Carvalho",   "Gomes",       "Martins",      "Rocha",      "Ribeiro",   "Alves",     "Pereira",    "Barbosa",
    "Cardoso",    "Moura",      "Barros",     "Dias",        "Teixeira",     "Correia",    "Moreira",   "Marques",   "Vieira",     "Cavalcanti",
    "Monteiro",   "Melo",       "Medeiros",   "Cunha",       "Batista",      "Freitas",    "Farias",    "Gonçalves", "Pires",      "Ramos",
    "Campos",     "Peixoto",    "Rezende",    "Assis",       "Siqueira",     "Queiroz",    "Tavares",   "Sales",     "Azevedo",    "Xavier",
    "Neves",      "Coelho",     "Figueiredo", "Coutinho",    "Duarte",       "Guimarães",  "Fonseca",   "Braga",     "Leal",       "Rangel",
    "Aguiar",     "Macedo",     "Borges",     "Castro",      "Moraes",       "Machado",    "Miranda",   "Nunes",     "Rosa",       "Valente",
    "Domingues",  "Ventura",    "Brito",      "Porto",       "Prado",        "Lacerda",    "Lopes",     "Torres",    "Mendes",     "Viana",
    "Serra",      "Barreto",    "Andrade",    "Santiago",    "Falcão",       "Pontes",     "Leme",      "Parente",   "Antunes",    "Pimentel",
    "Guerra",     "Dornelles",  "Bastos",     "Magalhães",   "Holanda",      "Dutra",      "Amorim",    "Meireles",  "Quevedo",    "Linhares"};

static const char *sortearPrimeiroNome(void)
{
    if (rand() % 2 == 0)
    {
        return nomes_masculinos[rand() % ARRAY_LENGTH(nomes_masculinos)];
    }

    return nomes_femininos[rand() % ARRAY_LENGTH(nomes_femininos)];
}

static size_t sortearSobrenome(void)
{
    return (size_t)(rand() % ARRAY_LENGTH(sobrenomes));
}

void gerarNomeAleatorio(char *destino, size_t tamanho)
{
    if (destino == NULL || tamanho == 0)
    {
        return;
    }

    const char *primeiro_nome = sortearPrimeiroNome();
    size_t quantidade_sobrenomes = (size_t)((rand() % 2) + 1);
    size_t indice_sobrenome1 = sortearSobrenome();
    size_t indice_sobrenome2 = sortearSobrenome();

    if (quantidade_sobrenomes == 1)
    {
        snprintf(destino, tamanho, "%s %s", primeiro_nome, sobrenomes[indice_sobrenome1]);
        return;
    }

    while (indice_sobrenome2 == indice_sobrenome1)
    {
        indice_sobrenome2 = sortearSobrenome();
    }

    snprintf(destino, tamanho, "%s %s %s", primeiro_nome, sobrenomes[indice_sobrenome1], sobrenomes[indice_sobrenome2]);
}

void gerarNomeCompletoAleatorio(char *destino, size_t tamanho)
{
    if (destino == NULL || tamanho == 0)
    {
        return;
    }

    const char *primeiro_nome = sortearPrimeiroNome();
    size_t indice_sobrenome1 = sortearSobrenome();
    size_t indice_sobrenome2 = sortearSobrenome();

    while (indice_sobrenome2 == indice_sobrenome1)
    {
        indice_sobrenome2 = sortearSobrenome();
    }

    snprintf(destino, tamanho, "%s %s %s", primeiro_nome, sobrenomes[indice_sobrenome1], sobrenomes[indice_sobrenome2]);
}
