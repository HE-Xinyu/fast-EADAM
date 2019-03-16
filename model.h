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
	Model(int n_stud, int n_school, int each_seats,
		double corr_school, double corr_stud, double consenting_prob, bool check = false);
	explicit Model(const std::string filename);
	Model(Model& M);
	~Model();
	friend std::ostream& operator<< (std::ostream& os, const Model& M);
};
