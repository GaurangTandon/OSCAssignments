#include "types.h"
#include "stat.h"
#include "user.h"

// Parent forks two children, waits for them to exit and then finally exits
int main(void) {
    int count = 4;

    // assert(count <= 6);

    for (int i = 0; i < count; i++) {
        if (fork() == 0) {
            double a;
            for (volatile int i = 0; i < 100; i++) {
                a = 3.14 * 54;
            }
            printf(5, "%lf", a);
            exit();
        }
    }

    for (int i = 0; i < count; i++) {
        wait();
    }
    exit();
}