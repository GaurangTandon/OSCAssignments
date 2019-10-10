#include "server.h"
#include "rider.h"

void acceptPayment() {
    sleep(2);
}

void makePayment(rider* rider) {
    sem_wait(&serversOpen);

    server* used = servers[--serversOpenCount];
    printf("Rider %d is making payment on server %d\n", rider->id, used->id);
    acceptPayment(used);
    serversOpenCount++;

    sem_post(&serversOpen);
}

void* initServer(void* serverTemp) {
    server* myserver = (server*)serverTemp;
    printf("Server id %d initialized\n", myserver->id);
    fflush(stdout);
    return NULL;
}