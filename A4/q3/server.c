#include "server.h"
#include "rider.h"

void* initServer(void* serverTemp) {
    server* myserver = (server*)serverTemp;
    printTimestamp();
    printf("Server %d:\tinitialized\n", myserver->id);
    fflush(stdout);

    while (ridersLeftToExit) {
        sem_wait(&serversOpen);

        pthread_mutex_lock(&checkPayment);
        rider* r = ridersPaying[--ridersPayingCount];
        pthread_mutex_unlock(&checkPayment);

        printTimestamp();
        printf("Server %d:\taccepting payment from %d\n", myserver->id, r->id);
        fflush(stdout);

        sleep(2);

        printTimestamp();
        printf("Server %d:\treceived payment from %d\n", myserver->id, r->id);
        fflush(stdout);
    }

    printTimestamp();
    printf("Server %d:\tAll riders left, so closing down\n", myserver->id);

    return NULL;
}