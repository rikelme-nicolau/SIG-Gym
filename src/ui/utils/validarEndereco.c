#include <stdbool.h>
#include <string.h>
#include <ctype.h> // Para isalnum() e isspace()

// --- Constantes Configuráveis ---
#define TAMANHO_MIN_ENDERECO 10
#define TAMANHO_MAX_ENDERECO 150

bool validarEndereco(const char *endereco) {
    // 1. Verificação de ponteiro nulo
    if (endereco == NULL) {
        return false;
    }

    // 2. Validação de comprimento
    size_t len = strlen(endereco);
    if (len < TAMANHO_MIN_ENDERECO || len > TAMANHO_MAX_ENDERECO) {
        return false;
    }

    // 3. Verifica se começa ou termina com espaço
    if (isspace((unsigned char)endereco[0]) || isspace((unsigned char)endereco[len - 1])) {
        return false;
    }

    // 4. Itera pela string para validar cada caractere
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = endereco[i];

        // 4.1. Verifica caracteres permitidos
        // isalnum() verifica se é letra OU número.
        // Adicionamos os caracteres de pontuação permitidos.
        if (!isalnum(c) && !isspace(c) && c != ',' && c != '-' && c != '.' && c != '/') {
            return false; // Se não for nenhum dos permitidos, é inválido
        }

        // 4.2. Verifica espaços consecutivos
        if (isspace(c) && (i + 1 < len) && isspace((unsigned char)endereco[i + 1])) {
            return false;
        }
    }

    // Se passou por todas as verificações, o endereço é válido
    return true;
}