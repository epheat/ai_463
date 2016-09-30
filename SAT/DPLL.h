// Evan Heaton
// 09/22/2016
// AI Satisfiability Programming Assignment
// DPLL helper functions header file

#ifndef DPLL_H
#define DPLL_H

using namespace std;

bool DPLL(vector<vector<int> > solution, int nbvar);
bool is_consistent(vector<vector<int> > solution, int nbvar);
bool contains_empty_clause(vector<vector<int> > solution);
vector<vector<int> > unit_propogate(vector<vector<int> > solution, int literal);
vector<vector<int> > pure_literal_assign(vector<vector<int> > solution, int literal);
vector<int> get_pure_literals(vector<vector<int> > solution, int nbvar);
int sign(int input);
int choose_literal(vector<vector<int> > solution);


#endif
