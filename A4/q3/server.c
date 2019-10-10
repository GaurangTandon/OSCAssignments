#include "server.h"

void acceptPayment(server* server) {
    sleep(2);
}

void makePayment() {
    sem_wait(&serversOpen);

    acceptPayment(servers[serversOpenCount--]);
    serversOpenCount++;

    sem_post(&serversOpen);
}