#include "types.h"
#include "stat.h"
#include "user.h"

// Parent forks two children, waits for them to exit and then finally exits
int main(void) {
#ifdef PBS
    int count = 10, lim = 1e7, halfLim = lim / 2;
    for (int j = 0; j < count; j++) {
        if (fork() == 0) {
            volatile int a = 0;
            for (volatile int i = 0; i <= lim; i++) {
                if (i == halfLim) {
                    set_priority(100 - j / 2);
                } else
                    a += 3;
            }
            printf(1, "%d\n", a);
            exit();
        }
    }

    for (int i = 0; i < count; i++) {
        wait();
    }

    exit();
#endif
}