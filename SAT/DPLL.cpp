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
#include "other_helpers.h"

using namespace std;

bool DPLL(vector<vector<int> > formula, int nbvar) {
	srand ((int)time(NULL)); // seed the random number generator

	if (is_consistent(formula, nbvar)) {
		// TO DO: for each literal in formula, set solution[abs(literal)] = sign(literal)
		return true;
	}
	if (contains_empty_clause(formula))
		return false;
	for (int i=0; i<formula.size(); i++) {
		if (formula[i].size() == 1) {
			// TO DO: set solution[formula[i][0] - 1] to sign(formula[i][0])
			formula = unit_propogate(formula, formula[i][0]);
			i = -1; //reset i, in case a unit clause was created near the beginning of the formula.
		}
	}
	vector<int> pure_literals = get_pure_literals(formula, nbvar);
	for (int i=0; i<pure_literals.size(); i++) {
		if (pure_literals[i]) {
			// TO DO: set solution[i] to 1
			formula = pure_literal_assign(formula, i+1);
		}
	}

	if (contains_empty_clause(formula))
		return false;
	if (is_consistent(formula, nbvar)) // add another check for consistency, in case it was reached by the pure literal assign/unit propogate
		return true;

	int literal = choose_literal(formula);

	vector<vector<int> > formula_1 = formula;
	vector<vector<int> > formula_2 = formula; // Make 2 more formulas. _1 will have unit clause of literal, _2 will have unit clause of !literal
	vector<int> positive_unit_literal;
	vector<int> negative_unit_literal;
	positive_unit_literal.push_back(literal);
	negative_unit_literal.push_back(-literal);
	formula_1.push_back(positive_unit_literal);
	formula_2.push_back(negative_unit_literal);

	return DPLL(formula_1, nbvar) || DPLL(formula_2, nbvar); // branch!

}


// returns true if the formula is a consistent set of literals (i.e. no literal AND its negation exists), false otherwise
bool is_consistent(vector<vector<int> > formula, int nbvar) {
	vector<int> var_states;
	for (int i=0; i<nbvar; i++)
		var_states.push_back(0);
	//var_states[literal] is 0 if that literal has not been found yet
	//var_states[literal] is -1 if the negation of that literal has been found
	//var_states[literal] is 1 if that literal has been found
	for (int i=0; i<formula.size(); i++) {
		for (int j=0; j<formula[i].size(); j++) {
			if (var_states[abs(formula[i][j])-1] == 0) {
				var_states[abs(formula[i][j])-1] = sign(formula[i][j]);
			} else if (sign(var_states[abs(formula[i][j])-1]) != sign(formula[i][j])) {
				return false;
			}
		}
	}
	return true;
}

// returns true if one of the clauses in the formula is empty, false otherwise
bool contains_empty_clause(vector<vector<int> > formula) {
	for (int i=0; i<formula.size(); i++) {
		if (formula[i].size() == 0)
			return true;
	}
	return false;
}

// returns an updated formula with the following properties:
// literal is set to true
// clauses with literal are removed (as they are satisfied)
// any negation of literal is removed in any clause in formula
vector<vector<int> > unit_propogate(vector<vector<int> > formula, int literal) {
	for (int i=0; i<formula.size(); i++) {
		for (int j=0; j<formula[i].size(); j++) {
			if (formula[i][j] == -literal) { //remove !literals
				formula[i].erase(formula[i].begin()+j);
				j--; //decrement j to stay in bounds. Once formula[i][j] has been removed, the NEXT literal we want to look at is still formula[i][j]
			} else if (formula[i][j] == literal) { //remove clauses containing literal
				formula.erase(formula.begin()+i);
				i--; //decrement i to stay in bounds.
				break;
			}
		}
	}
	return formula;
}

// returns an updated formula with the following properties:
// all clauses with literal are removed (as they are satisfied)
vector<vector<int> > pure_literal_assign(vector<vector<int> > formula, int literal) {
	for (int i=0; i<formula.size(); i++) {
		for (int j=0; j<formula[i].size(); j++) {
			if (formula[i][j] == literal) {
				formula.erase(formula.begin()+i);
				i--; // i should not increment this iteration
				break;
			}
		}
	}
	return formula;
}

// returns a vector of all pure literals in the formula.
// pure_literals[0] represents literal 1, pure_literals[1] represents literal 2, etc...
vector<int> get_pure_literals(vector<vector<int> > formula, int nbvar) {
	vector<int> pure_literals;
	for (int i=0; i<nbvar; i++)
		pure_literals.push_back(1); // a 1 represents a pure literal. All literals are pure until proven otherwise

	for (int i=0; i<formula.size(); i++) {
		for (int j=0; j<formula[i].size(); j++) {
			if (sign(formula[i][j]) == -1) { // not pure!
				pure_literals[abs(formula[i][j])-1] = 0;
			}
		}
	}
	return pure_literals;
}

// returns -1 for a negative number, 1 for a positive number, 0 for 0
int sign(int input) {
	if (input > 0)
		return 1;
	else if (input < 0)
		return -1;
	else
		return 0;
}

//returns a random literal that exists in formula
int choose_literal(vector<vector<int> > formula) {
	if (formula.size() == 0) {
		return 0;
	}
	vector<int> unique_literals;
	for (int i=0; i<formula.size(); i++) {
		for (int j=0; j<formula[i].size(); j++) { //each literal in formula
			bool unique = true;
			for (int k=0; k<unique_literals.size(); k++) {
				if (unique_literals[k] == abs(formula[i][j])) { //if the literal already exists in unique_literals
					unique = false;
					break;
				}
			}
			if (unique)
				unique_literals.push_back(abs(formula[i][j]));
		}
	}

	int random_index = rand() % unique_literals.size();
	return unique_literals[random_index];
}
