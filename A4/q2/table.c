#include "table.h"
#include "robot.h"
#include "student.h"

void tablePrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(TABLE_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void ready_to_serve_table(table* table) {
    pthread_mutex_lock(&tableMutexes[table->id]);
    for (int i = 0; i < 10; i++)
        table->studentsEatingHere[i] = -1;
    pthread_mutex_unlock(&tableMutexes[table->id]);

    int slots;
    table->slotsLeft = (slots = genRandomInRange(1, 10));
    table->readyToServe = 1;

    tablePrintMsg(table->id, "is ready to serve with %d slots\n",
                  table->slotsLeft);

    while (!gameOver) {
        pthread_mutex_lock(&tableMutexes[table->id]);
        if (table->slotsLeft == 0)
            break;
        pthread_mutex_unlock(&tableMutexes[table->id]);
    }

    if (gameOver) {
        pthread_mutex_unlock(&tableMutexes[table->id]);
        return;
    }

    table->readyToServe = 0;
    tablePrintMsg(table->id, "entering serving phase\n");

    for (int i = 0; i < 10; i++) {
        if (table->studentsEatingHere[i] == -1)
            break;
        studentPrintMsg(table->studentsEatingHere[i],
                        "on serving table %d has been served.\n",
                        table->id + 1);
    }

    for (int i = 0; i < 10; i++)
        table->studentsEatingHere[i] = -1;

    pthread_mutex_unlock(&tableMutexes[table->id]);

    tablePrintMsg(table->id,
                  "serving container is empty, waiting for refill\n");
}

void* initTable(void* tableTemp) {
    table* mytable = (table*)tableTemp;

    tablePrintMsg(mytable->id, "initialized\n");

    mytable->biryaniAmountRemaining = 0;
    mytable->needVessel = 1;

    while (!gameOver) {
        int flag = 0;
        for (int i = 0; i < robotCount; i++) {
            pthread_mutex_lock(&robotMutexes[i]);

            if (robots[i]->biryaniVesselsRemaining > 0) {
                flag = 1;

                // TODO scammy isn't this?
                robotPrintMsg(
                    robots[i]->id,
                    "refilling serving container of serving table %d\n",
                    mytable->id + 1);
                tablePrintMsg(mytable->id,
                              "serving container refilled by robot chef %d, "
                              "resuming serving now\n",
                              robots[i]->id + 1);
                robots[i]->biryaniVesselsRemaining--;
                pthread_mutex_lock(&tableMutexes[mytable->id]);
                mytable->biryaniAmountRemaining += robots[i]->vesselSize;
                pthread_mutex_unlock(&tableMutexes[mytable->id]);
            }

            pthread_mutex_unlock(&robotMutexes[i]);

            if (flag)
                break;
        }

        if (flag)
            ready_to_serve_table(mytable);
    }

    tablePrintMsg(mytable->id, "has left the system\n");

    return NULL;
}