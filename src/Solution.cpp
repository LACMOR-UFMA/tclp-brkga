#ifndef SOLUTION_CPP_
#define SOLUTION_CPP_

#include "Solution.h"

Solution::~Solution()
{
	Edge.clear();
}

void Solution::add_Edge(int i)
{
	Edge.insert(i);
}

void Solution::set_Cost(double objval)
{
	cost = objval;
}

void Solution::rm_Edge(int i)
{
	Edge.erase(i);
}

ostream &operator<<(ostream &strm, Solution &s)
{
	for (set<int>::iterator it = s.Edge.begin(); it != s.Edge.end(); ++it)
	{
		strm << *it << " ";
	}

	return strm;
}

int Solution::CheckFeas(ProblemInstance p, uint n_cores)
{

	vector<set<int>> Neighbor;

	set<int> Contador = this->getEdge();

#pragma omp parallel for num_threads(this->n_cores)
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

	int grau_inviabilidade = 0;

#pragma omp parallel for num_threads(this->n_cores)
	for (int k = 0; (k < p.NbK); ++k)
	{
		Path path(&p, k);
		path = BFS_OD(p, k, Neighbor, this->n_cores);

		if (path.getPath().size() > 0)
		{

#pragma omp critical
			{
				grau_inviabilidade++;
				k = p.NbK;
			}
		}
	}

	return grau_inviabilidade;
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
		Path path(&p, k);
		path = BFS_OD_2(p, k, Neighborhood);
		if (path.getPath().size() > 0)
		{
			grau_inviabilidade++;
		}
	}
	return grau_inviabilidade;
}

#endif // SOLUTION_CPP_
