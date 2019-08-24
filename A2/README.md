# GoSH! - Gaurang's Shell

This shell is designed for the Assignment 2 of CSE.

## Features

1. Background process completion notification (when user presses enter) and shows exit code.
2. History and nightswatch
3. `ls [-al] [<Directory>]`, `cd`, `pwd`, `echo` implemented on its own.

## Files used

1. `colors.h` - defines all colors used.
2. `commands.c` - tokenizes inputs commands, checks pending command stati, and executes commands.
3. `directory.c` - implements `cd`, `pwd`, `ls`, long listing, and other directory related functions.
4. `history.c` - implements functions for reading and writing to the history file, and printing the history.
5. `main.c` - implements the event loop - taking input and giving output.
6. `pinfo.c` - implements the said command.
7. `print.c` - `echo` command
8. `process.c` - implements for dirty memory and interrupt print and for executing background/foreground process.
9. `prompt.c` - implements function for printing prompt
10. `stringers.c` - implements `trim` function
11. `sysData.c` - stores system related information like username and hostname.
12. `takeInput.c` - to take input from user using `getchar`.
13. `test.c` - sample file to test exit status.
