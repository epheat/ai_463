# Evan Heaton
# 09/22/2016
# AI Satisfiability Programming Assignment
# Makefile

OBJECTS = satisfiability.o DPLL.o hill_climbing.o walk_SAT.o other_helpers.o
CXX = g++
CXXFLAGS = -Wall -g

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o satisfiability

satisfiability.o: satisfiability.cpp DPLL.h
	$(CXX) $(CXXFLAGS) -c satisfiability.cpp

DPLL.o: DPLL.cpp DPLL.h
	$(CXX) $(CXXFLAGS) -c DPLL.cpp

hill_climbing.o: hill_climbing.cpp hill_climbing.h
	$(CXX) $(CXXFLAGS) -c hill_climbing.cpp

walk_SAT.o: walk_SAT.cpp walk_SAT.h
	$(CXX) $(CXXFLAGS) -c walk_SAT.cpp

other_helpers.o: other_helpers.cpp other_helpers.h
	$(CXX) $(CXXFLAGS) -c other_helpers.cpp

zip:
	zip evan_heaton_satisfiability *.cpp *.h *.md *.png

clean:
	rm -f *.zip *.o satisfiability
