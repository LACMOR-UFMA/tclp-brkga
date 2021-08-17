#ifndef BFS_H_
#define BFS_H_

#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>

#include "ProblemPath.h"

using namespace std;

ProblemPath BFS_OD(ProblemInstance& p, int cdt, vector<set<int> > Neighbor, uint n_cores);

#endif /* BFS_H_ */
