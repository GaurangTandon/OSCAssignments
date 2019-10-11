#include "table.h"
#include "robot.h"

void tablePrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(TABLE_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void* initTable(void* tableTemp) {
    table* mytable = (table*)tableTemp;

    mytable->biryaniAmountRemaining = 0;
    mytable->needVessel = 1;

    while (studentsLeftCount > 0) {
        for (int i = 0; i < robotCount; i++) {
            pthread_mutex_lock(&robotMutexes[i]);
            int flag = 0;

            if (robots[i]->biryaniVesselsRemaining > 0) {
                flag = 1;
                robots[i]->biryaniVesselsRemaining--;
                mytable->biryaniAmountRemaining += robots[i]->vesselSize;
            }

            pthread_mutex_unlock(&robotMutexes[i]);

            if (flag)
                ready_to_serve_table(mytable);
        }
    }

    return NULL;
}

void ready_to_serve_table(table* table) {
    for (int i = 0; i < 10; i++)
        table->studentsEatingHere[i] = -1;

    int slots;
    table->slotsLeft = (slots = genRandomInRange(1, 10));
    table->readyToServe = 1;

    while (1) {
        pthread_mutex_lock(&tableMutexes[table->id]);
        if (table->slotsLeft == 0)
            break;
        pthread_mutex_unlock(&tableMutexes[table->id]);
    }

    table->readyToServe = 0;
    tablePrintMsg("has been occupied by %d students, with ids", slots);
    for (int i = 0; i < 10; i++) {
        if (table->studentsEatingHere[i] == -1)
            break;
        printf(" %d", table->studentsEatingHere[i]);
    }

    // need a mutex here
    studentsLeftCount -= slots;

    printf("\n");
}