
#include "nightswatch.h"
#include <stdio.h>
#include <time.h>
#include "commands.h"
#include "process.h"

void nightsprint(int dirty, int interval) {
    int c = 0;
    int msec = 0, iterations = 0;
    interval *= 1000;
    clock_t before = clock();

    if (dirty)
        dirtyMemPrint();
    else
        interruptPrint(c++);

    while (1) {
        clock_t difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        iterations++;

        if (keyboardWasPressed()) {
            char c = getchar();
            if (c == 'q') {
                printf("\n");
                fflush(stdout);
                break;
            }
        }
        if (msec >= interval) {
            before = clock();
            if (dirty)
                dirtyMemPrint();
            else
                interruptPrint(c++);
        }
    }
}