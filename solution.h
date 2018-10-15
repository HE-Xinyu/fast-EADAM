#include <string>
#pragma once

class Solution {
private:

public:
	int n_stud;
	int* stud_sol;
	int* school_cnt;
	int* school_worst;
	int n_stud_satisfied;
	int n_seat;
	int n_school;
	int** sol_matrix;

	Solution();
	void init(int n_stud, int n_school, int n_seat);
	bool is_good();
	void add(int stud, int school, bool is_worst);
	void drop(int stud);
	void change(int good, int bad, int school, int new_worst);
	void output(std::string filename);
};
