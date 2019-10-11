#include "table.h"

void tablePrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(TABLE_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void* initTable(void* tableTemp) {
    table* mytable = (table*)tableTemp;
    return NULL;
}

void* tablePrint() {
}

void ready_to_serve_table(int slots) {
}