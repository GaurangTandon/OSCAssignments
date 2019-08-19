#include "print.h"

void echo(char* str, int keepTrailingNewline) {
    printf("%s", str);

    if (keepTrailingNewline)
        printf("\n");
}