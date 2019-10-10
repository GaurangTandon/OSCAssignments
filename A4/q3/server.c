#include "server.h"
#include "rider.h"

void acceptPayment() {
    sleep(2);
}

void* initServer(void* serverTemp) {
    server* myserver = (server*)serverTemp;
    printf("Server %d:\tinitialized\n", myserver->id);
    fflush(stdout);

    while (1) {
        sem_wait(&serversOpen);
        printf("Server %d:\taccepting payment from %d\n", myserver->id, -1);
        fflush(stdout);
        sleep(2);
        printf("Server %d:\treceived payment from %d\n", myserver->id, -1);
        fflush(stdout);
    }

    return NULL;
}