#include "server.h"
#include "rider.h"

void serverPrintMsg(int id, char* fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    printMsg(SERVER_TYPE, id, fmt, argptr);
    va_end(argptr);
}

void* initServer(void* serverTemp) {
    server* myserver = (server*)serverTemp;
    serverPrintMsg(myserver->id, "initialized\n");

    while (ridersLeftToExit) {
        int x = sem_wait(&serversOpen);
        if (x < 0) {
            perror("Couldn't sem wait in server");
            break;
        }

        if (!ridersLeftToExit)
            break;

        pthread_mutex_lock(&checkPayment);
        if (!ridersPaying[0]) {
            pthread_mutex_unlock(&checkPayment);
            break;
        }
        rider* r = ridersPaying[ridersPayingCount - 1];
        ridersPaying[ridersPayingCount - 1] = NULL;
        ridersPayingCount -= 1;
        pthread_mutex_unlock(&checkPayment);
        serverPrintMsg(myserver->id, "accepting payment from %d\n", r->id + 1);

        sleep(2);

        serverPrintMsg(myserver->id, "received payment from %d\n", r->id + 1);
        madePayment(r);
    }

    serverPrintMsg(myserver->id, "All riders left, so closing down\n");

    return NULL;
}