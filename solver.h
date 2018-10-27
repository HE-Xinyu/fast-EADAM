#include "model.h"
#include "solution.h"
#pragma once

class Solver {
private:
	Model M;

public:
	Solver();
	void init(Model M);

	Solution Gale_Shapley(bool info=true);
	Solution fast_EADAM(bool info=true);
};
