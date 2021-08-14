#ifndef SOLUTION_CPP_
#define SOLUTION_CPP_

#include "Solution.h"

void Solution::add_Edge(int i)
{
	Edge.insert(i);
}

void Solution::set_Cost(double objval)
{
	cost = objval;
}

ostream &operator<<(ostream &strm, Solution &s)
{
	for (set<int>::iterator it = s.Edge.begin(); it != s.Edge.end(); ++it)
	{
		strm << *it << " ";
	}

	return strm;
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
		Path path(&p);
		path = BFS(p, k, Neighbor);

		if (path.getPath().size() > 0)
		{
#pragma omp atomic
			inviability_degree++;
		}
	}

	return inviability_degree;
}

#endif // SOLUTION_CPP_
