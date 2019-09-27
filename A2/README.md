# GoSH! - Gaurang's Shell

This shell is designed for the Assignment 2 of CSE.

## Features

1. Background process completion notification (when user presses enter) and shows exit code.
2. History and nightswatch. Cronjob and up arrow.
3. `ls [-al] [<Directory>]`, `cd`, `pwd`, `echo` implemented on its own. Also setenv, unsetenv, etc.
4. Done with piping and redirection.

## Files used

1. `colors.h` - defines all colors used.
2. `commands.c` - tokenizes inputs commands, checks pending command stati, and executes commands.
3. `cronjob.c` - for parsing cronjob commands and executing them
4. `directory.c` - implements `cd`, `pwd`, `ls`, long listing, and other directory related functions.
5. `history.c` - implements functions for reading and writing to the history file, and printing the history.
6. `main.c` - implements the event loop - taking input and giving output.
7. `pinfo.c` - implements the said command.
8. `print.c` - `echo` command
9. `process.c` - implements for dirty memory and interrupt print and for executing background/foreground process.
10. `prompt.c` - implements function for printing prompt
11. `stringers.c` - implements `trim` function
12. `sysData.c` - stores system related information like username and hostname.
13. `takeInput.c` - to take input from user using `getchar`.
14. `test.c` - sample file to test exit status.
