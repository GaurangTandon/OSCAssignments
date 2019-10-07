CFLAGS = -g -Wall -Wextra -Wpedantic

default: main

main: main.c rider.c rider.h common.h driver.c driver.h server.c server.h
	gcc $(CFLAGS) main.c rider.c driver.c server.c -o main -lm -pthread

clean:
	rm main
