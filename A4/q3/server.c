#include "server.h"
#include "rider.h"

void printServerHead(int id) {
    printTimestamp();
    printf(KBLUE "Server %d\t" KNRM, id + 1);
}

void* initServer(void* serverTemp) {
    server* myserver = (server*)serverTemp;
    printServerHead(myserver->id);
    printf("initialized\n");

    while (ridersLeftToExit) {
        sem_wait(&serversOpen);

        pthread_mutex_lock(&checkPayment);
        if (!ridersPaying[0]) {
            pthread_mutex_unlock(&checkPayment);
            break;
        }
        rider* r = ridersPaying[ridersPayingCount - 1];
        ridersPaying[ridersPayingCount - 1] = NULL;
        ridersPayingCount -= 1;
        pthread_mutex_unlock(&checkPayment);
        printServerHead(myserver->id);
        printf("accepting payment from %d\n", r->id + 1);

        sleep(2);

        printServerHead(myserver->id);
        printf("received payment from %d\n", r->id + 1);
        madePayment(r);
    }

    printServerHead(myserver->id);
    printf("All riders left, so closing down\n");

    return NULL;
}