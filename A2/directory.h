#pragma once

#include "sysData.h"

char expectedHomeDir[1000];
extern char currDirString[1000];
// 0 indicates root

void initDirSetup(int updateHome);
// return -1 on error; 0 on success
void changeDirectory(char*);
void ls(char* directory, int showHidden, int longListing);
void printPWD(int printNewline, int printAbs);