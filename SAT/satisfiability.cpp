// Evan Heaton
// 09/22/2016
// AI Satisfiability Programming Assignment

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>  //for abs(), rand(), srand()
#include <time.h>
#include "DPLL.h"
#include "hill_climbing.h"

using namespace std;

//prototype some functions
vector<vector<int> > get_test_case_vector(bool easy, int test_case, int &nbvar, int &nbclauses);
string vec_to_string(vector<int> v);
string vec_to_string(vector<vector<int> > v);


int main() {
	srand ((int)time(NULL)); // seed the random number generator

	int nbvar = 0, nbclauses = 0;

  cout << "hellooooo";

	vector<vector<int> > formula = get_test_case_vector(false, 10, nbvar, nbclauses);

	cout << vec_to_string(formula) << "\n";

	hill_climbing(formula, nbvar);


	// I'll fix DPLL later I guess...
	// cout << DPLL(formula, nbvar);

	// for (int i=0; i<10;i++) {
		vector<int> solution = generate_solution(nbvar);

	// }


	return 0;
}


// takes as arguments bool easy and int test_case
// if easy is true, it will return an easy test case. If false, returns a hard test case.
// test_case should be a number [1-100] representing which test case to get.
// returns a vector of vector<int>s, each vector<int> is a clause in the solution
vector<vector<int> > get_test_case_vector(bool easy, int test_case, int &nbvar, int &nbclauses) {
	ifstream in;
	stringstream ss;
	if (easy)
		ss << "./SATinstances/easy/" << test_case << ".cnf";
	else
		ss << "./SATinstances/hard/" << test_case << ".cnf";

	string file_to_open = ss.str();

	in.open(file_to_open);

	string extracted, cnf;
	nbvar = 0;
	nbclauses = 0;

	//this while loop ignores all comments and extracts nbvar and nbclauses from the cnf file
	while (nbclauses == 0) {
		in >> extracted;

		//if there's a c, it's a comment line. So ignore it.
		if (extracted == "c")
			in.ignore(256,'\n');
		else if (extracted == "p") {
			in >> cnf;
			in >> nbvar;
			in >> nbclauses;
		}
	}

	vector<vector<int> > solution;

	//extract each clause and put it into the "solution" vector of clause vectors
	for (int i=0; i<nbclauses; i++) {
		vector<int> clause;
		int literal = 1; //set to 1 initially to enter loop

		//a zero denotes the end of a clause, so extract literals until we see a 0
		while (literal != 0) {
			in >> literal;
			if (literal)
				clause.push_back(literal);
		}
		solution.push_back(clause);
	}
	return solution;
}

/*
// takes a vector<int> as an argument
// returns a string of that vector, mostly used for debugging purposes
string vec_to_string(vector<int> v) {
	stringstream ss;
	ss << "<";
	for (int i=0; i<v.size()-1; i++) {
		ss << v[i] << ", ";
	}
	ss << v[v.size()-1] << ">";
	return ss.str();
}
*/

// takes a vector<vector<int>> as an argument
// returns a string of that vector, mostly used for debugging purposes
string vec_to_string(vector<vector<int> > v) {
	stringstream ss;
	ss << "<\t";
	for (int i=0; i<v.size()-1; i++) {
		ss << vec_to_string(v[i]) << "\n\t";
	}
	ss << vec_to_string(v[v.size()-1]) << "\t>";
	return ss.str();
}
