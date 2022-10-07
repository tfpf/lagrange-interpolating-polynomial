SHELL  = /bin/sh
CC     = g++
CPPFLAGS = -O2 -std=c++17 -Wall -Wextra -Wpedantic -I./include
RM     = rm -f

Sources    = $(wildcard lib/*.cc)
Objects    = $(Sources:.cc=.o)
Executable = main

.PHONY: clean

$(Executable): $(Objects)
	$(LINK.cc) -o $(Executable) $(Objects)

clean:
	$(RM) $(Objects) $(Executable)
