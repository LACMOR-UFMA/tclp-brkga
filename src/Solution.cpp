#ifndef SOLUTION_CPP_
#define SOLUTION_CPP_

#include "Solution.h"
#include <omp.h>

void Solution::add_Edge(int i)
{
	Edge.insert(i);
}

int Solution::checkFeasibility(ProblemInstance p, uint n_cores)
{

	vector<set<int>> Neighbor;

	set<int> Contador = this->getEdge();

#pragma omp parallel for schedule(dynamic) num_threads(this->n_cores)
	for (int i = 0; i < p.NbNode; ++i)
	{
		set<int> temp;
		Neighbor.push_back(temp);
		for (vector<int>::iterator it = p.Neighbor[i].begin(); it != p.Neighbor[i].end(); ++it)
		{
			Neighbor[i].insert(*it);
		}
	}

	for (set<int>::iterator it = Contador.begin(); it != Contador.end(); ++it)
	{
		Neighbor[p.edge[*it].tail].erase(p.edge[*it].head);
		Neighbor[p.edge[*it].head].erase(p.edge[*it].tail);
	}

	int inviability_degree = 0;

#pragma omp parallel for schedule(dynamic) num_threads(this->n_cores)
	for (int k = 0; (k < p.NbK); ++k)
	{
		ProblemPath path(&p, k);
		path = BFS_OD(p, k, Neighbor, this->n_cores);

		if (path.getPath().size() > 0)
		{
#pragma omp atomic
			inviability_degree++;
		}
	}

	return inviability_degree;
}

int Solution::CheckFeas2(ProblemInstance p)
{

	vector<vector<int>> Neighborhood;

	for (int i = 0; i < p.NbNode; ++i)
	{
		vector<int> Neighbor;
		Neighbor.assign(p.Neighbor[i].begin(), p.Neighbor[i].end());

		for (vector<int>::iterator it = p.Neighbor[i].begin(); it != p.Neighbor[i].end(); ++it)
		{
			for (set<int>::iterator it2 = Edge.begin(); Edge.size() > 0 && it2 != Edge.end(); ++it2)
			{
				if ((*it) == p.edge[(*it2)].tail)
				{
					Neighbor.erase(
						remove(Neighbor.begin(), Neighbor.end(),
							   p.edge[(*it2)].tail),
						Neighbor.end());
				}
				else if ((*it) == p.edge[(*it2)].head)
				{
					Neighbor.erase(
						remove(Neighbor.begin(), Neighbor.end(),
							   p.edge[(*it2)].head),
						Neighbor.end());
				}
			}
		}

		Neighborhood.push_back(Neighbor);
	}

	int grau_inviabilidade = 0;
	for (int k = 0; k < p.NbK; ++k)
	{
		ProblemPath path(&p, k);
		path = BFS_OD_2(p, k, Neighborhood);
		if (path.getPath().size() > 0)
		{
			grau_inviabilidade++;
		}
	}
	return grau_inviabilidade;
}

int Solution::CheckFeas3(TCLP *tclp)
{
	uint edge, violations = 0;
	set<int> edges = this->getEdge();

	vector<pair<uint, uint>> pairs = tclp->discretizedPodsToLemon;
	SmartGraph::EdgeMap<bool> edgeMap(tclp->graph, false);

	for (set<int>::iterator it = edges.begin(); it != edges.end(); ++it)
	{
		edgeMap[tclp->graph.edgeFromId(*it)] = true;
	}

	SubGraph<SmartGraph> subgraph(tclp->graph, *tclp->nodeMap, edgeMap);

	ofstream output;
  	
	output.open ("output.eps");

	graphToEps(subgraph, output).scale(10).nodeScale(2).arcWidthScale(.4).run();

	output.close();

#pragma omp parallel for num_threads(this->n_cores)
	for (uint i = 0; i < pairs.size(); ++i)
	{
		if (!tclp->hasPath(subgraph, pairs[i].first, pairs[i].second))
		{
			violations++;
			i = pairs.size();
		}
	}

	return violations;
}

#endif // SOLUTION_CPP_
