#include "cab.h"
#include "common.h"
#include "rider.h"
#include "server.h"

int main() {
    int cabsCount, ridersCount, serversCount;
    scanf("%d%d%d", &cabsCount, &ridersCount, &serversCount);
    assert(cabsCount <= MAX_CABS);
    assert(ridersCount <= MAX_RIDERS);
    assert(serversCount <= MAX_SERVERS);

    serversOpenCount = serversCount;

    for (int i = 0; i < cabsCount; i++) {
        pthread_t thread;
        pthread_create(&thread, NULL, getNewCab, cabs[i]);
    }

    // second argument = 0 => initialize three semaphores shared between threads
    sem_init(&totalCabsOpen, 0, cabsCount);
    sem_init(&totalPoolCabsOpen, 0, 0);
    sem_init(&totalPremierCabsOpen, 0, 0);

    for (int i = 0; i < ridersCount; i++) {
        pthread_t thread;
        pthread_create(&thread, NULL, initRider, riders[i]);
    }

    sem_destroy(&totalCabsOpen);
    sem_destroy(&totalPoolCabsOpen);
    sem_destroy(&totalPremierCabsOpen);
    sem_destroy(&serversOpen);

    return 0;
}