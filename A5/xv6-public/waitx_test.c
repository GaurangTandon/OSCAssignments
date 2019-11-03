#include "types.h"
#include "stat.h"
#include "user.h"

// Parent forks two children, waits for them to exit and then finally exits
int main(void) {
    if (fork() == 0) {
        // child
        set_priority(50);
        for (int i = 0; i < 10; i++) {
            printf(5, "1 - %d\n", i);
        }
        exit();
    } else {
        if (fork() == 0) {
            // child
            set_priority(40);
            for (int i = 0; i < 10; i++) {
                printf(5, "2 - %d\n", i);
            }
            exit();
        } else {
            for (int i = 0; i < 10; i++) {
                printf(5, "2 - %d\n", i);
            }
            wait();
            wait();
            exit();
        }
    }
}