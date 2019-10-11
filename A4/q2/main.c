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
    int robotCount, studentCount, tableCount;
    printf("Enter robot count, student count, and table count:\n");
    scanf("%d%d%d", &robotCount, &studentCount, &tableCount);
    robotCount = 10;
    studentCount = 100;
    tableCount = 5;
    assert(robotCount <= MAX_ROBOTS);
    assert(studentCount <= MAX_STUDENTS);
    assert(tableCount <= MAX_TABLES);

    CAB_STRING = (char **)malloc(sizeof(char *) * 2);
    CAB_STRING[0] = "POOL";
    CAB_STRING[1] = "PREMIER";

    riderConditions =
        (pthread_cond_t *)shareMem(sizeof(pthread_cond_t) * MAX_STUDENTS);
    riderWaiting = (short *)shareMem(sizeof(short) * MAX_STUDENTS);

    for (int i = 0; i < MAX_STUDENTS; i++) {
        pthread_cond_t x = PTHREAD_COND_INITIALIZER;
        riderConditions[i] = x;
        riderWaiting[i] = -1;
    }

    serversOpenCount = tableCount;

    waitingCabs = (cab **)shareMem(sizeof(cab *) * MAX_ROBOTS);
    poolOneCabs = (cab **)shareMem(sizeof(cab *) * MAX_ROBOTS);
    for (int i = 0; i < robotCount; i++) {
        waitingCabs[i] = (cab *)shareMem(sizeof(cab));
        waitingCabs[i]->id = i;
        waitingCabs[i]->state = waitState;

        printCabHead(waitingCabs[i]->id);
        printf("initialized in wait state\n");
    }
    totalCabsOpen = robotCount;
    ridersLeftToExit = studentCount;

    pthread_t *serverThreads =
        (pthread_t *)malloc(sizeof(pthread_t) * tableCount);
    servers = (server **)shareMem(sizeof(server *) * MAX_TABLES);
    for (int i = 0; i < tableCount; i++) {
        servers[i] = (server *)shareMem(sizeof(server));
        servers[i]->id = i;
        pthread_create(&serverThreads[i], NULL, initServer, servers[i]);
    }

    // second argument = 0 => initialize semaphores shared between threads
    sem_init(&serversOpen, 0, 0);

    pthread_t *riderThreads =
        (pthread_t *)malloc(sizeof(pthread_t) * studentCount);
    riders = (rider **)shareMem(sizeof(rider *) * MAX_STUDENTS);
    ridersPaying = (rider **)shareMem(sizeof(rider *) * MAX_STUDENTS);
    ridersPayingCount = 0;
    for (int i = 0; i < studentCount; i++) {
        riders[i] = (rider *)shareMem(sizeof(rider));
        ridersPaying[i] = NULL;
        riders[i]->id = i;
        pthread_create(&riderThreads[i], NULL, initRider, riders[i]);
    }

    for (int i = 0; i < studentCount; i++) {
        pthread_join(riderThreads[i], NULL);
    }
    for (int i = 0; i < tableCount; i++) {
        // release the servers in case they're stuck
        sem_post(&serversOpen);
    }

    for (int i = 0; i < tableCount; i++) {
        pthread_join(serverThreads[i], NULL);
    }

    sem_destroy(&serversOpen);

    return 0;
}