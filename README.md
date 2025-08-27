# SIG-Gym: Um Sistema de Gestão de Academias de Ginástica

Sistema acadêmico de **gestão de academia** desenvolvido em **C99** sem a utilização de bibliotecas externas.  
**Disciplina**: DCT1106 — Programação.  

---

## Índice
- [1. Como compilar e executar](#1-como-compilar-e-executar)
- [2. Estrutura de pastas](#2-estrutura-de-pastas)

---

## 1. Como compilar e executar
> Requer GCC com suporte a C99.

### Comando direto
No diretório `src/` do projeto, execute:

```bash
gcc -std=c99 -Wall -Wextra -I. \
  *.c \
  ui/aluno/*.c \
  ui/checkin/*.c \
  ui/matricula/*.c \
  ui/plano/*.c \
  ui/*.c \
  -o sig_gym

Executar:
./sig_gym    # Linux/Mac
.\sig_gym    # Windows (PowerShell ou CMD)


---

## 2. Estrutura de pastas

SIG-Gym/
├─ src/
│  ├─ main.c             
│  └─ ui/
│     ├─ telaPrincipal.c
│     ├─ telaSobre.c
│     ├─ aluno/          
│     │   ├─ cadastrar.c
│     │   ├─ atualizar.c
│     │   ├─ visualizar.c
│     │   └─ excluir.c
│     └─ plano/          
│         ├─ cadastrar.c
│         ├─ atualizar.c
│         ├─ visualizar.c
│         └─ excluir.c

