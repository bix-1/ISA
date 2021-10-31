# File: Makefile
# Brief: Makefile for ISA project at FIT BUT 2021
#
# Project: POP3 client with TLS support
#
# Authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz

EXEC = popcl
FLAGS = -std=c++11 -Wall -Wextra -lcrypto -lssl

all: $(EXEC)

run: $(EXEC)
	./$<

$(EXEC): main.cpp
	g++ $(FLAGS) $^ -o $@

clean:
	rm -f $(EXEC) *.o
