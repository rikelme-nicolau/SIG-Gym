#ifndef VALIDAREMAIL_H
#define VALIDAREMAIL_H

#include <stdbool.h>

// ... outras declarações como validarNome, validarEndereco, etc. ...

/**
 * @brief Valida uma string para garantir que seja um endereço de e-mail com formato válido.
 * * Regras aplicadas:
 * 1. Deve conter exatamente um caractere '@'.
 * 2. Deve haver caracteres antes e depois do '@'.
 * 3. A parte do domínio (depois do '@') deve conter pelo menos um ponto.
 * 4. Não pode começar ou terminar com '@' ou '.'.
 * 5. Não contém espaços.
 * * @param email A string (const char*) a ser validada.
 * @return true se o e-mail for válido, false caso contrário.
 */
bool validarEmail(const char *email);

#endif // VALIDACAO_H