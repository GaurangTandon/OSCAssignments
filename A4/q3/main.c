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

char *getTimestamp() {
    time_t t;
    time(&t);
    char *t2 = ctime(&t);

    char *buf = (char *)calloc(sizeof(char), 9);
    memcpy(buf, t2 + 11, 8);
    return buf;
}

char *getHeader(int type, int id) {
    char *buf = (char *)calloc(sizeof(char), 1000);
    strcat(buf, getTimestamp());
    strcat(buf, "\t");

    switch (type) {
        case CAB_TYPE:
            strcat(buf, KGREEN "Cab");
            break;
        case RIDER_TYPE:
            strcat(buf, KMAGENTA "Rider");
            break;
        case SERVER_TYPE:
            strcat(buf, KBLUE "Server");
            break;
    }

    char b2[10] = {0};
    // alignment woes
    if (type == SERVER_TYPE)
        sprintf(b2, " %d\t" KNRM, id + 1);
    else
        sprintf(b2, " %d\t\t" KNRM, id + 1);
    strcat(buf, b2);
    return buf;
}

void printMsg(int type, int id, char *fmt, va_list arg) {
    char *buf = (char *)calloc(sizeof(char), 1000);
    char buf2[1000] = {0};
    vsprintf(buf, fmt, arg);

    strcat(buf2, getHeader(type, id));
    strcat(buf2, buf);

    printf("%s", buf2);
}

int main() {
    srand(time(0));

    int cabsCount, ridersCount, serversCount;
    printf("Enter cab count, rider count, server count:\n");
    scanf("%d%d%d", &cabsCount, &ridersCount, &serversCount);
    // cabsCount = 10;
    // ridersCount = 100;
    // serversCount = 5;
    // TODO this test fails :( also this 5 5 5
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

        cabPrintMsg(waitingCabs[i]->id, "initialized in wait state\n");
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

    printf("DEBUG: student threads joined\n");

    for (int i = 0; i < serversCount; i++) {
        // release the servers in case they're stuck
        int x = sem_post(&serversOpen);
        if (x == -1)
            perror("a");
        x = sem_post(&serversOpen);
        if (x == -1)
            perror("a");
    }

    for (int i = 0; i < serversCount; i++) {
        int x;
        x = sem_post(&serversOpen);
        if (x == -1)
            perror("a");
        pthread_join(serverThreads[i], NULL);
    }

    sem_destroy(&serversOpen);

    return 0;
}