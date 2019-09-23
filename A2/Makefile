CFLAGS = -g -Wall -Wextra -Wpedantic

default: main

main: main.c directory.c pinfo.c prompt.c sysData.c commands.c stringers.c takeInput.c print.c history.c process.c cronjob.c directory.h prompt.h sysData.h commands.h stringers.h takeInput.h print.h history.h process.h pinfo.h cronjob.h
	gcc $(CFLAGS) main.c pinfo.c directory.c prompt.c sysData.c commands.c stringers.c takeInput.c print.c history.c process.c cronjob.c -o main -lm
clean:
	rm main
