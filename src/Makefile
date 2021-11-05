# File: Makefile
# Brief: Makefile for ISA project at FIT BUT 2021
#
# Project: POP3 client with TLS support
#
# Authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz

EXEC = popcl
FLAGS = -std=c++17 -Wall -Wextra

all: $(EXEC)

run: $(EXEC)
	./$<

$(EXEC): main.cpp connection.o
	g++ $(FLAGS) $^ -lcrypto -lssl -o $@

connection.o: connection.cpp connection.h
	g++ $(FLAGS) $< -c -o $@

clean:
	rm -f $(EXEC) *.o
