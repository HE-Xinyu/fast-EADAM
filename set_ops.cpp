#include "set_ops.h"
#include "solution.h"

bool subset(unordered_set<pair<int, int>, BPHash> A, unordered_set<pair<int, int>, BPHash> B) {
	for (auto p : B)
		if (A.find(p) == A.end()) return false;
	return true;
}

bool subset(unordered_set<Solution, SolutionHash> A, unordered_set<Solution, SolutionHash> B) {
	for (auto p : B)
		if (A.find(p) == A.end()) return false;
	return true;
}

bool equals(unordered_set<Solution, SolutionHash> A, unordered_set<Solution, SolutionHash> B) {
	if (A.size() != B.size()) return false;
	for (auto sol : B)
		if (A.find(sol) == A.end()) return false;
	return true;
}
