// Evan Heaton
// CS463G: Intro to Artificial Intelligence
// Markov Decision Processes
// 12/1/2016

#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

vector< vector<double> > up_tt;
vector< vector<double> > right_tt;
vector< vector<double> > down_tt;
vector< vector<double> > left_tt;


// prototypes
void create_transition_tables();
void create_up();
void create_right();
void create_down();
void create_left();
double get_probability(int action, int x1, int y1, int x2, int y2);
int get_reward(int x, int y);
void print_transition_table(vector<vector<double> > tt);
void print_value_table(vector<vector<double> > v_t);
void print_value_table2(vector<vector<double> > v_t);
void print_action_table(vector<vector<int> > a_t);
int max_index(vector<double> v);

int main() {
	create_transition_tables();

	vector< vector< vector<double> > > values;
	vector< vector< vector<int> > > actions;

	// compute V0 for each cell
	vector< vector<double> > v0;
	vector< vector<int> > a0;
	for (int i=0; i<6; i++) {
		vector<double> row;
		vector<int> action_row;
		for (int j=0; j<6; j++) {
			row.push_back(get_reward(i, j));
			action_row.push_back(-1);
		}
		v0.push_back(row);
		a0.push_back(action_row);
	}
	values.push_back(v0);
	actions.push_back(a0);

	// compute up to V7 for each cell
	for (int t=1; t<8; t++) {
		vector< vector<double> > v_t;
		vector< vector<int> > a_t;
		// for each cell
		for (int i=0; i<6; i++) {
			vector<double> row;
			vector<int> action_row;
			for (int j=0; j<6; j++) {
				// the hard part

				vector<double> potential_values;
				// for each action
				for (int a=0; a<4; a++) {
					double sum_of_values_for_action = 0;
					// for each potential next cell with given action
					for (int k=0; k<6; k++) {
						for (int l=0; l<6; l++) {
							// if the probability of going from (i,j) to (k,l) is nonzero given action a, add it as a potential value
							double prob = get_probability(a, i, j, k, l);
							if (prob != 0)
								sum_of_values_for_action += prob*values[t-1][k][l];
						}
					}

					potential_values.push_back(sum_of_values_for_action);
				}

				// find the action which maximizes payout
				int best_action = max_index(potential_values);
				double max;
				if (best_action == -1)
					max = 0;
				else
					max = potential_values[best_action];
				action_row.push_back(best_action);
				row.push_back(get_reward(i,j) + max);

			}
			v_t.push_back(row);
			a_t.push_back(action_row);
		}
		values.push_back(v_t);
		actions.push_back(a_t);
	}


	for (int t=0; t<8; t++) {
		cout << "t = " << t << "\n";
		print_value_table(values[t]);
		print_action_table(actions[t]);
	}

	vector< vector< vector<double> > > values2;
	vector< vector< vector<int> > > actions2;


	// compute V0 for each cell
	vector< vector<double> > v02;
	vector< vector<int> > a02;
	for (int i=0; i<6; i++) {
		vector<double> row;
		vector<int> action_row;
		for (int j=0; j<6; j++) {
			row.push_back(get_reward(i, j));
			action_row.push_back(-1);
		}
		v02.push_back(row);
		a02.push_back(action_row);
	}
	values2.push_back(v02);
	actions2.push_back(a02);

	// compute V* for each cell
	for (int t=1; t<1001; t++) {
		vector< vector<double> > v_t;
		vector< vector<int> > a_t;
		// for each cell
		for (int i=0; i<6; i++) {
			vector<double> row;
			vector<int> action_row;
			for (int j=0; j<6; j++) {
				// the hard part

				vector<double> potential_values;
				// for each action
				for (int a=0; a<4; a++) {
					double sum_of_values_for_action = 0;
					// for each potential next cell with given action
					for (int k=0; k<6; k++) {
						for (int l=0; l<6; l++) {
							// if the probability of going from (i,j) to (k,l) is nonzero given action a, add it as a potential value
							double prob = get_probability(a, i, j, k, l);
							if (prob != 0)
								sum_of_values_for_action += prob*values2[t-1][k][l];
						}
					}

					potential_values.push_back(sum_of_values_for_action);
				}

				// find the action which maximizes payout
				int best_action = max_index(potential_values);
				double max;
				if (best_action == -1)
					max = 0;
				else
					max = potential_values[best_action];
				action_row.push_back(best_action);
				row.push_back(get_reward(i,j) + 0.95*max);

			}
			v_t.push_back(row);
			a_t.push_back(action_row);
		}
		values2.push_back(v_t);
		actions2.push_back(a_t);
	}

	cout << "V*\n";
	print_value_table2(values2[1000]);
	print_action_table(actions2[1000]);

}

void create_transition_tables() {

	//populate with vectors with 0s
	for (int i=0; i<36; i++) {
		vector<double> up_temp;
		vector<double> right_temp;
		vector<double> down_temp;
		vector<double> left_temp;
		for (int j=0; j<36; j++) {
			up_temp.push_back(0);
			right_temp.push_back(0);
			down_temp.push_back(0);
			left_temp.push_back(0);
		}
		up_tt.push_back(up_temp);
		right_tt.push_back(right_temp);
		down_tt.push_back(down_temp);
		left_tt.push_back(left_temp);
	}

	create_up();
	create_right();
	create_down();
	create_left();
}

