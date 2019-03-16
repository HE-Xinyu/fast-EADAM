#include <iostream>
#include <deque>
#include <unordered_set>
#include <cassert>
#include <queue>
#include "solver.h"
#include "solution.h"
#include "model.h"
#include "hash.h"
#include "set_ops.h"

Solver::Solver() {
}

Solver::~Solver() {
}

Solution Solver::Gale_Shapley(const Model& M, bool info) {
	/***************************************
	Student-optimal Gale Shapley Algorithm

	Usage: first call Solver::init(M), then simply call this function.

	Return: a solution object which contains all the information.
	****************************************/

	Solution S(M.n_stud, M.n_school, M.n_seat);
	// S.init();
	int* head_stud = new int[M.n_stud];
	memset(head_stud, 0, sizeof(int) * M.n_stud);
	while (!S.is_good()) {
		for (int i = 0; i < M.n_stud; i++) {
			if (S.stud_sol[i] == -1) {  // The student is not allocated to any school.
				while (head_stud[i] != M.n_school) {
					bool exit = false;  // If he finds a school, exit the loop.
					int cur_school = M.stud_pref[i][head_stud[i]];
					// Decide if he is the worst student of the school.
					bool flag = S.school_worst[cur_school] == -1 || 
						M.school_pos[cur_school][i] > M.school_pos[cur_school][S.school_worst[cur_school]];

					if (S.school_cnt[cur_school] < M.seat[cur_school]) {
						// The school still has remaining seats.
						S.add(i, cur_school, flag);
						exit = true;
					}
					else if (!flag) {
						int dropped_stud = S.school_worst[cur_school];
						int j;
						for (j = M.school_pos[cur_school][dropped_stud] - 1; j >= 0; j--)
							// Find the worst student excluding the dropped student.
							if (S.sol_matrix[M.school_pref[cur_school][j]][cur_school] || 
								(M.school_pref[cur_school][j] == i))
								break;
						S.change_school(i, dropped_stud, cur_school, M.school_pref[cur_school][j]);
						exit = true;
					}
					head_stud[i]++;
					if (exit) break;
				}
			}
		}
	}
	if (info)
		std::cout << "Gale-Shapley Algorithm Complete!" << std::endl;
	delete[] head_stud;
	return S;
}

Solution Solver::fast_EADAM(const Model& M, bool info) {
	/************************************
	Student-optimal fast EADAM algorithm.

	Usage: first initialize the model by calling Solver::init(M),
	then call this function.

	Return: a solution object which contains all the information.
	************************************/

	Solution S = Gale_Shapley(M, false);

	std::deque <int> Q;

	int* checked = new int[M.n_school];  // The school is checked iff head[school] == n_stud.
	int* visited_school = new int[M.n_school];  // 1 if the school is in queue.
	int* visited_stud = new int[M.n_stud];  // 1 if the student is in queue.
	int* head = new int[M.n_school];  // The head position for the main loop.

	// Initializations.
	memset(checked, 0, sizeof(int) * M.n_school);
	memset(head, 0, sizeof(int) * M.n_school);
	memset(visited_school, 0, sizeof(int) * M.n_school);
	memset(visited_stud, 0, sizeof(int) * M.n_stud);

	int n_school_checked = 0;
	int cur_school = 0;

	for (int i = 0; i < M.n_school; i++) {
		if (S.school_worst[i] == -1) {
			// There're no students choosing the school in GS.
			// Then the school should be useless.
			checked[i] = 1;
			head[i] = M.n_stud;
			n_school_checked++;
		}
		else 
			head[i] = M.school_pos[i][S.school_worst[i]] + 1;
	}
	
	// Here is the main algorithm.
	while (n_school_checked != M.n_school) {
		// Hint: the queue in the loop MUST have odd number of elements;
		// the queue should look like: (school, student, school, student, ..., school).
		if (Q.empty()) {
			// Choose the first unchecked school.
			// Notice that the permutation of schools will not affect the result.
			for (int i = 0; i < M.n_school; i++) {
				if (!checked[i]) {
					Q.push_back(i);
					cur_school = i;
					visited_school[i] = 1;
					break;
				}
			}
		}

		if (checked[cur_school]) {
			// The current school is useless. Pop it.
			Q.pop_back();
			visited_school[cur_school] = 0;
			if (Q.size() >= 2) {
				int tmp_stud = Q.back();
				Q.pop_back();
				visited_stud[tmp_stud] = 0;
				int tmp_school = Q.back();
				if (!M.is_consent[tmp_stud])
					// The student is not consenting, thus the school is blocked by the student.
					head[tmp_school] = M.n_stud;
				else
					head[tmp_school]++;
				cur_school = tmp_school;
			}
			else {
				// Find the next unchecked school.
				while (checked[cur_school])
					cur_school = (cur_school + 1) % M.n_school;

				Q.push_back(cur_school);
				visited_school[cur_school] = 1;
			}
		}

		bool flag = false;

		while (head[cur_school] < M.n_stud) {
			int cur_stud = M.school_pref[cur_school][head[cur_school]];
			int sol_school = S.stud_sol[cur_stud];
			if (sol_school == -1) {
				head[cur_school]++;
				continue;
			}
			if (M.stud_pos[cur_stud][cur_school] < M.stud_pos[cur_stud][sol_school]) {
				Q.push_back(cur_stud);
				if (visited_stud[cur_stud]) {
					while (true) {
						// Keep popping the queue twice at a time, until
						// the current student is popped.
						int tmp_stud = Q.back();
						visited_stud[tmp_stud] = 0;
						Q.pop_back();
						int tmp_school = Q.back();
						S.change_stud(tmp_stud, tmp_school);
						head[tmp_school]++;
						visited_school[tmp_school] = 0;
						Q.pop_back();
						if (Q.back() == cur_stud) {
							Q.pop_back();
							cur_school = Q.back();
							break;
						}
					}
				}
				else if (visited_school[sol_school]) {
					while (true) {
						// Keep popping the queue twice at a time, until
						// the current school is popped.
						// Notice: the sequence of lines in the loop is important.
						int tmp_stud = Q.back();
						visited_stud[tmp_stud] = 0;
						Q.pop_back();
						int tmp_school = Q.back();
						S.change_stud(tmp_stud, tmp_school);
						head[tmp_school]++;
						if (tmp_school == sol_school) {
							cur_school = tmp_school;
							break;
						}
						visited_school[tmp_school] = 0;
						Q.pop_back();
					}
				}
				else {
					// Push the student and school into the queue.
					// Set current school with sol_school.
					visited_stud[cur_stud] = 1;
					Q.push_back(sol_school);
					visited_school[sol_school] = 1;
					cur_school = sol_school;
					flag = true;
					break;
				}
			}
			else {
				head[cur_school]++;
			}
		}

		if (!flag) {
			// We finish checking the school :)
			checked[cur_school] = 1;
			n_school_checked++;
		}
	}

	// Maintain the school_worst array.
	//memset(S.school_worst, -1, sizeof(int) * S.n_school);
	//for (int i = 0; i < S.n_stud; i++) {
	//	int cur_school = S.stud_sol[i];
	//	if (S.school_worst[cur_school] == -1 || M.school_pos[cur_school][i] > M.school_pos[cur_school][S.school_worst[cur_school]])
	//		S.school_worst[cur_school] = i;
	//}
		
	if (info)
		std::cout << "EADAM Algorithm Complete!" << std::endl;
	
	delete[] checked;
	delete[] visited_school;
	delete[] visited_stud;
	delete[] head;
	return S;
}

