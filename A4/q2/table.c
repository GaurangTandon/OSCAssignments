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

    while (1) {
        pthread_mutex_lock(&updateMutex);
        pthread_cond_wait(&tableConditions[mytable->id], &updateMutex);

        // some chef sent me a signal
        // that means he is giving me biryani :D
        for (int i = 0; i < robotCount; i++) {
            if (robots[i]->biryaniVesselsRemaining > 0) {
                robots[i]->biryaniVesselsRemaining--;
                mytable->needVessel = 0;
                mytable->biryaniAmountRemaining += robots[i]->vesselSize;
                pthread_mutex_unlock(&updateMutex);
                ready_to_serve_table();
            }
        }
    }

    return NULL;
}

void* tablePrint() {
}

void ready_to_serve_table() {
}