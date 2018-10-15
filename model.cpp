#include "model.h"
#include <string>
#include <iostream>
#include <fstream>

Model::Model() {
	this->n_stud = 0;
	this->n_school = 0;
	this->is_consent = 0;
	this->school_pos = 0;
	this->school_pref = 0;
	this->stud_pref = 0;
	this->n_seat = 0;
}

void Model::init(std::string filename) {
	std::ifstream fin;
	fin.open(filename);

	if (fin.fail()) {
		std::cerr << "I/O Error: The input file " << filename << " does not exist.";
		throw;
	}

	fin >> n_stud >> n_school;

	stud_pref = new int*[n_stud];
	for (int i = 0; i < n_stud; i++)
		stud_pref[i] = new int[n_school];

	school_pref = new int*[n_school];
	for (int i = 0; i < n_stud; i++)
		school_pref[i] = new int[n_stud];

	school_pos = new int*[n_school];
	for (int i = 0; i < n_stud; i++)
		school_pos[i] = new int[n_stud];

	seat = new int[n_school];
	is_consent = new int[n_stud];
	memset(is_consent, 0, sizeof(int) * n_stud);

	for (int i = 0; i < n_school; i++) {
		fin >> seat[i];
		n_seat += seat[i];
	}
		
	for (int i = 0; i < n_stud; i++)
		fin >> is_consent[i];

	for (int i = 0; i < n_stud; i++) {
		for (int j = 0; j < n_school; j++) {
			fin >> stud_pref[i][j];
			stud_pref[i][j]--;
		}
	}

	for (int i = 0; i < n_school; i++) {
		for (int j = 0; j < n_stud; j++) {
			fin >> school_pref[i][j];			
			school_pref[i][j]--;
			school_pos[i][school_pref[i][j]] = j;
		}
	}
}
