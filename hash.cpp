#include "solution.h"
#include "hash.h"
#include <string>
#include <iostream>
#include <sstream>

std::size_t SolutionHash::operator()(const Solution& Sol) const {
	std::stringstream ss;
	for (int i = 0; i < Sol.n_stud; i++) {
		ss << Sol.stud_sol[i];
	}
	return std::hash<std::string>()(ss.str());
}

// Good for model subject to max($n_stud$, $n_school$) < $MOD$.
const int MOD = 97;
std::size_t BPHash::operator()(const std::pair<int, int>& p) const {
	return std::hash<int>()(p.first * MOD + p.second);
}