void create_up() {
	// cout << "up\n";

	int full1_indices[] = { 11,11, 24,24, 29,29 };
	int point1_indices[] = { 4,4, 13,13, 14,14, 15,15, 16,16, 19,19, 20,20, 21,21, 22,22, 31,31, 32,32, 33,33, 34,34 };
	int point2_indices[] = { 1,0, 4,3, 5,4, 7,6, 13,12, 14,13, 15,14, 16,15, 17,16, 19,18, 20,19, 21,20, 22,21, 23,22, 31,30, 32,31, 33,32, 34,33, 35,34 };
	int point3_indices[] = { 1,1, 5,5, 7,7, 17,17, 23,23, 35,35 };
	int point7_indices[] = { 0,1, 3,4, 4,5, 6,7, 12,13, 13,14, 14,15, 15,16, 16,17, 18,19, 19,20, 20,21, 21,22, 22,23, 30,31, 31,32, 32,33, 33,34, 34,35 };
	int point8_indices[] = { 0,0, 3,3, 6,6, 12,12, 18,18, 30,30 };

	for (int i=0; i<6; i+=2)
		up_tt[full1_indices[i]][full1_indices[i+1]] = 1;
	for (int i=0; i<26; i+=2)
		up_tt[point1_indices[i]][point1_indices[i+1]] = 0.1;
	for (int i=0; i<38; i+=2)
		up_tt[point2_indices[i]][point2_indices[i+1]] = 0.2;
	for (int i=0; i<12; i+=2)
		up_tt[point3_indices[i]][point3_indices[i+1]] = 0.3;
	for (int i=0; i<38; i+=2)
		up_tt[point7_indices[i]][point7_indices[i+1]] = 0.7;
	for (int i=0; i<12; i+=2)
		up_tt[point8_indices[i]][point8_indices[i+1]] = 0.8;
}

void create_right() {
	// cout << "right\n";

	int full1_indices[] = { 3,3, 4,4, 31,31, 32,32, 33,33, 34,34 };
	int point1_indices[] = { 6,6, 7,7, 11,11, 12,12, 13,13, 17,17, 18,18, 23,23, 24,24, 29,29 };
	int point2_indices[] = { 0,6, 1,7, 5,11, 6,12, 7,13, 11,17, 12,18, 13,19, 14,20, 15,21, 16,22, 17,23, 18,24, 23,29, 24,30, 29,35 };
	int point3_indices[] = { 0,0, 1,1, 5,5, 14,14, 15,15, 16,16 };
	int point7_indices[] = { 6,0, 7,1, 11,5, 12,6, 13,7, 17,11, 18,12, 19,13, 20,14, 21,15, 22,16, 23,17, 24,18, 29,23, 30,24, 35,29 };
	int point8_indices[] = { 19,19, 20,20, 21,21, 22,22, 30,30, 35,35 };

	for (int i=0; i<12; i+=2)
		right_tt[full1_indices[i]][full1_indices[i+1]] = 1;
	for (int i=0; i<20; i+=2)
		right_tt[point1_indices[i]][point1_indices[i+1]] = 0.1;
	for (int i=0; i<32; i+=2)
		right_tt[point2_indices[i]][point2_indices[i+1]] = 0.2;
	for (int i=0; i<12; i+=2)
		right_tt[point3_indices[i]][point3_indices[i+1]] = 0.3;
	for (int i=0; i<32; i+=2)
		right_tt[point7_indices[i]][point7_indices[i+1]] = 0.7;
	for (int i=0; i<12; i+=2)
		right_tt[point8_indices[i]][point8_indices[i+1]] = 0.8;

}

void create_down() {
	// cout << "down\n";

	int full1_indices[] = { 11,11, 24,24, 29,29 };
	int point1_indices[] = { 4,4, 13,13, 14,14, 15,15, 16,16, 19,19, 20,20, 21,21, 22,22, 31,31, 32,32, 33,33, 34,34 };
	int point7_indices[] = { 1,0, 4,3, 5,4, 7,6, 13,12, 14,13, 15,14, 16,15, 17,16, 19,18, 20,19, 21,20, 22,21, 23,22, 31,30, 32,31, 33,32, 34,33, 35,34 };
	int point8_indices[] = { 1,1, 5,5, 7,7, 17,17, 23,23, 35,35 };
	int point2_indices[] = { 0,1, 3,4, 4,5, 6,7, 12,13, 13,14, 14,15, 15,16, 16,17, 18,19, 19,20, 20,21, 21,22, 22,23, 30,31, 31,32, 32,33, 33,34, 34,35 };
	int point3_indices[] = { 0,0, 3,3, 6,6, 12,12, 18,18, 30,30 };

	for (int i=0; i<6; i+=2)
		down_tt[full1_indices[i]][full1_indices[i+1]] = 1;
	for (int i=0; i<26; i+=2)
		down_tt[point1_indices[i]][point1_indices[i+1]] = 0.1;
	for (int i=0; i<38; i+=2)
		down_tt[point2_indices[i]][point2_indices[i+1]] = 0.2;
	for (int i=0; i<12; i+=2)
		down_tt[point3_indices[i]][point3_indices[i+1]] = 0.3;
	for (int i=0; i<38; i+=2)
		down_tt[point7_indices[i]][point7_indices[i+1]] = 0.7;
	for (int i=0; i<12; i+=2)
		down_tt[point8_indices[i]][point8_indices[i+1]] = 0.8;


}

