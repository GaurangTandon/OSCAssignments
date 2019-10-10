#include "main.h"

int main() {
    int cabsCount, ridersCount, serversCount;
    scanf("%d%d%d", &cabsCount, &ridersCount, &serversCount);
    assert(cabsCount <= MAX_CABS);
    assert(ridersCount <= MAX_RIDERS);
    assert(serversCount <= MAX_SERVERS);

    for (int i = 0; i < MAX_RIDERS; i++) {
        pthread_cond_t x = PTHREAD_COND_INITIALIZER;
        riderConditions[i] = x;
    }

    for (int i = 0; i < MAX_RIDERS; i++) {
        pthread_mutex_t x = PTHREAD_MUTEX_INITIALIZER;
        riderMutexes[i] = x;
    }

    serversOpenCount = serversCount;

    for (int i = 0; i < cabsCount; i++) {
        pthread_t thread;
        waitingCabs[i]->id = i;
        pthread_create(&thread, NULL, initCab, waitingCabs[i]);
    }

    // second argument = 0 => initialize three semaphores shared between threads
    sem_init(&totalCabsOpen, 0, cabsCount);
    sem_init(&totalPoolCabsOpen, 0, 0);
    sem_init(&totalPremierCabsOpen, 0, 0);

    for (int i = 0; i < ridersCount; i++) {
        pthread_t thread;
        riders[i]->id = i;
        pthread_create(&thread, NULL, initRider, riders[i]);
    }

    sem_destroy(&totalCabsOpen);
    sem_destroy(&totalPoolCabsOpen);
    sem_destroy(&totalPremierCabsOpen);
    sem_destroy(&serversOpen);

    return 0;
}