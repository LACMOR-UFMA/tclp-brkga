#ifndef BFS_CPP_
#define BFS_CPP_

#include "BFS.h"

ProblemPath BFS_OD(ProblemInstance &p, int cdt, vector<set<int>> Neighbor, uint n_cores)
{

	int *visited = new int[p.NbNode];
	int *pred = new int[p.NbNode];
	bool flag = false;

	ProblemPath path(&p, cdt);

	for (int i = 0; i < p.NbNode; ++i)
	{
		visited[i] = 0;
		pred[i] = -1;
	}

	queue<int> bfsq;

	visited[p.commodity[cdt].orig] = 1;
	bfsq.push(p.commodity[cdt].orig);

	while (!bfsq.empty() && !flag)
	{
		int u = bfsq.front();
		bfsq.pop();

		for (set<int>::iterator it = Neighbor[u].begin(); it != Neighbor[u].end(); ++it)
		{
			int v = (*it);

			if (!visited[v])
			{
				visited[v] = 1;
				pred[v] = u;

				if (v == p.commodity[cdt].dest)
				{
					flag = true;
					break;
				}

				bfsq.push(v);
			}
		}
	}

	int key = p.commodity[cdt].dest;

	while (pred[key] != -1)
	{
		if (p.mapNodesToEdge[make_pair(key, pred[key])] != -1)
		{
			path.add_Edge(p.mapNodesToEdge[make_pair(key, pred[key])]);
		}
		else
		{
			path.add_Edge(p.mapNodesToEdge[make_pair(pred[key], key)]);
		}
		key = pred[key];
	}

	delete[] pred;
	delete[] visited;

	return path;
}

#endif /* BFS_CPP_ */
