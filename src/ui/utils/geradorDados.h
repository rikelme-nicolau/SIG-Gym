#ifndef GERADOR_DADOS_H
#define GERADOR_DADOS_H

#include <stddef.h>
#include <stdbool.h>

struct aluno;
struct funcionario;

void gerarCPFAleatorio(char *destino);
void gerarTelefoneAleatorio(char *destino);
void gerarEmailAleatorio(const char *nome, char *destino);
void gerarEnderecoAleatorio(char *destino, size_t tamanho);
void gerarDataNascimentoAleatoria(char *destino, int idadeMin, int idadeMax);
void gerarDataManutencaoAleatoria(char *destino, int diasMin, int diasMax);
void calcularProximaManutencaoAleatoria(const char *ultima, char *proxima);
bool verificarCPFUnico(const char *cpf,
                       const struct aluno *alunos, int totalAlunos,
                       const struct funcionario *funcs, int totalFuncs);
bool verificarEmailUnico(const char *email,
                         const struct aluno *alunos, int totalAlunos,
                         const struct funcionario *funcs, int totalFuncs);

#endif
