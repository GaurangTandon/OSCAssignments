#pragma once

#include "sysData.h"

extern char currDirString[1000];
// 0 indicates root

void initDirSetup();
// return -1 on error; 0 on success
void changeDirectory(char*);
void ls(char*, int, int);
void printPWD(int);