#ifndef VALIDARCPF_H
#define VALIDARCPF_H

#include <stdbool.h>

// ... outras declarações como validarNome ...

/**
 * @brief Valida uma string para garantir que seja um CPF válido.
 * * Regras aplicadas:
 * 1. A string pode ter o formato "XXX.XXX.XXX-XX" ou "XXXXXXXXXXX".
 * 2. Remove a pontuação para a análise.
 * 3. Verifica se o CPF contém 11 dígitos.
 * 4. Verifica se todos os dígitos são iguais (ex: "111.111.111-11"), o que é inválido.
 * 5. Calcula e valida o primeiro e o segundo dígitos verificadores.
 * * @param cpf A string (const char*) a ser validada.
 * @return true se o CPF for válido, false caso contrário.
 */
bool validarCPF(const char *cpf);

#endif // VALIDACAO_H