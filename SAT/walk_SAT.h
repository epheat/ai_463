// Evan Heaton
// 09/28/2016
// AI Satisfiability Programming Assignment
// walkSAT helper functions header file

#ifndef WALK_SAT_H
#define WALK_SAT_H

using namespace std;

vector<int> walk_sat(vector<vector<int> > formula, int nbvar, bool &satisfiable);
int choose_unsatisfied_clause(vector<int> solution, vector<vector<int> > formula);
int choose_literal_to_flip(vector<int> solution, vector<vector<int> > formula, int clause_index);
int get_number_of_unsatisfied_clauses(vector<int> solution, vector<vector<int> > formula);


#endif
