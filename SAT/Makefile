# Evan Heaton
# 09/22/2016
# AI Satisfiability Programming Assignment
# Makefile

OBJECTS = satisfiability.o DPLL.o
CXX = g++
CXXFLAGS = -Wall -g

all: satisfiability.o DPLL.o
	$(CXX) $(CXXFLAGS) satisfiability.o DPLL.o -o satisfiability

satisfiability.o: satisfiability.cpp DPLL.h
	$(CXX) $(CXXFLAGS) -c satisfiability.cpp

DPLL.o: DPLL.cpp DPLL.h
	$(CXX) $(CXXFLAGS) -c DPLL.cpp 

zip:
	zip evan_heaton_satisfiability *.cpp *.h *.md *.png

clean:
	rm *.zip *.o satisfiability
