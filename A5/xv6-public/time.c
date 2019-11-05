#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int a = fork();

    if (a == 0) {
        exec(argv[1], argv + 1);
    } else {
        int *wtime = 0, *rtime = 0;
        waitx(wtime, rtime);
        printf(1, "Runtime: %d\n", *wtime);
    }

    exit();
}
