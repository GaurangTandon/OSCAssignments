#include "main.h"

int main() {
    int cabsCount, ridersCount, serversCount;
    // scanf("%d%d%d", &cabsCount, &ridersCount, &serversCount);
    cabsCount = 5;
    ridersCount = 10;
    serversCount = 2;
    assert(cabsCount <= MAX_CABS);
    assert(ridersCount <= MAX_RIDERS);
    assert(serversCount <= MAX_SERVERS);

    CAB_STRING = (char**)malloc(sizeof(char*) * 2);
    CAB_STRING[0] = "POOL";
    CAB_STRING[1] = "PREMIER";

    for (int i = 0; i < MAX_RIDERS; i++) {
        pthread_cond_t x = PTHREAD_COND_INITIALIZER;
        riderConditions[i] = x;
        pthread_mutex_t x2 = PTHREAD_MUTEX_INITIALIZER;
        riderMutexes[i] = x2;
        riderWaiting[i] = 0;
    }

    serversOpenCount = serversCount;

    for (int i = 0; i < cabsCount; i++) {
        pthread_t thread;
        waitingCabs[i] = (cab*)malloc(sizeof(cab));
        waitingCabs[i]->id = i;
        pthread_create(&thread, NULL, initCab, waitingCabs[i]);
    }

    for (int i = 0; i < serversCount; i++) {
        pthread_t thread;
        servers[i] = (server*)malloc(sizeof(server));
        servers[i]->id = i;
        pthread_create(&thread, NULL, initServer, servers[i]);
    }

    // second argument = 0 => initialize semaphores shared between threads
    sem_init(&serversOpen, 0, 0);

    for (int i = 0; i < ridersCount; i++) {
        pthread_t thread;
        riders[i] = (rider*)malloc(sizeof(rider));
        riders[i]->id = i;
        pthread_create(&thread, NULL, initRider, riders[i]);
    }

    sem_destroy(&serversOpen);

    return 0;
}