#include <iostream>
#include <cstring>
#include <string>
#include "model.h"
#include "solver.h"
#include "solution.h"

using namespace std;

int main(int argc, char* argv[]) {
	string filename_in, filename_out;

	cout << "Please enter the input file path and the output file path: " << endl;
	cin >> filename_in >> filename_out;

	Model M;
	M.init(filename_in);

	Solver S;
	S.init(M);

	Solution Sol = S.fast_EADAM();
	
	Sol.output(filename_out);

	return 0;
}