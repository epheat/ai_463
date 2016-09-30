// Evan Heaton
// 09/29/2016
// AI Satisfiability Programming Assignment
// other helper functions

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// takes a vector<int> as an argument
// returns a string of that vector, mostly used for debugging purposes
string vec_to_string(vector<int> v) {
	if (v.size() == 0) {
		return "<>";
	}
	stringstream ss;
	ss << "<";
	for (int i=0; i<v.size()-1; i++) {
		ss << v[i] << ", ";
	}
	ss << v[v.size()-1] << ">";
	return ss.str();
}

// takes a vector<vector<int>> as an argument
// returns a string of that vector, mostly used for debugging purposes
string vec_to_string(vector<vector<int> > v) {
	if (v.size() == 0) {
		return "<>";
	}
	stringstream ss;
	ss << "<\t";
	for (int i=0; i<v.size()-1; i++) {
		ss << vec_to_string(v[i]) << "\n\t";
	}
	ss << vec_to_string(v[v.size()-1]) << "\t>";
	return ss.str();
}

// returns the fitness of solution in the range [0, formula.size()]
int get_fitness(vector<int> solution, vector<vector<int> > formula) {
	int fitness = 0;
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
		if (found_correct_literal)
			fitness++;
	}
	return fitness;
}

// returns a solution with a single random bit flipped
vector<int> mutate_solution(vector<int> solution) {
	if (solution.size() == 0)
		return solution;
	else {
		int random_index = rand() % solution.size();
		solution[random_index] = !solution[random_index]; //flip a random bit
		return solution;
	}
}

// generates a random assignment of trues and falses for every literal in the formula
vector<int> generate_solution(int nbvar) {
	vector<int> solution;
	for (int i=0; i<nbvar; i++) {
		solution.push_back(rand() % 2);
	}
	return solution;
}
