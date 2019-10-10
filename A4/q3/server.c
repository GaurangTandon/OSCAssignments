#include "server.h"
#include "rider.h"

void* initServer(void* serverTemp) {
    server* myserver = (server*)serverTemp;
    printTimestamp();
    printf("Server %d:\tinitialized\n", myserver->id);
    fflush(stdout);

    // TODO: why doesn't server code exit?
    while (ridersLeftToExit) {
        sem_wait(&serversOpen);

        printTimestamp();
        printf("Server %d:\taccepting payment from %d\n", myserver->id, -1);
        fflush(stdout);

        sleep(2);

        printTimestamp();
        printf("Server %d:\treceived payment from %d\n", myserver->id, -1);
        fflush(stdout);
    }

    printTimestamp();
    printf("Server %d:\tAll riders left, so closing down\n", myserver->id);

    return NULL;
}