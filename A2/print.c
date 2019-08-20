#include "print.h"
#include <stdio.h>

void echo(char* str, int keepTrailingNewline) {
    printf("%s", str);

    if (keepTrailingNewline)
        printf("\n");
}