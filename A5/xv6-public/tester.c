#include "types.h"
#include "stat.h"
#include "procstat.h"
#include "user.h"

// Parent forks two children, waits for them to exit and then finally exits
int main(void) {
#ifdef PBS
    // forks ten processes with priorities initially of the value pid/2,
    // but after that halfway through it resets the priorities to 1000-j/2
    int count = 10, lim = 1e7, halfLim = lim / 2;
    for (int j = 0; j < count; j++) {
        if (fork() == 0) {
            volatile int a = 0;
            for (volatile int i = 0; i <= lim; i++) {
                if (i == halfLim) {
                    set_priority(100 - j / 2);
                } else {
                    a += 3;
                }
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
#ifdef MLFQ
    // doesn't work
    struct proc_stat ps;
    getpinfo(&ps);
    printf(1, "%d\n", ps.pid);
#endif
}