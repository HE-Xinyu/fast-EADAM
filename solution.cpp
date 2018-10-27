#include "solution.h"
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

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

void Solution::init(int n_stud, int n_school, int n_seat) {
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

bool Solution::is_good() {
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

void Solution::output(std::string filename) {
	std::ofstream fout;
	fout.open(filename);

	for (int i = 0; i < n_stud; i++) {
		fout << "Student " << i + 1 << ": " << stud_sol[i] + 1 << std::endl;
	}


}
