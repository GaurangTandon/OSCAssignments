#include "cab.h"
#include "common.h"
#include "rider.h"
#include "server.h"

#define MAX_CABS 100
#define MAX_RIDERS 100
#define MAX_SERVERS 100

cab cabs[MAX_CABS];
rider riders[MAX_RIDERS];
server servers[MAX_SERVERS];

int main() {
    int cabsCount, ridersCount, serversCount;
    scanf("%d%d%d", &cabsCount, &ridersCount, &serversCount);
    assert(cabsCount <= MAX_CABS);
    assert(ridersCount <= MAX_RIDERS);
    assert(serversCount <= MAX_SERVERS);

    for (int i = 0; i < cabsCount; i++) {
        pthread_t thread;
        pthread_create(&thread, NULL, getNewCab, &cabs[i]);
    }

    // for (int i = 0; i < serversCount; i++) {
    //     pthread_t thread;
    //     pthread_create(&thread, NULL, getNewServer, &servers[i]);
    // }

    for (int i = 0; i < ridersCount; i++) {
        pthread_t thread;
        pthread_create(&thread, NULL, initRider, &riders[i]);
    }

    return 0;
}