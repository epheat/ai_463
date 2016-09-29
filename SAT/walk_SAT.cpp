// Evan Heaton
// 09/28/2016
// AI Satisfiability Programming Assignment
// walkSAT helper functions

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>  //for abs(), rand(), srand()
#include <time.h>
#include "walk_SAT.h"
#include "hill_climbing.h"

using namespace std;

vector<int> walk_sat(vector<vector<int> > formula, int nbvar, bool &satisfiable) {
  int max_restarts = 11; // If your algorithm uses randomness, do 10 runs per formula.
  vector<int> solution;
  while (max_restarts-- > 0 && !satisfiable) {

    solution = generate_solution(nbvar);
    for (int i=0; i<100; i++) { // Do 100 alterations to the solution, looking for a proper solution. I chose 100 arbitrarily

      if (get_fitness(solution, formula) == formula.size()) { // Have we satisfied everything yet??
        satisfiable = true;
        return solution;
      } else {
        int random_0_9 = rand() % 10;
        if (random_0_9 < 2) { // with 20% probability
          solution = mutate_solution(solution);
        } else {
          // randomly choose a clause that is not satisfied
          int chosen_clause_index = choose_unsatisfied_clause(solution, formula);
          // then, choose literal in that clause that, if flipped, would make the LEAST amount of other clauses unsatisfied
          int chosen_literal = choose_literal_to_flip(solution, formula, chosen_clause_index);
          // flip that literal
          solution[chosen_literal-1] = !solution[chosen_literal-1];
        }
      }
    }
  }
  return solution;
}

// returns the index of a random clause in [formula] that is not satisfied
int choose_unsatisfied_clause(vector<int> solution, vector<vector<int> > formula) {
  vector<int> potential_indices;
	for (int i=0; i<formula.size(); i++) {
		bool found_correct_literal = false;
		for (int j=0;j<formula[i].size(); j++) {
			if (formula[i][j] > 0) { // positive literal
				if (solution[formula[i][j]-1])
					found_correct_literal = true;
			} else { // negative literal
				if (!solution[-formula[i][j]-1])
					found_correct_literal = true;
			}
		}
		if (!found_correct_literal)
			potential_indices.push_back(i);
	}
  int random_index = potential_indices[rand() % potential_indices.size()];
	return random_index;
}

// returns the literal from a particular clause that, when flipped, would make the minimum number of other clauses become false
int choose_literal_to_flip(vector<int> solution, vector<vector<int> > formula, int clause_index) {
  int literal_with_lowest_unsat = 0;
  int lowest_unsat = formula.size(); // set the lowest number of unsatisfied clauses seen so far to the maximum it could conceivably be
  for (int i=0; i<formula[clause_index].size(); i++) { // loop through each literal in the particular clause
    vector<int> solution_with_literal_flipped = solution;
    solution_with_literal_flipped[abs(formula[clause_index][i])-1] = !solution_with_literal_flipped[abs(formula[clause_index][i])-1]; // flip the literal in the test solution
    int current_unsat = get_number_of_unsatisfied_clauses(solution_with_literal_flipped, formula);
    if (current_unsat < lowest_unsat) {
      lowest_unsat = current_unsat;
      literal_with_lowest_unsat = abs(formula[clause_index][i]);
    }
  }
  return literal_with_lowest_unsat;
}

// returns the number of unsatisfied clauses in [formula] with solution [solution]
int get_number_of_unsatisfied_clauses(vector<int> solution, vector<vector<int> > formula) {
  int unsat = 0;
	for (int i=0; i<formula.size(); i++) {
		bool found_correct_literal = false;
		for (int j=0;j<formula[i].size(); j++) {
			if (formula[i][j] > 0) { // positive literal
				if (solution[formula[i][j]-1])
					found_correct_literal = true;
			} else { // negative literal
				if (!solution[-formula[i][j]-1])
					found_correct_literal = true;
			}
		}
		if (!found_correct_literal)
			unsat++;
	}
	return unsat;
}
