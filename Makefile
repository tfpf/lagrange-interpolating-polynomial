SHELL  = /bin/sh
CC     = g++
CFLAGS = -O2
STD    = -std=c++11
WFLAGS = -Wall -Wextra

Source     = main.cpp
Assembly   = main.s
Binary     = main.elf


.PHONY: clean asm comp exec run all


clean:
	$(RM) $(Binary) $(Assembly)

asm:
	$(CC) $(CFLAGS) $(STD) $(WFLAGS) -S -o $(Assembly) $(Source)

comp:
	$(CC) $(CFLAGS) $(STD) $(WFLAGS) -o $(Binary) $(Source)

exec:
	./$(Binary)

run: comp exec

all: asm comp exec

