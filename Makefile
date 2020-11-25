SHELL  = /bin/sh
CC     = g++
CFLAGS = -ggdb3 -O2 -std=c++11 -Wall -Wextra

Source = main.cpp
Binary = main

input = input.txt


.PHONY: clean comp exec run all


comp:
	$(CC) $(CFLAGS) -o $(Binary) $(Source)

exec:
	./$(Binary) $(input)

run: comp exec

