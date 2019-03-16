#include "solution.h"
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <functional>
#include <unordered_set>

using std::cout;
using std::endl;
using std::unordered_set;
using std::pair;

Solution::Solution() {
	this->n_stud = 0;
	this->n_stud_satisfied = 0;
	this->school_cnt = 0;
	this->stud_sol = 0;
	this->n_seat = 0;
	this->n_school = 0;
	this->sol_matrix = 0;
	this->school_worst = 0;
}

Solution::Solution(const Solution& Sol) {
	n_stud = Sol.n_stud;
	n_stud_satisfied = Sol.n_stud_satisfied;
	n_school = Sol.n_school;
	n_seat = Sol.n_seat;
	school_cnt = new int[n_school];
	memcpy(school_cnt, Sol.school_cnt, sizeof(int) * n_school);
	stud_sol = new int[n_stud];
	memcpy(stud_sol, Sol.stud_sol, sizeof(int) * n_stud);
	sol_matrix = new int*[n_stud];
	for (int i = 0; i < n_stud; i++) {
		sol_matrix[i] = new int[n_school];
		memcpy(sol_matrix[i], Sol.sol_matrix[i], sizeof(int) * n_school);
	}
	school_worst = new int[n_school];
	memcpy(school_worst, Sol.school_worst, sizeof(int) * n_school);
}

Solution::Solution(int n_stud, int n_school, int n_seat) {
	this->n_stud = n_stud;
	this->n_school = n_school;
	this->n_seat = n_seat;
	n_stud_satisfied = 0;
	stud_sol = new int[n_stud];
	std::memset(stud_sol, -1, sizeof(int) * n_stud);
	school_cnt = new int[n_school];
	std::memset(school_cnt, 0, sizeof(int) * n_school);
	sol_matrix = new int*[n_stud];
	for (int i = 0; i < n_stud; i++) {
		sol_matrix[i] = new int[n_school];
		memset(sol_matrix[i], 0, sizeof(int) * n_school);
	}

	this->school_worst = new int[n_school];
	std::memset(school_worst, -1, sizeof(int) * n_school);
}

Solution::Solution(int n, int* stud_sol) {
	this->n_stud = n;
	this->n_school = n;
	this->n_seat = n;
	n_stud_satisfied = n;
	this->stud_sol = new int[n];
	memcpy(this->stud_sol, stud_sol, sizeof(int) * n);
	school_cnt = new int[n];
	for (int i = 0; i < n; i++)
		school_cnt[i] = 1;
	sol_matrix = new int*[n];
	for (int i = 0; i < n; i++) {
		sol_matrix[i] = new int[n];
		memset(sol_matrix[i], 0, sizeof(int) * n);
	}

	this->school_worst = new int[n];
	for (int i = 0; i < n; i++) {
		school_worst[stud_sol[i]] = i;
		sol_matrix[i][stud_sol[i]] = 1;
	}
}


Solution::~Solution() {
	delete[] stud_sol;
	delete[] school_cnt;
	for (int i = 0; i < n_stud; i++)
		delete[] sol_matrix[i];
	delete[] sol_matrix;
	delete[] school_worst;
}

bool Solution::is_good() const {
	return (n_stud == n_stud_satisfied || n_seat == n_stud_satisfied);
}

void Solution::add(int stud, int school, bool is_worst) {
	n_stud_satisfied++;
	stud_sol[stud] = school;
	sol_matrix[stud][school] = 1;
	school_cnt[school]++;
	if (is_worst) school_worst[school] = stud;
}

void Solution::change_school(int good, int bad, int school, int new_worst) {
	stud_sol[good] = school;
	sol_matrix[good][school] = 1;
	stud_sol[bad] = -1;
	sol_matrix[bad][school] = 0;
	school_worst[school] = new_worst;
}

void Solution::change_stud(int stud, int new_school) {
	// school_worst is not maintained anymore.
	if (stud_sol[stud] == -1) throw;
	int old_school = stud_sol[stud];
	sol_matrix[stud][old_school] = 0;
	sol_matrix[stud][new_school] = 1;
	stud_sol[stud] = new_school;
}

void Solution::drop(int a) {
	// TODO
}

bool Solution::operator==(const Solution& rhs) const {
	if (this->n_stud != rhs.n_stud) return false;
	for (int i = 0; i < this->n_stud; i++)
		if (this->stud_sol[i] != rhs.stud_sol[i]) return false;
	return true;
}

std::ostream& operator<< (std::ostream& os, const Solution& Sol) {
	for (int i = 0; i < Sol.n_stud; i++)
		os << "Student " << i + 1 << ": " << Sol.stud_sol[i] + 1 << std::endl;
	return os;
}
