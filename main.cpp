#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <ctime>
#include "model.h"
#include "solver.h"
#include "solution.h"

using namespace std;

void performance_test(int n_stud, int n_school, int each_seats, double consenting_prob, string result_path) {
	std::ofstream fout;
	fout.open(result_path);
	const int N = 10;  // The number of running times for each corr.
	for (double corr = 0.0; corr <= 1.0; corr += 0.05) {
		double avg_running_time = 0.0;
		for (int i = 0; i < N; i++) {
			Model M;
			M.init_randomly(n_stud, n_school, each_seats, corr, consenting_prob);
			clock_t start_time = clock();
			Solver S(M);
			Solution Sol = S.fast_EADAM(false);
			clock_t end_time = clock();
			avg_running_time += end_time - start_time;
		}
		avg_running_time /= N;
		fout << corr << " " << avg_running_time << endl;
		cout << "Performance test for corr = " << corr << " complete. Average running time: " << avg_running_time << " ms." << endl;
	}
}

int main(int argc, char* argv[]) {
	cout << "Please choose what you want to do: " << endl
		<< "1: Performance Test;" << endl
		<< "2: Solve a single problem." << endl;

	int x;
	cin >> x;
	switch (x) {
	case 1: {
		cout << "Please input the followings: " << endl
			<< "1: number of students" << endl
			<< "2: number of schools" << endl
			<< "3: number of seats for each school (assume that all schools have the same number of seats.)" << endl
			<< "4: consenting probability of each student" << endl
			<< "5: file path for output" << endl;
		int n_stud, n_school, each_seats;
		double consenting_prob;
		string output_path;
		cin >> n_stud >> n_school >> each_seats >> consenting_prob >> output_path;
		performance_test(n_stud, n_school, each_seats, consenting_prob, output_path);
		break;
	}

	case 2: {
		string filename_in, filename_out;
		cout << "Please enter the input file path and the output file path: " << endl;
		cin >> filename_in >> filename_out;
		Model M;
		M.init_from_file(filename_in);
		Solver S(M);
		ofstream fout;
		fout.open(filename_out);
		cout << "Please choose which algorithm to run: " << endl;
		cout << "1: Gale-Shapley Algorithm (DA);" << endl;
		cout << "2: EADAM." << endl;
		int y;
		cin >> y;
		switch (y) {
		case 1: {
			Solution Sol(S.Gale_Shapley());
			fout << Sol;
			break;
		}
		case 2: {
			Solution Sol(S.fast_EADAM());
			fout << Sol;
			break;
		}
		default: {
			cout << "Wrong Input! Please choose in {1, 2}" << endl;
			break;
		}
		}
	}

	default: {
		cout << "Wrong Input! Please choose in {1, 2}" << endl;
		break;
	}
	}

	return 0;
}
