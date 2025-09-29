#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cadastrarPlano.h"
#include "arquivoPlano.h"

#define PLANOS_FILE "planos.txt"
#define TMP_FILE    "planos.tmp"

// Salva todos os planos ativos no arquivo
void salvarPlanos(struct plano lista_planos[], int total_planos) {
    FILE *fp = fopen(TMP_FILE, "wt");
    if (!fp) { perror("Erro ao criar arquivo"); return; }

    for (int i = 0; i < total_planos; i++) {
        if(lista_planos[i].ativo) {
            fprintf(fp, "%s;%s;%s;%s;%d\n",
                    lista_planos[i].id,
                    lista_planos[i].nome,
                    lista_planos[i].horario_inicio,
                    lista_planos[i].horario_fim,
                    lista_planos[i].total_atividades);
            for(int j=0; j < lista_planos[i].total_atividades; j++) {
                fprintf(fp, "%s\n", lista_planos[i].atividades[j]);
            }
        }
    }

    fclose(fp);
    remove(PLANOS_FILE);
    rename(TMP_FILE, PLANOS_FILE);
}

// Carrega todos os planos do arquivo
int carregarPlanos(struct plano lista_planos[]) {
    FILE *fp = fopen(PLANOS_FILE, "rt");
    if(!fp) return 0;

    int total = 0;
    while(!feof(fp) && total < MAX_PLANOS) {
        if(fscanf(fp, "%11[^;];%1023[^;];%11[^;];%11[^;];%d\n",
                  lista_planos[total].id,
                  lista_planos[total].nome,
                  lista_planos[total].horario_inicio,
                  lista_planos[total].horario_fim,
                  &lista_planos[total].total_atividades) != 5) break;

        for(int j=0; j < lista_planos[total].total_atividades; j++) {
            fgets(lista_planos[total].atividades[j], MAX_BUFFER, fp);
            lista_planos[total].atividades[j][strcspn(lista_planos[total].atividades[j], "\n")] = '\0';
        }

        lista_planos[total].ativo = true;
        total++;
    }

    fclose(fp);
    return total;
}

// Atualiza um plano específico no arquivo
void atualizarPlanoNoArquivo(struct plano plano) {
    struct plano planos[MAX_PLANOS];
    int total = carregarPlanos(planos);

    for(int i=0; i<total; i++) {
        if(strcmp(planos[i].id, plano.id) == 0) {
            planos[i] = plano;
            break;
        }
    }

    salvarPlanos(planos, total);
}

// Marca um plano como excluído
void excluirPlano(char *id) {
    struct plano planos[MAX_PLANOS];
    int total = carregarPlanos(planos);

    for(int i=0; i<total; i++) {
        if(strcmp(planos[i].id, id) == 0) {
            planos[i].ativo = false;
            break;
        }
    }

    salvarPlanos(planos, total);
}
