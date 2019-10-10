#ifndef sevrer
#define sevrer
#include "common.h"

server* servers[MAX_SERVERS];

void* initServer(void* server);

void acceptPayment();

void makePayment();
#endif