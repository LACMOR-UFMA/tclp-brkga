#ifndef BFS_H_
#define BFS_H_

#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>

#include "Path.h"

using namespace std;

Path BFS(ProblemInstance &p, int cdt, vector<set<int>> Neighbor);

#endif /* BFS_H_ */
