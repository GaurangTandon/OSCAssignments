#include "student.h"
#include "table.h"

void studentPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(STUDENT_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void* initStudent(void* stTemp) {
    student* myStud = (student*)stTemp;

    myStud->arrivalTime = genRandomInRange(1, 100);
    sleep(myStud->arrivalTime);

    while (1) {
        for (int i = 0; i < tableCount; i++) {
            pthread_mutex_lock(&tableMutexes[i]);

            if (tables[i]->slotsLeft > 0) {
                tables[i]->slotsLeft--;
            }

            pthread_mutex_unlock(&tableMutexes[i]);
        }
    }

    return NULL;
}

void wait_for_slot() {
}

void student_in_slot() {
}