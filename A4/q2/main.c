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
    srand(time(0));

    int robotCount, studentCount, tableCount;
    printf("Enter robot count, student count, and table count:\n");
    scanf("%d%d%d", &robotCount, &studentCount, &tableCount);
    assert(robotCount <= MAX_ROBOTS);
    assert(studentCount <= MAX_STUDENTS);
    assert(tableCount <= MAX_TABLES);

    tables = (table **)shareMem(MAX_TABLES);
    robots = (robot **)shareMem(MAX_ROBOTS);
    students = (student **)shareMem(MAX_STUDENTS);

    pthread_t *tableThreads =
        (pthread_t *)malloc(sizeof(pthread_t) * tableCount);
    pthread_t *studentThreads =
        (pthread_t *)malloc(sizeof(pthread_t) * studentCount);
    pthread_t *robotThreads =
        (pthread_t *)malloc(sizeof(pthread_t) * robotCount);

    for (int i = 0; i < tableCount; i++) {
        tables[i] = (table *)shareMem(sizeof(table));
        tables[i]->id = i;
        pthread_create(&tableThreads[i], NULL, initTable, tables[i]);
    }

    for (int i = 0; i < robotCount; i++) {
        robots[i] = (robot *)shareMem(sizeof(robot));
        robots[i]->id = i;
        pthread_create(&robotThreads[i], NULL, initRobot, robots[i]);
    }

    for (int i = 0; i < studentCount; i++) {
        students[i] = (student *)shareMem(sizeof(student));
        students[i]->id = i;
        pthread_create(&studentThreads[i], NULL, initStudent, students[i]);
    }

    for (int i = 0; i < studentCount; i++) {
        pthread_join(studentThreads[i], NULL);
    }

    for (int i = 0; i < studentCount; i++) {
        pthread_join(tableThreads[i], NULL);
    }

    for (int i = 0; i < studentCount; i++) {
        pthread_join(robotThreads[i], NULL);
    }

    return 0;
}