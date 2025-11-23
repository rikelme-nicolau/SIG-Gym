#ifndef CONSOLE_LAYOUT_H
#define CONSOLE_LAYOUT_H

// Largura fixa para manter todas as telas simetricas
#define UI_WIDTH 76
#define UI_INNER (UI_WIDTH - 4)

void ui_line(char ch);
void ui_center_text(const char *text);
void ui_text_line(const char *text);
void ui_empty_line(void);
void ui_menu_option(char key, const char *label);
void ui_header(const char *title, const char *subtitle);
void ui_section_title(const char *title);

/**
 * Trunca uma string UTF-8 para caber em uma largura de coluna.
 * @param src        Texto de entrada (UTF-8)
 * @param max_cols   Largura máxima em colunas (cada codepoint conta como 1 col)
 * @param dest       Buffer de saída
 * @param dest_size  Tamanho do buffer de saída
 * @return Largura efetiva (em colunas) copiada para dest
 */
int ui_clip_utf8(const char *src, int max_cols, char *dest, size_t dest_size);

// Helpers para montar linhas de tabela com colunas fixas (largura em colunas)
void ui_append_col(char *line, size_t size, int *pos, const char *text, int col_width);
void ui_append_sep(char *line, size_t size, int *pos);

#endif // CONSOLE_LAYOUT_H
