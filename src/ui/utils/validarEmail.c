#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

// --- Constantes Configuráveis ---
#define TAMANHO_MIN_EMAIL 7
#define TAMANHO_MAX_EMAIL 254

bool validarEmail(const char *email) {
    // 1. Verificação de ponteiro nulo e comprimento
    if (email == NULL) {
        return false;
    }
    size_t len = strlen(email);
    if (len < TAMANHO_MIN_EMAIL || len > TAMANHO_MAX_EMAIL) {
        return false;
    }

    // 2. Procura pelo caractere '@'
    char *arroba = strchr(email, '@');
    if (arroba == NULL) {
        return false; // Não encontrou o '@'
    }
    
    // Verifica se existe mais de um '@'
    if (strchr(arroba + 1, '@') != NULL) {
        return false; // Encontrou um segundo '@'
    }

    // 3. Valida a parte local (antes do '@')
    ptrdiff_t pos_arroba = arroba - email;
    if (pos_arroba == 0) {
        return false; // O '@' não pode ser o primeiro caractere
    }
    
    // 4. Valida a parte do domínio (depois do '@')
    char *dominio = arroba + 1;
    if (strlen(dominio) < 3) {
        return false; // Domínio muito curto (ex: "a.b")
    }
    
    // Procura por um ponto no domínio
    char *ponto = strchr(dominio, '.');
    if (ponto == NULL || ponto == dominio || ponto[1] == '\0') {
        // - Não encontrou ponto
        // - O ponto é o primeiro caractere do domínio (ex: "@.com")
        // - O ponto é o último caractere do domínio (ex: "@site.")
        return false;
    }

    // 5. Verifica caracteres inválidos (como espaços) na string inteira
    for (size_t i = 0; i < len; ++i) {
        if (isspace(email[i])) {
            return false;
        }
    }

    // Se passou por todas as verificações, o e-mail é considerado válido
    return true;
}