# File: Makefile
# Brief: Makefile for ISA project at FIT BUT 2021
#
# Project: POP3 client with TLS support
#
# Authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz

EXEC = popcl
FLAGS = -std=c++17 -Wall -Wextra
MODULES = connection.o options.o

all: $(EXEC)

run: $(EXEC)
	./$< pop.mail.com -a auth -o out

$(EXEC): main.cpp $(MODULES)
	g++ $(FLAGS) $^ -lcrypto -lssl -o $@

$(MODULES): %.o: %.cpp
	g++ $(FLAGS) $< -c -o $@

clean:
	rm -f $(EXEC) *.o
