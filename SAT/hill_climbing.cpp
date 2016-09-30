// Evan Heaton
// 09/27/2016
// AI Satisfiability Programming Assignment
// hill climbing helper functions

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>  //for abs(), rand(), srand()
#include <time.h>
#include "hill_climbing.h"
#include "other_helpers.h"

using namespace std;

vector<int> repeated_hill_climbing(vector<vector<int> > formula, int nbvar, bool &satisfiable) {

	vector<int> solution;
	int max_restarts = 501; // If your algorithm uses randomness, do 10 runs per formula.

	while (max_restarts-- > 0 && !satisfiable) { // do hill climbing 200 (199?) times or until a proper solution is found
		solution = hill_climbing(formula, nbvar, satisfiable);
	}
	return solution;
}


vector<int> hill_climbing(vector<vector<int> > formula, int nbvar, bool &satisfiable) {
	vector<int> solution = generate_solution(nbvar);
	int current_fitness = get_fitness(solution, formula);
	bool better_neighbors_exist = true;

	while (better_neighbors_exist) {

		current_fitness = get_fitness(solution, formula);
		vector<vector<int> > neighbor_list = get_neighbors(solution);

		int index_of_first_neighbor_with_best_fitness = -1;
		for (int i=0; i<neighbor_list.size(); i++) {
			if (get_fitness(neighbor_list[i], formula) > current_fitness) { // if we've found a better solution
				current_fitness = get_fitness(neighbor_list[i], formula); // set the fitness to this new best fitness
				index_of_first_neighbor_with_best_fitness = i;
			}
		}

		if (index_of_first_neighbor_with_best_fitness == -1) { // meaning, no better neighbor was found
			better_neighbors_exist = false;
		} else {
			solution = neighbor_list[index_of_first_neighbor_with_best_fitness];
		}
	}

	if (get_fitness(solution, formula) == formula.size()) {
		satisfiable = true;
		return solution;
	} else {
		// cout << "Best solution found was: " << vec_to_string(solution) << " with fitness " << current_fitness << "\n";
		return solution;
	}
}

// returns a vector of all neighbors to solution
vector<vector<int> > get_neighbors(vector<int> solution) {
	vector<vector<int> > neighbor_list;
	for (int i=0; i<solution.size(); i++) {
		vector<int> neighbor = solution;
		neighbor[i] = !neighbor[i];
		neighbor_list.push_back(neighbor);
	}
	return neighbor_list;
}
