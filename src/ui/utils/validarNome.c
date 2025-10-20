#include "validarNome.h"
#include <string.h> // Para strlen()
#include <ctype.h>  // Para isalpha(), isspace()
#include <locale.h> // Para setlocale(), essencial para caracteres acentuados

bool validarNome(const char *nome) {
    // 1. Verifica se o ponteiro é nulo
    if (nome == NULL) {
        return false;
    }

    size_t len = strlen(nome);

    // 2. Validação de comprimento
    if (len < TAMANHO_MIN_NOME || len > TAMANHO_MAX_NOME) {
        return false;
    }

    // 3. Verifica se começa ou termina com espaço
    // isspace() verifica espaços, tabs, newlines, etc.
    if (isspace((unsigned char)nome[0]) || isspace((unsigned char)nome[len - 1])) {
        return false;
    }

    // 4. Itera pela string para validar cada caractere
    for (size_t i = 0; i < len; ++i) {
        // Converte para unsigned char para evitar comportamento indefinido com caracteres negativos
        unsigned char c = nome[i];

        // 4.1. Verifica caracteres permitidos: apenas letras e espaços
        // A função isalpha() só funciona para acentos se o locale estiver configurado!
        if (!isalpha(c) && !isspace(c)) {
            return false;
        }

        // 4.2. Verifica espaços consecutivos
        if (isspace(c) && (i + 1 < len) && isspace((unsigned char)nome[i + 1])) {
            return false;
        }
    }

    // Se passou por todas as verificações, o nome é válido
    return true;
}