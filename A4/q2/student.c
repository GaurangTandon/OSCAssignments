#include "student.h"

void studentPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(STUDENT_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void* initStudent(void* stTemp) {
    student* myStud = (student*)stTemp;
    return NULL;
}

void wait_for_slot() {
}

void student_in_slot() {
}