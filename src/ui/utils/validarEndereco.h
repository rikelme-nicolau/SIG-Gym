#ifndef VALIDARENDERECO_H
#define VALIDARENDERECO_H

#include <stdbool.h>

// ... outras declarações como validarNome, validarTelefone, etc. ...

/**
 * @brief Valida uma string para garantir que seja um endereço válido.
 * * Regras aplicadas:
 * 1. Garante que o comprimento esteja entre um mínimo e um máximo.
 * 2. Não permite que comece ou termine com espaços.
 * 3. Permite apenas caracteres alfanuméricos, espaços e pontuação comum (,-./).
 * 4. Não permite espaços consecutivos.
 * * @param endereco A string (const char*) a ser validada.
 * @return true se o endereço for válido, false caso contrário.
 */
bool validarEndereco(const char *endereco);

#endif // VALIDACAO_H