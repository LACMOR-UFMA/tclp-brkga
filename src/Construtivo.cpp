#ifndef CONSTRUTIVO_CPP_
#define CONSTRUTIVO_CPP_

#include "Construtivo.h"

Construtivo::Construtivo(ProblemInstance &_p, int cores) : p(_p), cores(cores)
{
}

Solution Construtivo::C1()
{
	Solution s;
	vector<int> Par;

	for (int k = 0; k < p.NbK; ++k)
	{
		Par.push_back(k);
	}

	while (Par.size() > 0)
	{
		int index = rand() % Par.size();
		int k = Par[index];

		if (p.Neighbor[p.commodity[k].orig].size() <= p.Neighbor[p.commodity[k].dest].size())
		{
			for (vector<int>::iterator it = p.Neighbor[p.commodity[k].orig].begin(); it != p.Neighbor[p.commodity[k].orig].end(); ++it)
			{
				if (p.mapNodesToEdge.find(make_pair(p.commodity[k].orig, *it))->second != -1)
				{
					s.add_Edge(p.mapNodesToEdge.find(make_pair(p.commodity[k].orig, *it))->second);
				}
				else
				{
					s.add_Edge(p.mapNodesToEdge.find(make_pair(*it, p.commodity[k].orig))->second);
				}
			}
		}
		else
		{
			for (vector<int>::iterator it = p.Neighbor[p.commodity[k].dest].begin(); it != p.Neighbor[p.commodity[k].dest].end(); ++it)
			{
				if (p.mapNodesToEdge.find(make_pair(p.commodity[k].dest, *it))->second != -1)
				{
					s.add_Edge(p.mapNodesToEdge.find(make_pair(p.commodity[k].dest, *it))->second);
				}
				else
				{
					s.add_Edge(p.mapNodesToEdge.find(make_pair(*it, p.commodity[k].dest))->second);
				}
			}
		}

		Par.erase(Par.begin() + index);
	}

	return s;
}

Solution Construtivo::C2()
{
	Solution s;

	bool Flag = false;
	int posa, a, t, h;
	vector<set<int>> Neighbor;
	vector<int> Par;

	for (int k = 0; k < p.NbK; ++k)
	{
		Par.push_back(k);
	}

	for (int i = 0; i < this->p.NbNode; ++i)
	{
		set<int> temp;
		Neighbor.push_back(temp);
		for (vector<int>::iterator it = this->p.Neighbor[i].begin(); it != this->p.Neighbor[i].end(); ++it)
		{
			Neighbor[i].insert(*it);
		}
	}

	int index;
	while (Par.size() > 0)
	{
		Flag = true;
		index = rand() % Par.size();
		while (Flag)
		{
			ProblemPath path(&this->p, Par[index]);
			path = BFS_OD(this->p, Par[index], Neighbor);
			if (path.getPath().size() > 0)
			{
				posa = rand() % path.getPath().size();
				a = path.getPath()[posa];
				s.add_Edge(a);
				t = this->p.edge[a].tail;
				h = this->p.edge[a].head;
				Neighbor[h].erase(t);
				Neighbor[t].erase(h);
			}
			else
			{
				Flag = false;
				Par.erase(Par.begin() + index);
			}
		}
	}

	return s;
}

Solution Construtivo::C3()
{
	Solution s;
	vector<int> Par;
	vector<set<int>> Neighbor;
	int cont = 0;

	for (int k = 0; k < p.NbK; ++k)
	{
		Par.push_back(k);
	}

	for (int i = 0; i < this->p.NbNode; ++i)
	{
		set<int> temp;
		Neighbor.push_back(temp);
		for (vector<int>::iterator it = this->p.Neighbor[i].begin(); it != this->p.Neighbor[i].end(); ++it)
		{
			Neighbor[i].insert(*it);
		}
	}

	int index;

	while (Par.size() > 0)
	{

		index = rand() % Par.size();
		MinCut alg = MinCut();

		alg.run_maxflow(p.NbNode, Neighbor);
		alg.generate_min_cut(p.commodity[Par[index]].orig, p.commodity[Par[index]].dest);

		for (int i = 0; i < p.NbNode; ++i)
		{
			if (alg.is_node_in_cut(i))
			{
				for (vector<int>::iterator it = p.Neighbor[i].begin(); it != p.Neighbor[i].end(); ++it)
				{

					if (!alg.is_node_in_cut((*it)))
					{

						if (p.mapNodesToEdge.find(make_pair(i, *it))->second != -1)
						{
							s.add_Edge(p.mapNodesToEdge.find(make_pair(i, *it))->second);
							Neighbor[i].erase(*it);
							Neighbor[*it].erase(i);
						}
						else
						{
							s.add_Edge(p.mapNodesToEdge.find(make_pair(*it, i))->second);
							Neighbor[i].erase(*it);
							Neighbor[*it].erase(i);
						}
					}
				}
			}
		}

		Par.erase(Par.begin() + index);

		if (Par.size() > 0)
		{
			vector<int>::iterator it;
			it = Par.begin();
			do
			{
				if (alg.is_node_in_cut(p.commodity[*it].orig) and !alg.is_node_in_cut(p.commodity[*it].dest))
				{
					Par.erase(it);
					it--;
					cont++;
				}
				else if (alg.is_node_in_cut(p.commodity[*it].dest) and !alg.is_node_in_cut(p.commodity[*it].orig))
				{
					Par.erase(it);
					it--;
					cont++;
				};

				it++;
			} while (it != Par.end());
		}
	}

	return s;
}

#endif // CONSTRUTIVO_CPP_