std::unordered_set<Solution, SolutionHash> Solver::one_step_improvement(const Solution& Sol, const Model& M) {
	int n = M.n_stud;
	assert(M.n_school == M.n_stud && Sol.n_school == Sol.n_stud && Sol.n_school == M.n_school);
	std::unordered_set<Solution, SolutionHash> rtn;
	std::deque<int> Q;

	int* visited_stud = new int[n];
	// A student is visited if either
	// he is a sink;
	// he leads to a circle.
	int* head = new int[n];  // The head position for the main loop.
	int* school_sol = new int[n];
	int* circle = new int[n];

	memset(visited_stud, 0, sizeof(int) * n);
	memset(circle, 0, sizeof(int) * n);

	for (int i = 0; i < n; i++) {
		int school = Sol.stud_sol[i];
		head[i] = M.stud_pos[i][school] + 1;
		school_sol[school] = i;
	}

	int cnt = 0;
	int cur_stud = 0;
	while (cnt != n) {
		// The queue only consists of students, because the corresponding school can be easily find via stud_sol.

		if (Q.empty()) {
			// Find the first unchecked student, and then break.
			for (int i = 0; i < n; i++) {
				if (!visited_stud[i]) {
					Q.push_back(i);
					cur_stud = i;
					visited_stud[i] = 1;
					// cnt++;
					break;
				}
			}
		}

		else cur_stud = Q.back();

		while (head[cur_stud] < n) {
			
			int cur_school = M.stud_pref[cur_stud][head[cur_stud]];
			int worst_stud = school_sol[cur_school];

			// cout << cur_stud + 1 << " " << cur_school + 1 << endl;
			//if (visited_stud[worst_stud]) {
			//	head[cur_stud]++;
			//	continue;
			//}

			if (M.school_pos[cur_school][cur_stud] < M.school_pos[cur_school][worst_stud]) {
				if (circle[worst_stud]) {
					head[cur_stud]++;
				}
				else if (visited_stud[worst_stud]) {
					int* new_stud_sol = new int[n];
					memcpy(new_stud_sol, Sol.stud_sol, sizeof(int) * M.n_stud);
					int prev_stud = worst_stud;
					int tmp_stud, tmp_school;
					do {
						tmp_stud = Q.back();
						tmp_school = Sol.stud_sol[prev_stud];
						new_stud_sol[tmp_stud] = tmp_school;
						prev_stud = tmp_stud;
						Q.pop_back();
						circle[tmp_stud] = 1;
						cnt++;
					} while (tmp_stud != worst_stud);
					Solution new_sol(n, new_stud_sol);
					rtn.emplace(new_sol);
					if (!Q.empty()) {
						cur_stud = Q.back();
					}
					break;
				}
				else {
					visited_stud[worst_stud] = 1;
					Q.push_back(worst_stud);
					cur_stud = worst_stud;
				}
			}
			else {
				head[cur_stud]++;
			}
		}

		if (head[cur_stud] == n) {
			visited_stud[cur_stud] = 1;
			circle[cur_stud] = 1;
			Q.pop_back();
			cnt++;
			if (!Q.empty()) {
				cur_stud = Q.back();
				head[cur_stud]++;
			}
		}

	}
	delete[] head;
	delete[] school_sol;
	delete[] circle;
	delete[] visited_stud;
	return rtn;
}

