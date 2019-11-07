#include "types.h"
#include "stat.h"
#include "user.h"
#include "procstat.h"

int main(int argc, char* argv[]) {
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
}