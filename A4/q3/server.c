#include "server.h"
#include "rider.h"

void acceptPayment() {
    sleep(2);
}

void* initServer(void* serverTemp) {
    server* myserver = (server*)serverTemp;
    printf("Server id %d initialized\n", myserver->id);
    fflush(stdout);

    while (1) {
        sem_wait(&serversOpen);
        printf("Server %d: accepting payment from %d\n", myserver->id, -1);
        fflush(stdout);
        sleep(2);
        printf("Server %d: received payment from %d\n", myserver->id, -1);
        fflush(stdout);
    }

    return NULL;
}