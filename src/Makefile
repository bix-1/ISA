# File: Makefile
# Brief: Makefile for ISA project at FIT BUT 2021
#
# Project: POP3 client with TLS support
#
# Authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz

EXEC = popcl
FLAGS = -std=c++17 -Wall -Wextra
MODULES = connection.o options.o
CC = g++
CCMIN = g++-9.1
LOGIN = xbartk07

all: $(EXEC)

run: $(EXEC)
	./$< pop.mail.com -a auth -o out -n

$(EXEC): main.cpp $(MODULES)
	$(CC) $(FLAGS) $^ -lcrypto -lssl -o $@

$(MODULES): %.o: %.cpp %.h
	$(CC) $(FLAGS) $< -c -o $@

min: *.cpp *.h
	$(CCMIN) $(FLAGS) connection.cpp -c -o connection.o
	$(CCMIN) $(FLAGS) options.cpp -c -o options.o
	$(CCMIN) $(FLAGS) main.cpp $(MODULES) -lcrypto -lssl -o $(EXEC)

pack: $(LOGIN).tar

$(LOGIN).tar: *.cpp *.h Makefile *.pdf README
	tar -cvf $@ $^

clean:
	rm -f $(EXEC) *.o *.tar
