//#include "model.h"
//#include "hash.h"
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_set>
#pragma once


using std::unordered_set;
using std::pair;

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
	Solution(int n_stud, int n_school, int n_seat);
	Solution(int n, int* stud_sol);
	Solution(const Solution& Sol);
	~Solution();

	bool is_good() const;
	void add(int stud, int school, bool is_worst);
	void drop(int stud);
	void change_school(int good, int bad, int school, int new_worst);
	void change_stud(int stud, int new_school);
	bool operator==(const Solution& rhs) const;
	friend class SolutionHash;
	friend std::ostream& operator<< (std::ostream& os, const Solution& Sol);
};


