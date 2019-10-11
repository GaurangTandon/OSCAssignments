#ifndef riderss
#define riderss
#include "common.h"

rider** riders;
rider** ridersPaying;

void* initRider(void* rider);
void madePayment(rider* rider);

#endif