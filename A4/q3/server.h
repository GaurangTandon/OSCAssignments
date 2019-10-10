#ifndef sevrer
#define sevrer
#include "common.h"

server* servers[MAX_SERVERS];

void* getNewServer(server* server);

void acceptPayment();

void makePayment();
#endif