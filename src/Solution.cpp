#ifndef SOLUTION_CPP_
#define SOLUTION_CPP_

#include "Solution.h"
#include "omp.h"

void Solution::add_Edge(int i)
{
	this->Edge.insert(i);
}

int Solution::checkFeasibility(TCLP &tclp, ProblemInstance &p)
{
	uint edge, violations = 0;
	set<int> edges = this->getEdge();

	vector<pair<uint, uint>> pairs = tclp.discretizedPodsToLemon;
	SmartGraph::EdgeMap<bool> edgeMap(tclp.graph, true);
	SmartGraph::NodeMap<bool> nodeMap(tclp.graph, true);

	for (set<int>::iterator it = edges.begin(); it != edges.end(); ++it)
	{
		edge = tclp.edgesToLemon.at(p.edge[*it].ID);
		edgeMap[tclp.graph.edgeFromId(edge)] = false;
	}

	SubGraph<SmartGraph> subgraph(tclp.graph, nodeMap, edgeMap);
	for (uint i = 0; i < pairs.size(); ++i)
	{
		if (tclp.hasPath(subgraph, pairs[i].first, pairs[i].second))
		{
			violations++;
		}
	}

	return violations;
}

#endif // SOLUTION_CPP_
