#ifndef VALIDARNOME_H
#define VALIDARNOME_H

#include <stdbool.h> // Para usar o tipo 'bool' (true/false) em C99
#include <stddef.h>  // Para usar o tipo 'size_t'

// --- Constantes Configuráveis ---
#define TAMANHO_MIN_NOME 3
#define TAMANHO_MAX_NOME 50

/**
 * @brief Valida uma string para garantir que seja um nome de usuário válido.
 * * Regras aplicadas:
 * 1. Comprimento entre TAMANHO_MIN_NOME e TAMANHO_MAX_NOME.
 * 2. Não pode ser nulo (NULL).
 * 3. Não pode começar ou terminar com espaço.
 * 4. Não pode conter espaços consecutivos.
 * 5. Deve conter apenas caracteres alfabéticos (incluindo acentuados,
 * se o locale estiver configurado corretamente) e espaços.
 * * @param nome A string (const char*) a ser validada.
 * @return true se o nome for válido, false caso contrário.
 */
bool validarNome(const char *nome);

#endif // VALIDACAO_H