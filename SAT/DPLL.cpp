// Evan Heaton
// 09/22/2016
// AI Satisfiability Programming Assignment
// DPLL helper functions

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>  //for abs(), rand(), srand()
#include <time.h>
#include "DPLL.h"

using namespace std;

bool DPLL(vector<vector<int> > solution, int nbvar) {
	srand ((int)time(NULL)); // seed the random number generator

	if (is_consistent(solution, nbvar))
		return true;
	if (contains_empty_clause(solution))
		return false;
	for (int i=0; i<solution.size(); i++) {
		if (solution[i].size() == 1) {
			// TO DO: set solution[i][0] to true
			solution = unit_propogate(solution, solution[i][0]);
			i = -1; //reset i, in case a unit clause was created near the beginning of the solution.
		}
	}
	vector<int> pure_literals = get_pure_literals(solution, nbvar);
	for (int i=1; i<pure_literals.size(); i++) {
		if (pure_literals[i]) {
			solution = pure_literal_assign(solution, i);
		}
	}
	int literal = choose_literal(solution);

	vector<vector<int> > solution_1 = solution;
	vector<vector<int> > solution_2 = solution; // Make 2 more solutions. _1 will have unit clause of literal, _2 will have unit clause of !literal
	vector<int> literal_1;
	vector<int> literal_2;
	literal_1.push_back(literal);
	literal_2.push_back(-literal);
	solution_1.push_back(literal_1);
	solution_2.push_back(literal_2);

	return DPLL(solution_1, nbvar) || DPLL(solution_2, nbvar); // branch!

}


// returns true if the solution is a consistent set of literals (i.e. no literal AND its negation exists), false otherwise
bool is_consistent(vector<vector<int> > solution, int nbvar) {
	vector<int> var_states;
	for (int i=0; i<=nbvar; i++)
		var_states.push_back(0);
	//var_states[literal] is 0 if that literal has not been found yet
	//var_states[literal] is -1 if the negation of that literal has been found
	//var_states[literal] is 1 if that literal has been found
	for (int i=0; i<solution.size(); i++) {
		for (int j=0; j<solution[i].size(); j++) {
			if (var_states[solution[i][j]] == 0) {
				var_states[solution[i][j]] = sign(solution[i][j]);
			} else if (sign(var_states[solution[i][j]]) != sign(solution[i][j])) {
				return false;
			}
		}
	}
	return true;
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
	for (int i=0; i<solution.size(); i++) {
		for (int j=0; j<solution[i].size(); j++) {
			if (solution[i][j] == -literal) { //remove !literals
				solution[i].erase(solution[i].begin()+j);
				j--; //decrement j to stay in bounds. Once solution[i][j] has been removed, the NEXT literal we want to look at is still solution[i][j]
			} else if (solution[i][j] == literal) { //remove clauses containing literal
				solution.erase(solution.begin()+i);
				i--; //decrement i to stay in bounds.
				break;
			}
		}
	}
	return solution;
}

// returns an updated solution with the following properties:
// all clauses with literal are removed (as they are satisfied)
vector<vector<int> > pure_literal_assign(vector<vector<int> > solution, int literal) {
	for (int i=0; i<solution.size(); i++) {
		for (int j=0; j<solution[i].size(); j++) {
			if (solution[i][j] == literal) {
				solution.erase(solution.begin()+i);
				i--;
				break;
			}
		}
	}
	return solution;
}

// returns a vector of all pure literals in the solution.
vector<int> get_pure_literals(vector<vector<int> > solution, int nbvar) {
	vector<int> pure_literals;
	pure_literals.push_back(0); //pure_literals[0] = 0;
	for (int i=0; i<nbvar; i++)
		pure_literals.push_back(1); // a 1 represents a pure literal. All literals are pure until proven otherwise

	for (int i=0; i<solution.size(); i++) {
		for (int j=0; j<solution[i].size(); j++) {
			if (sign(solution[i][j]) == -1) // not pure!
				pure_literals[solution[i][j]] = 0;
		}
	}
	return pure_literals;
}

int sign(int input) {
	if (input > 0)
		return 1;
	else if (input < 0)
		return -1;
	else
		return 0;
}

//returns a random literal that exists in solution
int choose_literal(vector<vector<int> > solution) {
	vector<int> unique_literals;
	for (int i=0; i<solution.size(); i++) {
		for (int j=0; j<solution[i].size(); j++) { //each literal in solution
			bool unique = true;
			for (int k=0; k<unique_literals.size(); k++) {
				if (unique_literals[k] == abs(solution[i][j])) { //if the literal already exists in unique_literals
					unique = false;
					break;
				}
			}
			if (unique)
				unique_literals.push_back(abs(solution[i][j]));
		}
	}

	int random_index = rand() % unique_literals.size();
	return unique_literals[random_index];
}
