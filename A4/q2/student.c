#include "student.h"
#include "table.h"

void studentPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(STUDENT_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void student_in_slot(student* myStud, table* t) {
    studentPrintMsg(myStud->id,
                    "assigned a slot on the serving table %d and "
                    "is waiting to be served\n",
                    t->id + 1);
    int j = 0;
    while (t->studentsEatingHere[j] != -1)
        j++;
    t->studentsEatingHere[j] = myStud->id;
    pthread_mutex_unlock(&tableMutexes[t->id]);
}

void wait_for_slot(student* myStud) {
    while (1) {
        for (int i = 0; i < tableCount; i++) {
            pthread_mutex_lock(&tableMutexes[i]);
            int flag = 0;

            if (tables[i]->slotsLeft > 0) {
                tables[i]->slotsLeft--;

                flag = 1;
            }

            if (flag) {
                student_in_slot(myStud, tables[i]);
                return;
            }

            pthread_mutex_unlock(&tableMutexes[i]);
        }
    }
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

    wait_for_slot(myStud);

    return NULL;
}
