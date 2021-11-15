# File: Makefile
# Brief: Makefile for ISA project at FIT BUT 2021
#
# Project: POP3 client with TLS support
#
# Authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz

EXEC = popcl
FLAGS = -std=c++17 -Wall -Wextra
MODULES = connection.o options.o
CXX = g++
LOGIN = xbartk07

all: $(EXEC)

run: $(EXEC)
	./$< pop.mail.com -a auth -o out -n

$(EXEC): main.cpp $(MODULES)
	$(CXX) $(FLAGS) $^ -lcrypto -lssl -o $@

$(MODULES): %.o: %.cpp %.h
	$(CXX) $(FLAGS) $< -c -o $@

pack: $(LOGIN).tar

$(LOGIN).tar: *.cpp *.h Makefile *.pdf README
	tar -cvf $@ $^

clean:
	rm -f $(EXEC) *.o *.tar
