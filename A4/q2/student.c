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

    myStud->arrivalTime = genRandomInRange(1, MAX_ARRIVAL_TIME);
    studentPrintMsg(myStud->id, "initialized with arrival time %d\n",
                    myStud->arrivalTime);
    sleep(myStud->arrivalTime);
    studentPrintMsg(myStud->id, "has arrived\n");
    studentPrintMsg(myStud->id,
                    "is waiting to be allocated a slot on the serving table\n");

    while (1) {
        for (int i = 0; i < tableCount; i++) {
            pthread_mutex_lock(&tableMutexes[i]);
            int flag = 0;

            if (tables[i]->slotsLeft > 0) {
                tables[i]->slotsLeft--;
                int j = 0;
                while (tables[i]->studentsEatingHere[j] != -1)
                    j++;
                tables[i]->studentsEatingHere[j] = myStud->id;
                flag = 1;
            }

            pthread_mutex_unlock(&tableMutexes[i]);
            if (flag) {
                studentPrintMsg(myStud->id,
                                "assigned a slot on the serving table %d and "
                                "is waiting to be served\n",
                                tables[i]->id + 1);
                return NULL;
            }
        }
    }

    return NULL;
}

void wait_for_slot() {
}

void student_in_slot() {
}