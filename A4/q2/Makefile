CFLAGS = -g -Wall -Wextra -Wpedantic

default: main

main: main.c robot.c robot.h common.h student.c student.h table.c table.h colors.h main.h
	gcc $(CFLAGS) main.c robot.c student.c table.c -o main -lm -pthread

clean:
	rm main
