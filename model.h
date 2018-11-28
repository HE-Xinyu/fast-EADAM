#include<string>
#include<random>
#pragma once

class Model {
private:
	void allocate(int n_stud, int n_school);

public:
	int n_stud;
	int n_school;
	int n_seat;

	int ** stud_pref;
	int ** stud_pos;
	int ** school_pref;
	int ** school_pos;
	int * is_consent;
	int * seat;
	Model();
	Model(Model& M);
	~Model();
	void init_from_file(const std::string filename);
	void init_randomly(int n_stud, int n_school, int each_seats,
		double corr, double consenting_prob, bool check = false);
};
