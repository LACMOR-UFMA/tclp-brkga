#ifndef PROBLEMINSTANCE_H_
#define PROBLEMINSTANCE_H_

#include <iostream>
#include <stdio.h>
#include <vector>
#include <set>
#include <map>
#include "Scanner.h"

using namespace std;
using namespace scannerpp;

struct Edge
{
	int ID;
	int tail;
	int head;
};

struct Commodity
{
	int orig;
	int dest;
};

class ProblemInstance
{
public:
	int NbNode;
	int NbEdge;
	int NbCities;
	int NbK;
	int L;

	set<int> Nodes;

	int *City_ID;
	Edge *edge;
	Commodity *commodity;

	map<int, int> mapNodeToId;
	map<int, int> mapIdToNode;
	map<pair<int, int>, int> mapNodesToEdge;

	vector<int> *Neighbor;
	vector<set<int>> Neighbor_Set;

	vector<set<pair<int, int>>> Set_OD_s;

	ProblemInstance(Scanner &scanner);
	ProblemInstance();

	virtual ~ProblemInstance();

	void printInstance();
};

#endif /*PROBLEMINSTANCEEDGES_H_*/
