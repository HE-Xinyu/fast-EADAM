#include "solution.h"
#include <string>
#include <iostream>
#include <sstream>
#pragma once

class SolutionHash {
public:
	std::size_t operator()(const Solution& Sol) const;
};

class BPHash {
public:
	std::size_t operator()(const std::pair<int, int>& p) const;
};