std::unordered_set<Solution, SolutionHash> Solver::vnm_stable_set(const Model& M) {
	// Currently it only deals with situations when n_stud == n_school.
	assert(M.n_school == M.n_stud);
	// Start from student-optimal efficiency-adjusted solution.
	Solution Sol = fast_EADAM(M, false);

	std::unordered_set<Solution, SolutionHash> rtn;
	std::unordered_set<Solution, SolutionHash> prev;

	prev.emplace(Sol);
	rtn.emplace(Sol);

	while (!prev.empty()) {
		std::unordered_set<Solution, SolutionHash> cur;
		for (const Solution& tmp_sol : prev) {
			std::unordered_set<Solution, SolutionHash> tmp_set = one_step_improvement(tmp_sol, M);
			for (const Solution& tmp_sol_new : tmp_set) {
				cur.emplace(tmp_sol_new);
				rtn.emplace(tmp_sol_new);
			}			
		}
		prev = cur;
	}

	return rtn;

}


unordered_set<pair<int, int>, BPHash> Solver::blocking_pairs(const Solution& Sol, const Model& M) {
	assert(Sol.n_school == Sol.n_stud && M.n_school == M.n_stud && Sol.n_school == M.n_school);
	unordered_set<pair<int, int>, BPHash> rtn;
	
	for (int i = 0; i < M.n_stud; i++) {
		int cur_school = Sol.stud_sol[i];
		for (int j = 0; j < M.stud_pos[i][cur_school]; j++) {
			int desirable_school = M.stud_pref[i][j];
			int cur_stud = Sol.school_worst[desirable_school];
			if (M.school_pos[desirable_school][i] < M.school_pos[desirable_school][cur_stud])
				rtn.insert(std::make_pair(i, desirable_school));
		}
	}

	return rtn;
}

const int fact[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800 };

void decode(int code, int n, int* arr) {
	int* used = new int[n];
	memset(used, 0, sizeof(int) * n);
	for (int i = 0; i < n; i++) {
		int x = code / fact[n - i - 1];
		code %= fact[n - i - 1];
		for (int j = 0; ; j++) {
			if (!used[j]) {
				if (x == 0) {
					arr[i] = j;
					used[j] = 1;
					break;
				}
				x--;
			}
		}

	}
	delete[] used;
}

unordered_set<Solution, SolutionHash> Solver::bp_generate_set(const Model& M) {
	unordered_set<Solution, SolutionHash> rtn;
	Solution eadam_sol = fast_EADAM(M, false);
	// rtn.insert(eadam_sol);
	unordered_set<pair<int, int>, BPHash> bp_set = blocking_pairs(eadam_sol, M);
	int n = M.n_school;

	//cout << "blocking pairs for EADAM: " << endl;
	//for (auto p : bp_set)
	//	cout << "(" << p.first << ", " << p.second << ")\n";
	
	int* cur_stud_sol = new int[n];
	// Start with solution (1, 2, ..., n)
	//for (int i = 0; i < n; i++)
	//	cur_stud_sol[i] = i;

	for (int i = 0; i < fact[n]; i++) {
		decode(i, n, cur_stud_sol);
		Solution cur(n, cur_stud_sol);
		// cout << cur << endl;
		if (subset(bp_set, blocking_pairs(cur, M)))
			rtn.insert(cur);

		//if (cur_stud_sol[0] == 0 && cur_stud_sol[1] == 1 && cur_stud_sol[2] == 3 && cur_stud_sol[3] == 2)
		//	for (auto p : blocking_pairs(cur, M))
		//		cout << "(" << p.first << ", " << p.second << ")\n";
	}
	delete[] cur_stud_sol;
	return rtn;
}


double Solver::welfare(const Model& M, const Solution& Sol) {
	assert(M.n_school == Sol.n_school && M.n_stud == Sol.n_stud);
	double Ans = 0.0;
	for (int i = 0; i < Sol.n_stud; i++) {
		if (Sol.stud_sol[i] == -1) Ans += 1.0 + M.n_school;
		else Ans += 1.0 + M.stud_pos[i][Sol.stud_sol[i]];
	}
	return Ans / Sol.n_stud;
}
