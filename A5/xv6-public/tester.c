#include "types.h"
#include "stat.h"
#include "user.h"
#include "procstat.h"

// Parent forks two children, waits for them to exit and then finally exits
int main(int argc, char* argv[]) {
#ifdef FCFS
    int count = 10, lim = 1e7;
    for (int j = 0; j < count; j++) {
        if (fork() == 0) {
            volatile int a = 0;
            for (volatile int i = 0; i <= lim; i++) {
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
#ifdef PBS
    // forks ten processes with priorities initially of the value pid/2,
    // but after that halfway through it resets the priorities to 1000-j/2
    int count = 10, lim = 1e8, halfLim = lim / 2;

    for (int j = 0; j < count; j++) {
        if (fork() == 0) {
            volatile int a = 0;
            int pid = getpid();
            for (volatile int i = 0; i <= lim; i++) {
                if (i % (lim / 10) == 0)
                    printf(1, "Completed %d by %d of pid %d\n", i / (lim / 10),
                           10, pid);
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
    printf(1, "use tester2 instead\n");
    exit();
#endif
#ifdef DEFAULT
    int count = 10, lim = 1e7;
    for (int j = 0; j < count; j++) {
        if (fork() == 0) {
            volatile int a = 0;
            int pid = getpid();
            for (volatile int i = 0; i <= lim; i++) {
                if (i % (lim / 10) == 0) {
                    printf(1, "Completed %d by %d of pid %d\n", i / (lim / 10),
                           10, pid);
                }
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