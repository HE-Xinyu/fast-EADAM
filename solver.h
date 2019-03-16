#include "model.h"
#include "solution.h"
#include "hash.h"
#include <unordered_set>
#pragma once

using namespace std;

class Solver {

public:
	Solver();
	~Solver();

	Solution Gale_Shapley(const Model& M, bool info=true);
	Solution fast_EADAM(const Model& M, bool info=true);
	unordered_set<Solution, SolutionHash> one_step_improvement(const Solution& Sol, const Model& M);
	unordered_set<Solution, SolutionHash> vnm_stable_set(const Model& M);
	unordered_set<pair<int, int>, BPHash> blocking_pairs(const Solution& Sol, const Model& M);
	unordered_set<Solution, SolutionHash> bp_generate_set(const Model& M);
	double welfare(const Model& M, const Solution& Sol);
};
