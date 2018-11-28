#include <string>
#include <iostream>
#pragma once

class Solution {
private:

public:
	int n_stud;
	int n_stud_satisfied;
	int n_seat;
	int n_school;

	int* stud_sol;
	int* school_cnt;
	int* school_worst;
	int** sol_matrix;

	Solution();
	Solution(const Solution& Sol);
	~Solution();
	void init(int n_stud, int n_school, int n_seat);
	bool is_good() const;
	void add(int stud, int school, bool is_worst);
	void drop(int stud);
	void change_school(int good, int bad, int school, int new_worst);
	void change_stud(int stud, int new_school);
	// void output(std::string filename);
	friend std::ostream& operator<< (std::ostream& os, const Solution Sol);
};
