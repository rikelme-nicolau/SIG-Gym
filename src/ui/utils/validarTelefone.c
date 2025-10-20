#include <stdbool.h>
#include <string.h>
#include <ctype.h> // Para isdigit()

bool validarTelefone(const char *telefone) {
    // 1. Verificação de ponteiro nulo
    if (telefone == NULL) {
        return false;
    }

    // 2. Limpeza da string: remove tudo que não for dígito
    char telefone_numeros[12]; // Máximo de 11 dígitos + '\0'
    int j = 0; // Índice para telefone_numeros
    for (int i = 0; telefone[i] != '\0'; i++) {
        if (isdigit(telefone[i])) {
            if (j < 11) {
                telefone_numeros[j++] = telefone[i];
            } else {
                return false; // Mais de 11 dígitos
            }
        }
    }
    telefone_numeros[j] = '\0'; // Finaliza a string de números

    // 3. Verifica o comprimento total de dígitos
    size_t len = strlen(telefone_numeros);
    if (len != 10 && len != 11) {
        return false; // Deve ter 10 (fixo) ou 11 (celular) dígitos
    }

    // 4. Valida o DDD (não pode começar com 0)
    if (telefone_numeros[0] == '0') {
        return false;
    }

    // 5. Se tiver 11 dígitos, valida se é um celular (o 3º dígito deve ser '9')
    if (len == 11 && telefone_numeros[2] != '9') {
        return false;
    }

    // Se passou por todas as verificações, o telefone é válido!
    return true;
}