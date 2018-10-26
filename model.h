#include<string>
#pragma once

class Model {
private:

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
	void init(std::string filename);
};
