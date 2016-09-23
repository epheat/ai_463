// Evan Heaton
// 09/22/2016
// AI Satisfiability Programming Assignment
// DPLL helper functions header file

#ifndef DPLL_H
#define DPLL_H

using namespace std;

bool is_consistent(vector<vector<int> > solution);
bool contains_empty_clause(vector<vector<int> > solution);
vector<vector<int> > unit_propogate(vector<vector<int> > solution, int literal);
vector<vector<int> > pure_literal_assign(vector<vector<int> > solution, int literal);

#endif
