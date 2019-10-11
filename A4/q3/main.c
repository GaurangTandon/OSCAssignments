#include "main.h"
#include <sys/shm.h>

void *shareMem(size_t size) {
    key_t mem_key = IPC_PRIVATE;
    // get shared memory of this much size and with this private key
    // what is 0666?
    int shm_id = shmget(mem_key, size, IPC_CREAT | 0666);
    // attach the address space of shared memory to myself (callee)
    return (void *)shmat(shm_id, NULL, 0);
}

void printTimestamp() {
    time_t t;
    time(&t);
    char *t2 = ctime(&t);

    char buf[9] = {0};
    memcpy(buf, t2 + 11, 8);
    printf("%s\t", buf);
}

int main() {
    int cabsCount, ridersCount, serversCount;
    // scanf("%d%d%d", &cabsCount, &ridersCount, &serversCount);
    cabsCount = 5;
    ridersCount = 10;
    serversCount = 2;
    assert(cabsCount <= MAX_CABS);
    assert(ridersCount <= MAX_RIDERS);
    assert(serversCount <= MAX_SERVERS);

    CAB_STRING = (char **)malloc(sizeof(char *) * 2);
    CAB_STRING[0] = "POOL";
    CAB_STRING[1] = "PREMIER";

    riderConditions =
        (pthread_cond_t *)shareMem(sizeof(pthread_cond_t) * MAX_RIDERS);
    riderWaiting = (short *)shareMem(sizeof(short) * MAX_RIDERS);

    for (int i = 0; i < MAX_RIDERS; i++) {
        pthread_cond_t x = PTHREAD_COND_INITIALIZER;
        riderConditions[i] = x;
        riderWaiting[i] = -1;
    }

    serversOpenCount = serversCount;

    waitingCabs = (cab **)shareMem(sizeof(cab *) * MAX_CABS);
    poolOneCabs = (cab **)shareMem(sizeof(cab *) * MAX_CABS);
    for (int i = 0; i < cabsCount; i++) {
        waitingCabs[i] = (cab *)shareMem(sizeof(cab));
        waitingCabs[i]->id = i;
        waitingCabs[i]->state = waitState;

        printCabHead(waitingCabs[i]->id);
        printf("initialized in wait state\n");
    }
    totalCabsOpen = cabsCount;
    ridersLeftToExit = ridersCount;

    pthread_t *serverThreads =
        (pthread_t *)malloc(sizeof(pthread_t) * serversCount);
    servers = (server **)shareMem(sizeof(server *) * MAX_SERVERS);
    for (int i = 0; i < serversCount; i++) {
        servers[i] = (server *)shareMem(sizeof(server));
        servers[i]->id = i;
        pthread_create(&serverThreads[i], NULL, initServer, servers[i]);
    }

    // second argument = 0 => initialize semaphores shared between threads
    sem_init(&serversOpen, 0, 0);

    pthread_t *riderThreads =
        (pthread_t *)malloc(sizeof(pthread_t) * ridersCount);
    riders = (rider **)shareMem(sizeof(rider *) * MAX_RIDERS);
    ridersPaying = (rider **)shareMem(sizeof(rider *) * MAX_RIDERS);
    ridersPayingCount = 0;
    for (int i = 0; i < ridersCount; i++) {
        riders[i] = (rider *)shareMem(sizeof(rider));
        ridersPaying[i] = NULL;
        riders[i]->id = i;
        pthread_create(&riderThreads[i], NULL, initRider, riders[i]);
    }

    for (int i = 0; i < ridersCount; i++) {
        pthread_join(riderThreads[i], NULL);
    }
    for (int i = 0; i < serversCount; i++) {
        pthread_join(serverThreads[i], NULL);
    }

    sem_destroy(&serversOpen);

    return 0;
}