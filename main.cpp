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

	cout << "Please choose which algorithm to run: " << endl;
	cout << "1: Gale-Shapley Algorithm (DA);" << endl;
	cout << "2: EADAM." << endl;

	int i;
	cin >> i;

	Solution Sol;

	switch (i)
	{
	case 1:
		Sol = S.Gale_Shapley();
		break;
	case 2:
		Sol = S.fast_EADAM();
		break;
	}
	
	Sol.output(filename_out);

	return 0;
}