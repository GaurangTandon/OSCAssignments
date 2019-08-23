CFLAGS = -g -Wall -Wextra -Wpedantic

default: main

main: main.c directory.c prompt.c sysData.c commands.c stringers.c takeInput.c print.c history.c process.c directory.h prompt.h sysData.h commands.h stringers.h takeInput.h print.h history.h process.h
	gcc $(CFLAGS) main.c directory.c prompt.c sysData.c commands.c stringers.c takeInput.c print.c history.c process.c -o main -lm
clean:
	rm main