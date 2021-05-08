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
#include "Path.h"
#include "MinCut.h"

using namespace std;

class Construtivo
{
private:
	ProblemInstance &p;
	int cores;

public:
	Construtivo(ProblemInstance &_p, int cores);
	~Construtivo();
	Solution C1();
	Solution C1BRASIL();
	Solution C2(long seed, uint n_cores);
	Solution C3();
	Solution C3FAST();
	Solution C3_F2();
	void CheckFeas(Solution s);
};

#endif /* CONSTRUTIVO_H_ */
