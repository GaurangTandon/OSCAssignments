#include "server.h"
#include "rider.h"

void printServerHead(int id) {
    printTimestamp();
    printf(KBLUE "Server %d\t" KNRM, id);
}

void* initServer(void* serverTemp) {
    server* myserver = (server*)serverTemp;
    printServerHead(myserver->id);
    printf("initialized\n");
    fflush(stdout);

    while (ridersLeftToExit) {
        sem_wait(&serversOpen);

        pthread_mutex_lock(&checkPayment);
        rider* r = ridersPaying[--ridersPayingCount];
        pthread_mutex_unlock(&checkPayment);

        printServerHead(myserver->id);
        printf("accepting payment from %d\n", r->id);
        fflush(stdout);

        sleep(2);

        printServerHead(myserver->id);
        printf("received payment from %d\n", r->id);
        fflush(stdout);
    }

    printServerHead(myserver->id);
    printf("All riders left, so closing down\n");

    return NULL;
}