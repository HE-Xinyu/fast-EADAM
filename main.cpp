#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <ctime>
#include <unordered_set>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include "model.h"
#include "solver.h"
#include "solution.h"
#include "hash.h"
#include "set_ops.h"

using namespace std;

#define max(a,b) (((a) > (b)) ? (a) : (b))

void performance_test(int n_stud, int n_school, int each_seats, double consenting_prob, string result_path) {
	std::ofstream fout;
	fout.open(result_path);
	const int N = 10;  // The number of running times for each corr.
	for (double corr = 0.0; corr <= 1.01; corr += 0.05) {
		double avg_running_time = 0.0;
		double w_gs = 0.0, w_ea = 0.0;
		double worst = 0.0;
		for (int i = 0; i < N; i++) {
			// Randomly generate the model N times.
			Model M(n_stud, n_school, each_seats, corr, 0.0, consenting_prob);
			// M.init_randomly(n_stud, n_school, each_seats, corr, consenting_prob);
			int Max = 0;
			for (int j = 0; j < N; j++) {
				// Run the EADAM algorithm N times for each model.
				clock_t start_time = clock();
				Solver S;
				Solution Sol_GS = S.Gale_Shapley(M, false);
				Solution Sol = S.fast_EADAM(M, false);
				clock_t end_time = clock();
				
				for (int k = 0; k < Sol.n_stud; k++) {
					int s = Sol.stud_sol[k];
					Max = max(Max, M.stud_pos[k][s]);
				}
				break;
				if (j == 0) {
					w_gs += S.welfare(M, Sol_GS);
					w_ea += S.welfare(M, Sol);
				}
				avg_running_time += end_time - start_time;
			}
			worst += Max;
		}
		// cout << "Welfare for GS: " << w_gs / N << " for ea: " << w_ea / N << endl;
		avg_running_time /= (N * N); 
		fout << corr << " " << worst / N << endl;
		// fout << corr << " " << w_gs / N << " " << w_ea / N << endl;
		cout << "Performance test for corr = " << corr << " complete. Average running time: " << avg_running_time << " ms." << endl;

		// break;
	}
}

//bool operator==(const Solution& lhs, const Solution& rhs) {
//	if (lhs.n_stud != rhs.n_stud) return false;
//	for (int i = 0; i < lhs.n_stud; i++)
//		if (lhs.stud_sol[i] != rhs.stud_sol[i]) return false;
//	return true;
//};

//int main(int argc, char* argv[]) {
//	cout << "Please choose what you want to do: " << endl
//		<< "1: Performance Test;" << endl
//		<< "2: Solve a single problem." << endl;
//
//	int x;
//	cin >> x;
//	switch (x) {
//	case 1: {
//		cout << "Please input the followings: " << endl
//			<< "1: number of students" << endl
//			<< "2: number of schools" << endl
//			<< "3: number of seats for each school (assume that all schools have the same number of seats.)" << endl
//			<< "4: consenting probability of each student" << endl;
//		int n_stud, n_school, each_seats;
//		double consenting_prob;
//		
//		cin >> n_stud >> n_school >> each_seats >> consenting_prob;
//		string output_path = to_string(n_stud) + "_"
//			+ to_string(n_school) + "_"
//			+ to_string(each_seats) + "_"
//			+ to_string(consenting_prob) + "_"
//			+ ".out";
//		performance_test(n_stud, n_school, each_seats, consenting_prob, output_path);
//		break;
//	}
//
//	case 2: {
//		string filename_in, filename_out;
//		cout << "Please enter the input file path and the output file path: " << endl;
//		cin >> filename_in >> filename_out;
//		Model M(filename_in);
//		/*M.init_from_file(filename_in);*/
//		Solver S;
//		ofstream fout;
//		fout.open(filename_out);
//		cout << "Please choose which algorithm to run: " << endl;
//		cout << "1: Gale-Shapley Algorithm (DA);" << endl;
//		cout << "2: EADAM." << endl;
//		int y;
//		cin >> y;
//		switch (y) {
//		case 1: {
//			Solution Sol(S.Gale_Shapley(M));
//			fout << Sol;
//			break;
//		}
//			
//		case 2: {
//			unordered_set<Solution, SolutionHash> Set;
//			Solution Sol(S.fast_EADAM(M));
//			Set.insert(Sol);
//			Solution Sol2(S.fast_EADAM(M));
//			// fout << Sol;
//			// cout << (Sol == Sol2) << endl;
//			cout << Sol << endl;
//			cout << (Set.find(Sol2) == Set.end()) << endl;
//			cout << (Sol == Sol2) << endl;
//			break;
//		}
//			
//		default: {
//			cout << "Wrong Input! Please choose in {1, 2}" << endl;
//			break;
//		}		 
//		}
//		break;
//	}
//
//	default: {
//		cout << "Wrong Input! Please choose in {1, 2}" << endl;
//		break;
//	}
//	}
//
//	return 0;
//}

