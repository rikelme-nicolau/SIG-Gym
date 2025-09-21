#include <stdio.h>
#include <string.h>
#include <time.h>

/* Delay em milissegundos sem libs externas (busy-wait) */
static void delay_ms(int ms) {
    const clock_t ticks = (clock_t)((double)ms * (double)CLOCKS_PER_SEC / 1000.0);
    const clock_t start = clock();
    while ((clock() - start) < ticks) { /* busy-wait */ }
}

/* Limpa a tela usando ANSI e posiciona o cursor no topo-esquerda */
static void clear_screen(void) {
    /* ANSI: limpa tudo e move cursor para 1;1 */
    printf("\x1b[2J\x1b[H");
    fflush(stdout);
    /* Se preferir (menos portátil), descomente a linha abaixo para Windows:
       system("cls");
    */
}

void animacao(void) {
    
    /* Seu logo com “pixels grandes” (#) */
    const char *art[] = {
        "  ########   #######   ###########   ###########   ##     ##   ####  ####",
        "  ####        #####    ###      ##   ###      ##     #   #    #####  #####",
        "  ####         ###     ###           ###              # #    ######  ######",
        "  #######      ###     ###    ####   ###    ####      ###    ##############",
        "      ###      ###      ####    ##    ####   ###      ###    #####    #####",
        "      ###     #####       ########      ########     #####   #####    #####",
        "  #######    #######        ######        ######    #######  #####    #####"
    };
    const int rows = (int)(sizeof(art)/sizeof(art[0]));

    /* Calcula a largura máxima para o efeito */
    int width = 0;
    for (int r = 0; r < rows; ++r) {
        int len = (int)strlen(art[r]);
        if (len > width) width = len;
    }

    /* Parâmetros da animação */
    const int frames = 12;       /* ~12 quadros */
    const int ms_per_frame = 250;/* 12 * 250ms ≈ 3000ms (3s) */

    /* Revelação em “wipe” diagonal (coluna limite cresce a cada frame) */
    for (int f = 1; f <= frames; ++f) {
        clear_screen();

        /* Progresso horizontal (0..width) para este frame */
        int limit = (int)((double)width * (double)f / (double)frames);

        for (int r = 0; r < rows; ++r) {
            /* Deslocamento por linha para criar a diagonal (ajuste fino: *2) */
            int row_bias = r * 2;

            for (int c = 0; c < width; ++c) {
                char ch = ' ';
                if (c < (int)strlen(art[r])) ch = art[r][c];

                if (ch == '#') {
                    /* Revela quando passou do limite - viés (wipe diagonal) */
                    if (c + row_bias <= limit) {
                        putchar('#');
                    } else {
                        putchar(' ');
                    }
                } else {
                    /* Mantém espaços e outros caracteres (se houver) */
                    putchar(ch);
                }
            }
            putchar('\n');
        }

        fflush(stdout);
        delay_ms(ms_per_frame);
    }

    /* Mostra a arte completa ao final */
    clear_screen();
    for (int r = 0; r < rows; ++r) {
        printf("%s\n", art[r]);
    }
    fflush(stdout);

    delay_ms(1000);   // espera 1 segundo
    clear_screen();
    
}