void create_left() {
	// cout << "left\n";

	int full1_indices[] = { 3,3, 4,4, 31,31, 32,32, 33,33, 34,34 };
	int point1_indices[] = { 6,6, 7,7, 11,11, 12,12, 13,13, 17,17, 18,18, 23,23, 24,24, 29,29 };
	int point7_indices[] = { 0,6, 1,7, 5,11, 6,12, 7,13, 11,17, 12,18, 13,19, 14,20, 15,21, 16,22, 17,23, 18,24, 23,29, 24,30, 29,35 };
	int point8_indices[] = { 0,0, 1,1, 5,5, 14,14, 15,15, 16,16 };
	int point2_indices[] = { 6,0, 7,1, 11,5, 12,6, 13,7, 17,11, 18,12, 19,13, 20,14, 21,15, 22,16, 23,17, 24,18, 29,23, 30,24, 35,29 };
	int point3_indices[] = { 19,19, 20,20, 21,21, 22,22, 30,30, 35,35 };

	for (int i=0; i<12; i+=2)
		left_tt[full1_indices[i]][full1_indices[i+1]] = 1;
	for (int i=0; i<20; i+=2)
		left_tt[point1_indices[i]][point1_indices[i+1]] = 0.1;
	for (int i=0; i<32; i+=2)
		left_tt[point2_indices[i]][point2_indices[i+1]] = 0.2;
	for (int i=0; i<12; i+=2)
		left_tt[point3_indices[i]][point3_indices[i+1]] = 0.3;
	for (int i=0; i<32; i+=2)
		left_tt[point7_indices[i]][point7_indices[i+1]] = 0.7;
	for (int i=0; i<12; i+=2)
		left_tt[point8_indices[i]][point8_indices[i+1]] = 0.8;

}

// input: action, (x1, y1) & (x2, y2)
// action: 0=up, 1=right, 2=down, 3=left
// returns a double, representing the probability of transitioning from (x1, y1) to (x2, y2) with action
double get_probability(int action, int x1, int y1, int x2, int y2) {
	if (action == 0)
		return up_tt[6*x2+y2][6*x1+y1];
	else if (action == 1)
		return right_tt[6*x2+y2][6*x1+y1];
	else if (action == 2)
		return down_tt[6*x2+y2][6*x1+y1];
	else
		return left_tt[6*x2+y2][6*x1+y1];
}

// input: (x, y)
// returns the reward of that particular cell
int get_reward(int x, int y) {
	if (x == 3 && y == 1)
		return 10;
	else if (x == 0 && y == 3)
		return 100;
	else if (x == 5 && y == 3)
		return 20;
	else if (x == 2 && y == 4)
		return 5;
	else
		return 0;
}

void print_transition_table(vector<vector<double> > tt) {
	for (int i=0; i<tt.size(); i++) {
		for (int j=0; j<tt[i].size(); j++) {
			printf("%1.1f ", tt[j][i]);
		}
		cout << "\n";
	}
}

void print_value_table(vector<vector<double> > v_t) {
	for (int i=0; i<v_t.size(); i++) {
		for (int j=0; j<v_t[i].size(); j++) {
			printf("%7.3f\t", v_t[j][i]);
		}
		cout << "\n";
	}
}

void print_value_table2(vector<vector<double> > v_t) {
	for (int i=0; i<v_t.size(); i++) {
		for (int j=0; j<v_t[i].size(); j++) {
			printf("%8.3f\t", v_t[j][i]);
		}
		cout << "\n";
	}
}

void print_action_table(vector<vector<int> > a_t) {
	for (int i=0; i<a_t.size(); i++) {
		for (int j=0; j<a_t[i].size(); j++) {
			int a = a_t[j][i];
			if (a == 0)
				cout << "^";
			else if (a == 1)
				cout << ">";
			else if (a == 2)
				cout << "v";
			else if (a == 3)
				cout << "<";
			else
				cout << " ";

			cout << "\t";
		}
		cout << "\n";
	}
	cout << "\n";
}

int max_index(vector<double> v) {
	double greatest = v[0];
	int greatest_index = 0;
	for (int i=1; i<v.size(); i++) {
		if (v[i] > greatest) {
			greatest = v[i];
			greatest_index = i;
		}
	}
	if (greatest != 0)
		return greatest_index;
	else
		return -1;
}
