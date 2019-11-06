#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int a = fork();

    if (a == 0) {
        int c = 0;
        for (volatile int i = 0; i < 1e7; i++) {
            c++;
            i--;
            i++;
        }
        printf(1, "C = %d\n", c);
    } else {
        int *wtime = (int *)malloc(sizeof(int)),
            *rtime = (int *)malloc(sizeof(int));
        waitx(wtime, rtime);
        printf(1, "w = %d; r = %d\n", *wtime, *rtime);
    }

    exit();
}
