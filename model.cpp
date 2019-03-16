#include "model.h"
#include <string>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <ctime>
#include <cstring>
#include <cmath>

using std::default_random_engine;
using std::uniform_real_distribution;
using std::cout;
using std::endl;

default_random_engine e(time(0));
uniform_real_distribution<double> u(0, 1);

Model::Model() {
}

Model::Model(Model& M) {
	n_stud = M.n_stud;
	n_school = M.n_school;
	n_seat = M.n_seat;
	is_consent = new int[n_stud];
	memcpy(is_consent, M.is_consent, sizeof(int) * n_stud);
	school_pos = new int*[n_school];
	school_pref = new int*[n_school];
	for (int i = 0; i < n_school; i++) {
		school_pos[i] = new int[n_stud];
		memcpy(school_pos[i], M.school_pos[i], sizeof(int) * n_stud);
		school_pref[i] = new int[n_stud];
		memcpy(school_pref[i], M.school_pref[i], sizeof(int) * n_stud);
	}
	stud_pos = new int*[n_stud];
	stud_pref = new int*[n_stud];
	for (int i = 0; i < n_stud; i++) {
		stud_pos[i] = new int[n_school];
		memcpy(stud_pos[i], M.stud_pos[i], sizeof(int) * n_school);
		stud_pref[i] = new int[n_school];
		memcpy(stud_pref[i], M.stud_pref[i], sizeof(int) * n_school);
	}
	seat = new int[n_school];
	memcpy(seat, M.seat, sizeof(int) * n_school);
}

Model::Model(int n_stud, int n_school, int each_seats,
	double corr_school, double corr_stud, double consenting_prob, bool check) {
	
	allocate(n_stud, n_school);
	this->n_stud = n_stud;
	this->n_school = n_school;
	this->n_seat = each_seats * n_school;

	for (int i = 0; i < n_stud; i++) {
		is_consent[i] = u(e) < consenting_prob;
	}

	for (int i = 0; i < n_school; i++)
		seat[i] = each_seats;

	// The school preference is affected by the correlation argument.
	double* public_pref = new double[n_stud];
	for (int i = 0; i < n_stud; i++) public_pref[i] = u(e);

	double* private_pref = new double[n_stud];
	for (int i = 0; i < n_school; i++) {
		for (int j = 0; j < n_stud; j++) {
			private_pref[j] = u(e) * sqrt(1 - corr_school * corr_school) + public_pref[j] * corr_school;
		}
		std::iota(school_pref[i], school_pref[i] + n_stud, 0);
		std::sort(school_pref[i], school_pref[i] + n_stud, [&](int i1, int i2) {return private_pref[i1] < private_pref[i2]; });
		for (int j = 0; j < n_stud; j++)
			school_pos[i][school_pref[i][j]] = j;
	}

	// The student preference is NOT affected by the correlation argument.
	double* public_pref_stud = new double[n_school];
	for (int i = 0; i < n_school; i++) public_pref_stud[i] = u(e);

	double* pref_value_stud = new double[n_school];
	for (int i = 0; i < n_stud; i++) {
		for (int j = 0; j < n_school; j++)
			pref_value_stud[j] = u(e) * sqrt(1 - corr_stud * corr_stud) + public_pref_stud[j] * corr_stud;
		std::iota(stud_pref[i], stud_pref[i] + n_school, 0);
		std::sort(stud_pref[i], stud_pref[i] + n_school, [&](int i1, int i2) {return pref_value_stud[i1] < pref_value_stud[i2]; });
		for (int j = 0; j < n_school; j++)
			stud_pos[i][stud_pref[i][j]] = j;
	}
}

Model::Model(std::string filename) {
	std::ifstream fin;
	fin.open(filename);

	if (fin.fail()) {
		std::cerr << "I/O Error: The input file " << filename << " does not exist.";
		throw;
	}

	fin >> n_stud >> n_school;

	allocate(n_stud, n_school);

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
			// stud_pos means how much a student likes a school (the smaller, the higher priority.)
			stud_pos[i][stud_pref[i][j]] = j;
		}
	}

	for (int i = 0; i < n_school; i++) {
		for (int j = 0; j < n_stud; j++) {
			fin >> school_pref[i][j];
			school_pref[i][j]--;
			// school_pos means how much a school likes a student (the smaller, the higher priority.)
			school_pos[i][school_pref[i][j]] = j;
		}
	}
}

Model::~Model() {
	delete[] is_consent;
	for (int i = 0; i < n_school; i++) {
		delete[] school_pos[i];
		delete[] school_pref[i];
	}
	delete[] school_pos;
	delete[] school_pref;
	for (int i = 0; i < n_stud; i++) {
		delete[] stud_pos[i];
		delete[] stud_pref[i];
	}
	delete[] stud_pos;
	delete[] stud_pref;
	delete[] seat;
}

void Model::allocate(int n_stud, int n_school) {
	stud_pref = new int*[n_stud];
	for (int i = 0; i < n_stud; i++)
		stud_pref[i] = new int[n_school];

	stud_pos = new int*[n_stud];
	for (int i = 0; i < n_stud; i++)
		stud_pos[i] = new int[n_school];

	school_pref = new int*[n_school];
	for (int i = 0; i < n_school; i++)
		school_pref[i] = new int[n_stud];

	school_pos = new int*[n_school];
	for (int i = 0; i < n_school; i++)
		school_pos[i] = new int[n_stud];

	seat = new int[n_school];
	is_consent = new int[n_stud];
}

std::ostream& operator<< (std::ostream& os, const Model& M) {
	os << "Student Preference: " << endl;
	for (int i = 0; i < M.n_stud; i++) {
		for (int j = 0; j < M.n_school; j++)
			os << M.stud_pref[i][j] + 1 << " ";
		os << endl;
	}
	os << endl;
	os << "School Preference: " << endl;
	for (int i = 0; i < M.n_school; i++) {
		for (int j = 0; j < M.n_stud; j++)
			os << M.school_pref[i][j] + 1 << " ";
		os << endl;
	}
	return os;
}
