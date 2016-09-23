// Evan Heaton
// 09/22/2016
// AI Satisfiability Programming Assignment
// DPLL helper functions

#include <iostream>
#include <string>
#include <vector>
#include "DPLL.h"

using namespace std;

// returns true if the solution is a consistent set of literals (i.e. no literal AND its negation exists), false otherwise
bool is_consistent(vector<vector<int> > solution) {
	return false;
}

// returns true if one of the clauses in the solution is empty, false otherwise
bool contains_empty_clause(vector<vector<int> > solution) {
	for (int i=0; i<solution.size(); i++) {
		if (solution[i].size() == 0)
			return true;
	}
	return false;
}

// returns an updated solution with the following properties:
// literal is set to true
// clauses with literal are removed (as they are satisfied)
// any negation of literal is removed in any clause in solution
vector<vector<int> > unit_propogate(vector<vector<int> > solution, int literal) {
	return solution;
}

// returns an updated solution with the following properties:
// all clauses with literal are removed (as they are satisfied)
vector<vector<int> > pure_literal_assign(vector<vector<int> > solution, int literal) {
	return solution;
}
