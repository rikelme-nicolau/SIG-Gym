#include <stdio.h>
#include <stdlib.h>

void limparTela(void) {
#ifdef _WIN32
    system("cls");   // Windows
#else
    system("clear"); // Linux / Mac
#endif
}