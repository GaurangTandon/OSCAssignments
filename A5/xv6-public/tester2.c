#include "types.h"
#include "stat.h"
#include "user.h"
#include "procstat.h"

int main(int argc, char* argv[]) {
    int count = 10, lim = 5e7;

    for (int j = 0; j < count; j++) {
        if (fork() == 0) {
            volatile int a = 0;
            int pid = getpid();
            int lim2 = lim * (10 - j + 1);

            for (volatile int i = 0; i <= lim2; i++) {
                if (i % (lim2 / 10) == 0) {
                    if (DEBUG)
                        printf(1, "Completed %d by %d of pid %d\n",
                               i / (lim / 10), 10, pid);
                }

                a += 3;
            }

            if (DEBUG)
                printf(1, "%d\n", a);
            exit();
        }
    }

    for (int i = 0; i < count; i++) {
        wait();
    }

    exit();
}