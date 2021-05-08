#ifndef CONSTRUTIVO_CPP_
#define CONSTRUTIVO_CPP_

#include "Construtivo.h"

Construtivo::Construtivo(ProblemInstance &_p, int cores) : p(_p), cores(cores)
{
}

Construtivo::~Construtivo()
{
}

Solution Construtivo::C1()
{
	Solution s(cores);
	double custo = 0;
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

	custo = s.getEdge().size();
	s.set_Cost(custo);

	return s;
}

Solution Construtivo::C1BRASIL()
{
	Solution s(cores);
	double custo = 0;
	int i, j;
	for (int k = 0; k < p.NbK; ++k)
	{
		i = p.Neighbor[p.commodity[k].orig].at(0);
		j = p.Neighbor[p.commodity[k].dest].at(0);
		if (p.Neighbor[i].size() <= p.Neighbor[j].size())
		{
			for (vector<int>::iterator it = p.Neighbor[i].begin(); it != p.Neighbor[i].end(); ++it)
			{
				if (p.mapNodesToEdge.find(make_pair(i, *it))->second != -1)
				{
					s.add_Edge(p.mapNodesToEdge.find(make_pair(i, *it))->second);
				}
				else
				{
					s.add_Edge(p.mapNodesToEdge.find(make_pair(*it, i))->second);
				}
			}
			s.rm_Edge(p.mapNodesToEdge.find(make_pair(p.commodity[k].orig, i))->second);
			s.rm_Edge(p.mapNodesToEdge.find(make_pair(i, p.commodity[k].orig))->second);
		}
		else
		{
			for (vector<int>::iterator it = p.Neighbor[j].begin(); it != p.Neighbor[j].end(); ++it)
			{
				if (p.mapNodesToEdge.find(make_pair(j, *it))->second != -1)
				{
					s.add_Edge(p.mapNodesToEdge.find(make_pair(j, *it))->second);
				}
				else
				{
					s.add_Edge(p.mapNodesToEdge.find(make_pair(*it, j))->second);
				}
			}
			s.rm_Edge(p.mapNodesToEdge.find(make_pair(p.commodity[k].dest, j))->second);
			s.rm_Edge(p.mapNodesToEdge.find(make_pair(j, p.commodity[k].dest))->second);
		}
	}
	custo = s.getEdge().size();
	s.set_Cost(custo);

	return s;
}

Solution Construtivo::C2(long seed, uint n_cores)
{

	Solution s(cores);
	srand(seed);
	double custo = 0;
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
			Path path(&this->p, Par[index]);
			path = BFS_OD(this->p, Par[index], Neighbor, n_cores);
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

	custo = s.getEdge().size();
	s.set_Cost(custo);

	return s;
}

