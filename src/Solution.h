#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <iostream>
#include "ProblemInstance.h"
#include "BFS.h"
#include <vector>
#include <set>
#include "tclp.h"
#include "ProblemPath.h"

using namespace std;

class Solution
{
private:
	int n_cores;
	double cost;

public:
	set<int> Edge;
	Solution(int cores) : n_cores(cores)
	{
		cost = 0;
	};

	void add_Edge(int i);

	set<int> getEdge() { return this->Edge; };
	int checkFeasibility(ProblemInstance p, uint n_cores);
	double getCost() { return cost; };
	int CheckFeas(ProblemInstance p, uint n_cores);
	int CheckFeas2(ProblemInstance p);
	int CheckFeas3(TCLP *tclp);
};

#endif //SOLUTION_H_
