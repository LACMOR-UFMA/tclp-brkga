#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <iostream>
#include <vector>
#include <set>
#include "tclp.h"
#include "ProblemInstance.h"

using namespace std;

class Solution
{
private:
	double cost;

public:
	set<int> Edge;
	Solution()
	{
		cost = 0;
	};

	void add_Edge(int i);
	set<int> getEdge() { return this->Edge; };
	int checkFeasibility(TCLP &tclp, ProblemInstance &p);
};

#endif //SOLUTION_H_
