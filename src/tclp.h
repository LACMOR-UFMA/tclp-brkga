#ifndef TCLP_H
#define TCLP_H

#include "parameters.h"
#include <lemon/smart_graph.h>
#include <lemon/bfs.h>
#include <lemon/adaptors.h>
#include <lemon/concepts/graph.h>

using namespace std;
using namespace lemon;

class TCLP
{
public:
    TCLP(Parameters *p);

    virtual ~TCLP();

    void initialize();

    SmartGraph::Node buildNode(unsigned int nodeId);

    void discretizePodsToLemon();

    bool hasPath(SubGraph<SmartGraph> subgraph, uint uId, uint vId);

    Parameters *p;

    SmartGraph graph;

    SmartGraph::NodeMap<bool> *nodeMap;
    SmartGraph::EdgeMap<bool> *edgeMap;

    unordered_map<unsigned int, unsigned int> nodesToLemon;
    unordered_map<unsigned int, unsigned int> edgesToLemon;
    unordered_map<unsigned int, unsigned int> podsToLemon;

    vector<pair<uint, uint>> discretizedPodsToLemon;
};

#endif // TCLP_H
