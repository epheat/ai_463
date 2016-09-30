// Evan Heaton
// 09/29/2016
// AI Satisfiability Programming Assignment
// other helper functions header file

#ifndef OTHER_HELPERS_H
#define OTHER_HELPERS_H

using namespace std;

string vec_to_string(vector<int> v);
string vec_to_string(vector<vector<int> > v);
int get_fitness(vector<int> solution, vector<vector<int> > formula);
vector<int> mutate_solution(vector<int> solution);
vector<int> generate_solution(int nbvar);

#endif
