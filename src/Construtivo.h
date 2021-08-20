#ifndef CONSTRUTIVO_H_
#define CONSTRUTIVO_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include "Solution.h"
#include "ProblemInstance.h"
#include "BFS.h"
#include "MinCut.h"

using namespace std;

class Construtivo
{
private:
	ProblemInstance &p;
	int cores;

public:
	Construtivo(ProblemInstance &_p, int cores);
	Solution C1();
	Solution C2();
	Solution C3();
};

#endif /* CONSTRUTIVO_H_ */
