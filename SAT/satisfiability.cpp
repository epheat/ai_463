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
#include <ctime> // for clock()
#include "DPLL.h"
#include "hill_climbing.h"
#include "walk_SAT.h"
#include "other_helpers.h"

using namespace std;

//prototype some functions
vector<vector<int> > get_test_case_vector(bool easy, int test_case, int &nbvar, int &nbclauses);
string vec_to_string(vector<int> v);
string vec_to_string(vector<vector<int> > v);


// 1st command line argument is 1 or 0, 1 for easy, 0 for hard.
// 2nd command line argument is 1-100, representing which formula to attempt to satisfy

int main(int argc, char* argv[]) {
	srand ((int)time(NULL)); // seed the random number generator

	if (argc != 3) {
		cout << "usage: " << argv[0] << " [easy/~hard] [#1-100]\n";
		return 1;
	}

	stringstream ss;
	bool easy;
	int formula_number;
	ss << argv[1] << " " << argv[2];
	if (!(ss >> easy)) {
		cout << "usa1ge: " << argv[0] << " [easy/~hard] [#1-100]\n";
		return 1;
	}
	if (!(ss >> formula_number)) {
		cout << "usa2ge: " << argv[0] << " [easy/~hard] [#1-100]\n";
		return 1;
	}
	if (formula_number < 1 || formula_number > 100) {
		cout << "usa3ge: " << argv[0] << " [easy/~hard] [#1-100]\n";
		return 1;
	}

	int nbvar = 0, nbclauses = 0;
	vector<vector<int> > formula = get_test_case_vector(easy, formula_number, nbvar, nbclauses); // parse the cnf file

	clock_t start;
	double duration;

	/*
	bool satisfiable = false;
	start = clock();
	satisfiable = DPLL(formula, nbvar);
	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	if (satisfiable)
		cout << "SAT by DPLL in     \t" << duration << "s\n";
	else
		cout << "UNSAT by DPLL in   \t" << duration << "s\n";
	*/


	bool satisfiable = false;
	start = clock();
	vector<int> solution = walk_sat(formula, nbvar, satisfiable);
	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	if (satisfiable)
		cout << "SAT by walkSAT in  \t" << duration << "s\n";
	else
		cout << "UNSAT by walkSAT in\t" << duration << "s\n";

	satisfiable = false;
	start = clock();
	solution = repeated_hill_climbing(formula, nbvar, satisfiable);
	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	if (satisfiable)
		cout << "SAT by LS in       \t" << duration << "s\n";
	else
		cout << "UNSAT by LS in     \t" << duration << "s\n";

	// I'll fix DPLL later I guess...
	// cout << DPLL(formula, nbvar);

	return 0;
}

vector<vector<int> > make_test_formula() {
	vector<vector<int> > test_formula; // this should be consistent
	vector<int> clause1;
	clause1.push_back(1);
	clause1.push_back(2);
	clause1.push_back(-3);

	vector<int> clause2;
	clause2.push_back(-4);
	clause2.push_back(-5);
	clause2.push_back(-6);


	vector<int> clause3;
	clause3.push_back(1);
	clause3.push_back(-4);
	clause3.push_back(-3);

	test_formula.push_back(clause1);
	test_formula.push_back(clause2);
	test_formula.push_back(clause3);

	return test_formula;
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
