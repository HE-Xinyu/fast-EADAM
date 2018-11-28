#include <iostream>
#include <deque>
#include <unordered_set>
#include "solver.h"
#include "solution.h"
#include "model.h"


using std::cout;
using std::endl;
Solver::Solver(Model& M) {
	M_ptr = &M;
}

Solver::~Solver() {
}

Solution Solver::Gale_Shapley(bool info) {
	/***************************************
	Student-optimal Gale Shapley Algorithm

	Usage: first call Solver::init(M), then simply call this function.

	Return: a solution object which contains all the information.
	****************************************/

	Solution S;
	S.init(M_ptr->n_stud, M_ptr->n_school, M_ptr->n_seat);
	int* head_stud = new int[M_ptr->n_stud];
	memset(head_stud, 0, sizeof(int) * M_ptr->n_stud);
	while (!S.is_good()) {
		for (int i = 0; i < M_ptr->n_stud; i++) {
			if (S.stud_sol[i] == -1) {  // The student is not allocated to any school.
				while (head_stud[i] != M_ptr->n_school) {
					bool exit = false;  // If he finds a school, exit the loop.
					int cur_school = M_ptr->stud_pref[i][head_stud[i]];
					// Decide if he is the worst student of the school.
					bool flag = S.school_worst[cur_school] == -1 || 
						M_ptr->school_pos[cur_school][i] > M_ptr->school_pos[cur_school][S.school_worst[cur_school]];

					if (S.school_cnt[cur_school] < M_ptr->seat[cur_school]) {
						// The school still has remaining seats.
						S.add(i, cur_school, flag);
						exit = true;
					}
					else if (!flag) {
						int dropped_stud = S.school_worst[cur_school];
						int j;
						for (j = M_ptr->school_pos[cur_school][dropped_stud] - 1; j >= 0; j--)
							// Find the worst student excluding the dropped student.
							if (S.sol_matrix[M_ptr->school_pref[cur_school][j]][cur_school] || 
								(M_ptr->school_pref[cur_school][j] == i))
								break;
						S.change_school(i, dropped_stud, cur_school, M_ptr->school_pref[cur_school][j]);
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
	return S;
}

Solution Solver::fast_EADAM(bool info) {
	/************************************
	Student-optimal fast EADAM algorithm.

	Usage: first initialize the model by calling Solver::init(M),
	then call this function.

	Return: a solution object which contains all the information.
	************************************/

	Solution S = Gale_Shapley(false);

	std::deque <int> Q;

	int* checked = new int[M_ptr->n_school];  // The school is checked iff head[school] == n_stud.
	int* visited_school = new int[M_ptr->n_school];  // 1 if the school is in queue.
	int* visited_stud = new int[M_ptr->n_stud];  // 1 if the student is in queue.
	int* head = new int[M_ptr->n_school];  // The head position for the main loop.

	// Initializations.
	memset(checked, 0, sizeof(int) * M_ptr->n_school);
	memset(head, 0, sizeof(int) * M_ptr->n_school);
	memset(visited_school, 0, sizeof(int) * M_ptr->n_school);
	memset(visited_stud, 0, sizeof(int) * M_ptr->n_stud);

	int n_school_checked = 0;
	int cur_school = 0;

	for (int i = 0; i < M_ptr->n_school; i++) {
		if (S.school_worst[i] == -1) {
			// There're no students choosing the school in GS.
			// Then the school should be useless.
			checked[i] = 1;
			head[i] = M_ptr->n_stud;
			n_school_checked++;
		}
		else 
			head[i] = M_ptr->school_pos[i][S.school_worst[i]] + 1;
	}
	
	// Here is the main algorithm.
	// Warning: After fast EADAM, the school_worst may not be correct. 
	while (n_school_checked != M_ptr->n_school) {
		// Hint: the queue in the loop MUST have odd number of elements;
		// the queue should look like: (school, student, school, student, ..., school).
		if (Q.empty()) {
			// Choose the first unchecked school.
			// Notice that the permutation of schools will not affect the result.
			for (int i = 0; i < M_ptr->n_school && (!checked[i]); i++) {
				Q.push_back(i);
				cur_school = i;
				visited_school[i] = 1;
				break;
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
				if (!M_ptr->is_consent[tmp_stud])
					// The student is not consenting, thus the school is blocked by the student.
					head[tmp_school] = M_ptr->n_stud;
				else
					head[tmp_school]++;
				cur_school = tmp_school;
			}
			else {
				// Find the next unchecked school.
				while (checked[cur_school])
					cur_school = (cur_school + 1) % M_ptr->n_school;

				Q.push_back(cur_school);
				visited_school[cur_school] = 1;
			}
		}

		bool flag = false;

		while (head[cur_school] < M_ptr->n_stud) {
			int cur_stud = M_ptr->school_pref[cur_school][head[cur_school]];
			int sol_school = S.stud_sol[cur_stud];
			if (sol_school == -1) {
				head[cur_school]++;
				continue;
			}
			if (M_ptr->stud_pos[cur_stud][cur_school] < M_ptr->stud_pos[cur_stud][sol_school]) {
				if (visited_stud[cur_stud]) {
					Q.push_back(cur_stud);
					while (true) {
						// Keep poping the queue twice at a time, until
						// the current student is poped.
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
					Q.push_back(cur_stud);
					while (true) {
						// Keep poping the queue twice at a time, until
						// the current school is poped.
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
					// Set current student with sol_school.
					Q.push_back(cur_stud);
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
			// We finally check the school :)
			checked[cur_school] = 1;
			n_school_checked++;
		}
	}
	if (info)
		std::cout << "EADAM Algorithm Complete!" << std::endl;
	return S;
}

