int foregroundProcId;
char* pendingNames[100];
int pendingIDs[100];
int pendingCount;
char** tokenizeCommands(char*, int*);
void execCommand(char* command);
int keyboardWasPressed();