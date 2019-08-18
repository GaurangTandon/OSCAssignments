#include "print.h"

char* escapeSlashes(char* str, int len) {
    int i = 0, j = 0;
    char output[2 * len + 1];

    for (; i < len; i++) {
        if (str[i] == '\\') {
        }
    }
}

void echo(char* str, int keepTrailingNewline, int enableBackslash) {
    if (enableBackslash)
        printf("%s", str);
    else {
    }
    if (keepTrailingNewline)
        printf("\n");
}