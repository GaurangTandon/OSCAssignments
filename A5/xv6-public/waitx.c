#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int a = fork();

    if (a == 0) {
        char **args = 0;
        exec("./test", args);
    } else {
        int *wtime = 0, *rtime = 0;
        waitx(wtime, rtime);
    }

    exit();
}
