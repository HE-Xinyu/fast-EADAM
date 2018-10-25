#include <iostream>
#include <deque>
#include <unordered_set>
#include "solver.h"
#include "solution.h"
#include "model.h"

Solver::Solver() {
	
}

void Solver::init(Model M) {
	this->M = M;
}

Solution Solver::Gale_Shapley() {
	Solution S;
	S.init(M.n_stud, M.n_school, M.n_seat);

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

	return S;
}

Solution Solver::fast_EADAM() {
	Solution S = Gale_Shapley();

	std::deque <int> Q;

	int* checked = new int[M.n_school];
	int* visited_school = new int[M.n_school];
	int* visited_stud = new int[M.n_stud];
	int* head = new int[M.n_school];
	memset(checked, 0, sizeof(int) * M.n_school);
	memset(head, 0, sizeof(int) * M.n_school);
	memset(visited_school, 0, sizeof(int) * M.n_school);
	memset(visited_stud, 0, sizeof(int) * M.n_stud);

	int n_school_checked = 0;
	int cur_school = 0;

	for (int i = 0; i < M.n_school; i++) {
		if (S.school_worst[i] == -1)
			head[i] = M.n_stud;
		else 
			head[i] = M.school_pos[i][S.school_worst[i]] + 1;
	}
	

	while (n_school_checked != M.n_school) {
		if (Q.empty()) {
			for (int i = 0; i < M.n_school && (!checked[i]); i++) {
				Q.push_back(i);
				cur_school = i;
				visited_school[i] = 1;
				break;
			}
		}

		if (checked[cur_school]) {
			Q.pop_back();
			visited_school[cur_school] = 0;
			if (Q.size() >= 2) {
				int tmp_stud = Q.back();
				Q.pop_back();
				visited_stud[tmp_stud] = 0;
				int tmp_school = Q.back();
				head[tmp_school]++;
				cur_school = tmp_school;
			}
			else {
				while (checked[cur_school])
					cur_school = (cur_school + 1) % M.n_school;

				Q.push_back(cur_school);
				visited_school[cur_school] = 1;
			}
		}

		bool flag = false;

		for (int i = head[cur_school]; i < M.n_stud; i++) {
			int cur_stud = M.school_pref[cur_school][i];
			int sol_school = S.stud_sol[cur_stud];
			if (M.stud_pos[cur_stud][cur_school] < M.stud_pos[cur_stud][sol_school]) {
				if (visited_stud[cur_stud]) {
					Q.push_back(cur_stud);
					while (true) {
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

		if ((!flag) && head[cur_school] == M.n_stud) {
			checked[cur_school] = 1;
			n_school_checked++;
		}
	}
	return S;
}