Solution Construtivo::C3()
{
	Solution s(cores);
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

		double flow = alg.generate_min_cut(p.commodity[Par[index]].orig, p.commodity[Par[index]].dest); //Calculando Corte Mínimo

		s.set_Cost(s.getCost() + flow);

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

Solution Construtivo::C3FAST()
{
	Solution s(cores);
	vector<set<pair<int, int>>> NeighborOD = p.Set_OD_s;
	vector<set<int>> Neighbor = p.Neighbor_Set;

	int pindex;
	int index;

	while (NeighborOD.size() != 0)
	{

		pindex = rand() % NeighborOD.size();
		index = NeighborOD[pindex].begin()->first;

		set<int> temp;
		Neighbor.push_back(temp);

		for (set<pair<int, int>>::iterator j = NeighborOD[pindex].begin(); j != NeighborOD[pindex].end(); ++j)
		{
			Neighbor[p.NbNode].insert(j->second);
		}

		MinCut alg = MinCut();

		alg.run_maxflow_2(p.NbNode + 1, Neighbor);

		double flow = alg.generate_min_cut(index, p.NbNode); //Calculando Corte Mínimo

		s.set_Cost(s.getCost() + flow);

		for (int k = 0; k < p.NbNode; ++k)
		{
			if (alg.is_node_in_cut(k))
			{
				for (vector<int>::iterator it = p.Neighbor[k].begin(); it != p.Neighbor[k].end(); ++it)
				{
					if (!alg.is_node_in_cut((*it)))
					{
						if (p.mapNodesToEdge.find(make_pair(k, *it))->second != -1)
						{
							s.add_Edge(p.mapNodesToEdge.find(make_pair(k, *it))->second);
							Neighbor[k].erase(*it);
							Neighbor[*it].erase(k);
						}
						else
						{
							s.add_Edge(p.mapNodesToEdge.find(make_pair(*it, k))->second);
							Neighbor[k].erase(*it);
							Neighbor[*it].erase(k);
						}
					}
				}
			}
		}

		Neighbor.pop_back();
		NeighborOD.erase(NeighborOD.begin() + pindex);
	}

	return s;
}

Solution Construtivo::C3_F2()
{
	Solution s(cores);

	vector<set<pair<int, int>>> NeighborOD = p.Set_OD_s;
	vector<set<int>> Neighbor = p.Neighbor_Set;

	int pindex;
	int index;
	int cont = 0;
	while (NeighborOD.size() > 0)
	{
		pindex = rand() % NeighborOD.size();
		index = NeighborOD[pindex].begin()->first;

		set<int> temp;
		Neighbor.push_back(temp);

		for (set<pair<int, int>>::iterator j = NeighborOD[pindex].begin(); j != NeighborOD[pindex].end(); ++j)
		{
			Neighbor[p.NbNode].insert(j->second);
		}

		MinCut alg = MinCut();

		alg.run_maxflow_2(p.NbNode + 1, Neighbor);

		double flow = alg.generate_min_cut(index, p.NbNode); //Calculando Corte Mínimo

		s.set_Cost(s.getCost() + flow);

		for (int k = 0; k < p.NbNode; ++k)
		{
			if (alg.is_node_in_cut(k))
			{
				for (vector<int>::iterator it = p.Neighbor[k].begin(); it != p.Neighbor[k].end(); ++it)
				{
					if (!alg.is_node_in_cut((*it)))
					{
						if (p.mapNodesToEdge.find(make_pair(k, *it))->second != -1)
						{
							s.add_Edge(p.mapNodesToEdge.find(make_pair(k, *it))->second);
							Neighbor[k].erase(*it);
							Neighbor[*it].erase(k);
						}
						else
						{
							s.add_Edge(p.mapNodesToEdge.find(make_pair(*it, k))->second);
							Neighbor[k].erase(*it);
							Neighbor[*it].erase(k);
						}
					}
				}
			}
		}

		Neighbor.pop_back();

		NeighborOD.erase(NeighborOD.begin() + pindex);

		for (unsigned i = 0; NeighborOD.size() > 0 && i < NeighborOD.size(); ++i)
		{
			set<pair<int, int>>::iterator it;
			it = NeighborOD[i].begin();
			do
			{
				if (alg.is_node_in_cut(it->first) and !alg.is_node_in_cut(it->second))
				{
					NeighborOD[i].erase(*it);
					it--;
					cont++;
				}
				else if (alg.is_node_in_cut(it->second) and !alg.is_node_in_cut(it->first))
				{
					NeighborOD[i].erase(*it);
					it--;
					cont++;
				};

				if (it->first != it->second)
				{
					it++;
				}
				else
				{
					break;
				}
			} while (it != NeighborOD[i].end());
		}

		for (unsigned i = 0; NeighborOD.size() > 0 && i < NeighborOD.size(); ++i)
		{
			if (NeighborOD[i].size() < 1)
			{
				NeighborOD.erase(NeighborOD.begin() + i);
			}
		}
	}

	return s;
}

void Construtivo::CheckFeas(Solution s)
{
}

#endif // CONSTRUTIVO_CPP_
