#include "types.h"
#include "stat.h"
#include "user.h"
#include "procstat.h"

int main(int argc, char* argv[]) {
    int count = 10, lim = 5e7;

    for (int j = 0; j < count; j++) {
        int a = fork();

        if (a == 0) {
            volatile int a = 0;
            int pid = getpid();
            int lim2 = lim * (10 - j + 1);

            for (volatile int i = 0; i <= lim2; i++) {
                if (i % (lim2 / 10) == 0) {
                    if (DEBUG)
                        printf(1, "Completed %d by %d of pid %d\n",
                               i / (lim2 / 10), 10, pid);
                }

                a += 3;
            }

#ifdef MLFQ
            struct proc_stat* ps =
                (struct proc_stat*)malloc(sizeof(struct proc_stat));
            getpinfo(ps, pid);
            printf(1, "PID %d, rtime %d, num run %d, allotedQ %d %d, ticks: ",
                   ps->pid, ps->runtime, ps->num_run, ps->allotedQ[0],
                   ps->allotedQ[1]);
            for (int i = 0; i < PQ_COUNT; i++) {
                printf(1, "%d, ", ps->ticks[i]);
            }
            printf(1, "\n");
#endif
            exit();
        }
    }

    for (int i = 0; i < count; i++) {
        wait();
    }

    exit();
}