#include "table.h"
#include "robot.h"
#include "student.h"

#define min(x, y) ((x) <= (y) ? (x) : (y))

void tablePrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(TABLE_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void ready_to_serve_table(table* table) {
    pthread_mutex_lock(&tableMutexes[table->id]);
    if (gameOver)
        return;
    for (int i = 0; i < 10; i++)
        table->studentsEatingHere[i] = -1;

    int slots;
    table->slotsLeft =
        (slots = genRandomInRange(1, min(10, table->biryaniAmountRemaining)));
    table->readyToServe = 1;
    tablePrintMsg(table->id, "is ready to serve with %d slots\n",
                  table->slotsLeft);
    pthread_mutex_unlock(&tableMutexes[table->id]);

    while (1) {
        pthread_mutex_lock(&tableMutexes[table->id]);
        if (table->slotsLeft == 0) {
            tablePrintMsg(table->id, "entering serving phase\n");
            break;
        }
        pthread_mutex_unlock(&tableMutexes[table->id]);
    }

    table->readyToServe = 0;

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
}

void* initTable(void* tableTemp) {
    table* mytable = (table*)tableTemp;

    tablePrintMsg(mytable->id, "initialized\n");

    mytable->biryaniAmountRemaining = 0;
    mytable->needVessel = 1;

    while (1) {
        int flag = 0;
        for (int i = 0; i < robotCount; i++) {
            if (gameOver) {
                goto end;
            }

            pthread_mutex_lock(&robotMutexes[i]);

            if (robots[i]->biryaniVesselsRemaining > 0) {
                flag = 1;

                // TODO scammy isn't this?
                robotPrintMsg(
                    robots[i]->id,
                    "refilling serving container of serving table %d\n",
                    mytable->id + 1);
                robots[i]->biryaniVesselsRemaining--;
                pthread_mutex_lock(&tableMutexes[mytable->id]);
                tablePrintMsg(mytable->id,
                              "serving container refilled by robot chef %d, "
                              "resuming serving now\n",
                              robots[i]->id + 1);
                mytable->biryaniAmountRemaining += robots[i]->vesselSize;
                pthread_mutex_unlock(&tableMutexes[mytable->id]);
            }

            pthread_mutex_unlock(&robotMutexes[i]);

            if (flag)
                break;
        }

        if (gameOver) {
            break;
        }

        if (flag) {
            while (!gameOver) {
                pthread_mutex_lock(&tableMutexes[mytable->id]);
                int x = mytable->biryaniAmountRemaining;
                pthread_mutex_unlock(&tableMutexes[mytable->id]);

                if (x > 0)
                    ready_to_serve_table(mytable);
            }

            tablePrintMsg(mytable->id,
                          "serving container is empty, waiting for refill\n");

            if (gameOver)
                break;
        }
    }
end:
    tablePrintMsg(mytable->id, "has left the system\n");

    return NULL;
}