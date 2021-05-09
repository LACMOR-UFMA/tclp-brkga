#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <list>
#include <iterator>
#include <map>
#include <math.h>
#include <unordered_map>

using namespace std;

class Parameters
{
public:
    Parameters(const char *fileName);
    virtual ~Parameters();

    unordered_map<uint, uint> getPods();
    unordered_map<uint, pair<uint, uint>> getEdges();

    uint getNEdges();
    uint getNPods();
    uint getPODByIndex(uint index);
    uint getEdgeByIndex(uint index);
    vector<uint> *getVectorPods();

protected:
    void load();
    void readInstanceHead();
    void readInstanceParameters();
    void readInstancePods();
    void readInstanceEdges();

private:
    uint nNodes, nEdges, nPODS;

    unordered_map<uint, uint> pods;
    unordered_map<uint, pair<uint, uint>> edges;

    vector<uint> *vectorPods;
    vector<uint> *vectorEdges;

    ifstream file;
};

#endif // PARAMETERS_H