int main() {
	std::ofstream fout;
	fout.open("seat_welfare.out");
	for (int i = 50; i <= 150; i += 10) {
		double Ans = 0.0;
		for (int _ = 0; _ < 10; _++) {
			Model M(10000, 100, i, 1.0, 0.5, 1.0, false);
			Solver S;
			Solution Sol = S.fast_EADAM(M);
			Ans += S.welfare(M, Sol);
		}
		fout << i << " " << Ans / 10 << endl;
	}

	string filename = "test/vnm.in";
	// Model M(filename);
	Solver S;

	// SolutionHash debug;

	//Solution Sol = S.fast_EADAM(M, false);
	//unordered_set<Solution, SolutionHash> Set = S.vnm_stable_set(M);
	//unordered_set<Solution, SolutionHash> Set_1 = S.bp_generate_set(M);
	//cout << equals(Set, Set_1) << endl;
	//cout << "VNM_Stable_Set: " << endl;
	//for (const Solution& tmp : Set) {
	//	cout << tmp << endl;
	//}
	// unordered_set<pair<int, int>, BPHash> Set = S.blocking_pairs(Sol, M);

	//for (auto p : Set) {
	//	cout << "(" << p.first + 1 << ", " << p.second + 1 << ")" << endl;
	//}


	//cout << equals(Set, Set_1) << endl;
	//for (const Solution& tmp : Set) {
	//	cout << tmp << endl;
	//}
	//int cnt = 0;
	//for (int i = 0; i < 10000; i++) {
	//	Model M(8, 8, 1, 0.0, 1.0, false);
	//	//try
	//	//{
	//	//	unordered_set<Solution, SolutionHash> Set = S.vnm_stable_set(M);
	//	//}
	//	//catch (const std::exception&)
	//	//{
	//	//	cout << M << endl;
	//	//}
	//	// cout << M << endl;
	//	unordered_set<Solution, SolutionHash> Set_1 = S.bp_generate_set(M);
	//	unordered_set<Solution, SolutionHash> Set = S.vnm_stable_set(M);
	//	if (!subset(Set, Set_1)) {
	//		cout << "Model: " << endl;
	//		cout << M << endl;
	//		cout << "EADAM Solution: " << endl;
	//		cout << S.fast_EADAM(M, false) << endl;
	//		cout << "BP_generate_set: " << endl;
	//		for (const Solution& tmp : Set_1) {
	//			cout << tmp << endl;
	//		}
	//		cout << "VNM_Stable_Set: " << endl;
	//		for (const Solution& tmp : Set) {
	//			cout << tmp << endl;
	//		}
	//		break;
	//	}
	//	cnt += subset(Set, Set_1);
	//	// this_thread::sleep_for(chrono::milliseconds(5));
	//}
	//cout << cnt << endl;
	return 0;
}
