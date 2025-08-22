#ifndef HEADER_H
#define HEADER_H
void telaSobre(void);   
char telaPrincipal(void);   
char telaPlano(void);
char telaCheckin(void);
char telaMatriculas(void);

void moduloAluno(char);


void telaAluno(void);

typedef void (*modulo_aluno)(char);
typedef void (*modulo_aluno_return)(void);

void telaCadastrarAluno(void);
void telaVisualizarAluno(void); 
void telaAtualizarAluno(void);
void telaExcluirAluno(void);
#endif