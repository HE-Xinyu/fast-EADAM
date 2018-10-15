#include <iostream>
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
			if (S.stud_sol[i] == -1) {
				while (head_stud[i] != M.n_school) {
					bool exit = false;
					int cur_school = M.stud_pref[i][head_stud[i]];
					bool flag = S.school_worst[cur_school] == -1 || 
						M.school_pos[cur_school][i] > M.school_pos[cur_school][S.school_worst[cur_school]];

					if (S.school_cnt[cur_school] < M.seat[cur_school]) {
						S.add(i, cur_school, flag);
						exit = true;
					}
					else if (!flag) {
						int dropped_stud = S.school_worst[cur_school];
						int j;
						for (j = M.school_pos[cur_school][dropped_stud] - 1; j >= 0; j--)
							if (S.sol_matrix[M.school_pref[cur_school][j]][cur_school] || 
								(M.school_pref[cur_school][j] == i))
								break;
						S.change(i, dropped_stud, cur_school, M.school_pref[cur_school][j]);
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
