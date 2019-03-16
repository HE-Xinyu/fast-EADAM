#include "hash.h"
#include "solution.h"
#include <unordered_set>
#pragma once

using std::unordered_set;
using std::pair;

bool subset(unordered_set<pair<int, int>, BPHash> A, unordered_set<pair<int, int>, BPHash> B);
bool subset(unordered_set<Solution, SolutionHash> A, unordered_set<Solution, SolutionHash> B);
bool equals(unordered_set<Solution, SolutionHash> A, unordered_set<Solution, SolutionHash> B);
