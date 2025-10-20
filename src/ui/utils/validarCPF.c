#include <stdbool.h>
#include <string.h>

#include <ctype.h> // Para a função isdigit()

bool validarCPF(const char *cpf) {
    // 1. Verificação de ponteiro nulo
    if (cpf == NULL) {
        return false;
    }

    // 2. Limpeza da string: remove pontos e traços
    char cpf_numeros[12]; // 11 dígitos + '\0'
    int j = 0; // Índice para cpf_numeros
    for (int i = 0; cpf[i] != '\0'; i++) {
        if (isdigit(cpf[i])) {
            if (j < 11) {
                cpf_numeros[j++] = cpf[i];
            } else {
                return false; // Mais de 11 dígitos
            }
        }
    }
    cpf_numeros[j] = '\0'; // Finaliza a string de números

    // 3. Verifica se o CPF tem exatamente 11 dígitos
    if (strlen(cpf_numeros) != 11) {
        return false;
    }

    // 4. Verifica se todos os dígitos são iguais (caso inválido)
    bool todos_iguais = true;
    for (int i = 1; i < 11; i++) {
        if (cpf_numeros[i] != cpf_numeros[0]) {
            todos_iguais = false;
            break;
        }
    }
    if (todos_iguais) {
        return false;
    }

    // 5. Cálculo do primeiro dígito verificador
    int soma = 0;
    for (int i = 0; i < 9; i++) {
        soma += (cpf_numeros[i] - '0') * (10 - i);
    }
    int resto = soma % 11;
    int digito1 = (resto < 2) ? 0 : 11 - resto;

    // Compara com o 10º dígito do CPF
    if (digito1 != (cpf_numeros[9] - '0')) {
        return false;
    }

    // 6. Cálculo do segundo dígito verificador
    soma = 0;
    for (int i = 0; i < 10; i++) {
        soma += (cpf_numeros[i] - '0') * (11 - i);
    }
    resto = soma % 11;
    int digito2 = (resto < 2) ? 0 : 11 - resto;

    // Compara com o 11º dígito do CPF
    if (digito2 != (cpf_numeros[10] - '0')) {
        return false;
    }

    // Se passou por todas as verificações, o CPF é válido!
    return true;
}