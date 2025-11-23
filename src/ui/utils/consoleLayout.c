#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "ui/utils/consoleLayout.h"

static int utf8_decode(const char *s, uint32_t *cp);

void ui_line(char ch)
{
    putchar('+');
    for (int i = 0; i < UI_WIDTH - 2; ++i)
    {
        putchar(ch);
    }
    puts("+");
}

static int clamp_len(const char *text)
{
    int len = (int)strlen(text);
    return len > UI_INNER ? UI_INNER : len;
}

void ui_center_text(const char *text)
{
    int len = clamp_len(text);
    int pad = (UI_INNER - len) / 2;
    printf("| %*s%.*s%*s |\n", pad, "", len, text, UI_INNER - pad - len, "");
}

static int utf8_width(const char *s)
{
    int cols = 0;
    const char *p = s;
    while (*p != '\0')
    {
        uint32_t cp;
        int consumed = utf8_decode(p, &cp);
        if (consumed <= 0)
        {
            break;
        }
        cols += 1; // considera largura 1 para caracteres latinos/acentos
        p += consumed;
    }
    return cols;
}

void ui_text_line(const char *text)
{
    char clipped[UI_INNER * 4 + 1];
    const char *use = text;
    int cols = utf8_width(text);
    if (cols > UI_INNER)
    {
        ui_clip_utf8(text, UI_INNER, clipped, sizeof(clipped));
        use = clipped;
        cols = utf8_width(use);
    }

    fputs("| ", stdout);
    fputs(use, stdout);
    for (int i = cols; i < UI_INNER; ++i)
    {
        putchar(' ');
    }
    puts(" |");
}

void ui_empty_line(void)
{
    ui_text_line("");
}

void ui_menu_option(char key, const char *label)
{
    char buffer[UI_INNER + 1];
    snprintf(buffer, sizeof(buffer), "[%c] %s", key, label);
    ui_text_line(buffer);
}

void ui_header(const char *title, const char *subtitle)
{
    ui_line('=');
    ui_center_text(title);
    if (subtitle != NULL && subtitle[0] != '\0')
    {
        ui_center_text(subtitle);
    }
    ui_line('=');
}

void ui_section_title(const char *title)
{
    ui_line('-');
    ui_center_text(title);
    ui_line('-');
}

// --- Helpers de largura UTF-8 (cada codepoint conta como 1 coluna) ---
static int utf8_decode(const char *s, uint32_t *cp)
{
    const unsigned char *u = (const unsigned char *)s;
    if (u[0] < 0x80)
    {
        *cp = u[0];
        return 1;
    }
    if ((u[0] & 0xE0) == 0xC0 && (u[1] & 0xC0) == 0x80)
    {
        *cp = ((u[0] & 0x1F) << 6) | (u[1] & 0x3F);
        return 2;
    }
    if ((u[0] & 0xF0) == 0xE0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80)
    {
        *cp = ((u[0] & 0x0F) << 12) | ((u[1] & 0x3F) << 6) | (u[2] & 0x3F);
        return 3;
    }
    if ((u[0] & 0xF8) == 0xF0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80 && (u[3] & 0xC0) == 0x80)
    {
        *cp = ((u[0] & 0x07) << 18) | ((u[1] & 0x3F) << 12) | ((u[2] & 0x3F) << 6) | (u[3] & 0x3F);
        return 4;
    }
    *cp = '?';
    return 1;
}

int ui_clip_utf8(const char *src, int max_cols, char *dest, size_t dest_size)
{
    if (dest_size == 0)
    {
        return 0;
    }

    int cols = 0;
    size_t di = 0;
    const char *p = src;

    while (*p != '\0' && cols < max_cols && di + 1 < dest_size)
    {
        uint32_t cp = 0;
        int consumed = utf8_decode(p, &cp);
        if (consumed <= 0)
        {
            break;
        }

        int w = 1; // Latin e sinais comuns contam como 1 coluna
        if (cols + w > max_cols)
        {
            break;
        }

        if (di + (size_t)consumed >= dest_size)
        {
            break;
        }

        for (int i = 0; i < consumed; i++)
        {
            dest[di++] = p[i];
        }
        cols += w;
        p += consumed;
    }

    dest[di] = '\0';
    return cols;
}

void ui_append_col(char *line, size_t size, int *pos, const char *text, int col_width)
{
    char clip[256];
    int width = ui_clip_utf8(text, col_width, clip, sizeof(clip));
    if (width < 0)
    {
        width = 0;
    }

    int written = snprintf(line + *pos, size > (size_t)*pos ? size - (size_t)*pos : 0, "%s", clip);
    if (written > 0)
    {
        *pos += written;
    }

    int pad = col_width - width;
    while (pad-- > 0 && (size_t)*pos + 1 < size)
    {
        line[(*pos)++] = ' ';
    }
}

void ui_append_sep(char *line, size_t size, int *pos)
{
    const char *sep = " | ";
    for (int i = 0; sep[i] != '\0' && (size_t)*pos + 1 < size; i++)
    {
        line[(*pos)++] = sep[i];
    }
}
