#ifndef VALIDARNASCIMENTO_H
#define VALIDARNASCIMENTO_H

#include <stdbool.h>

// ... outras declarações como validarNome, validarCPF ...

/**
 * @brief Valida uma string para garantir que seja uma data de nascimento válida.
 * * Regras aplicadas:
 * 1. O formato deve ser "DD/MM/AAAA".
 * 2. Valida se a data existe no calendário (considerando anos bissextos).
 * 3. Garante que o ano esteja em um intervalo razoável (ex: > 1900).
 * 4. A data de nascimento não pode ser uma data futura.
 * * @param data_nasc A string (const char*) a ser validada.
 * @return true se a data for válida, false caso contrário.
 */
bool validarNascimento(const char *data_nasc);
int calcularIdade(const char *nascimento);

#endif // VALIDACAO_H