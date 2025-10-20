#ifndef VALIDARTELEFONE_H
#define VALIDARTELEFONE_H

#include <stdbool.h>

// ... outras declarações como validarNome, validarCPF, etc. ...

/**
 * @brief Valida uma string para garantir que seja um número de telefone brasileiro válido.
 * * Regras aplicadas:
 * 1. Aceita formatos como "(DD) 9XXXX-XXXX", "DD9XXXXXXXX", "DD XXXX-XXXX", etc.
 * 2. Remove a formatação para a análise.
 * 3. Garante que o número de dígitos seja 10 (fixo) ou 11 (celular).
 * 4. Valida se o DDD não começa com 0.
 * 5. Se o número tiver 11 dígitos, verifica se o nono dígito (o terceiro no geral) é '9'.
 * * @param telefone A string (const char*) a ser validada.
 * @return true se o telefone for válido, false caso contrário.
 */
bool validarTelefone(const char *telefone);

#endif // VALIDACAO_H