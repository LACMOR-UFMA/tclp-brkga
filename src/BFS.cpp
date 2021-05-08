#ifndef BFS_CPP_
#define BFS_CPP_

#include "BFS.h"

int *BFS(int init_node, int NMAX, vector<int> *adjlist)
{
	int *visited = new int[NMAX];
	int *distance = new int[NMAX];

	for (int i = 0; i < NMAX; ++i)
	{
		visited[i] = 0;
		distance[i] = -1;
	}

	distance[init_node] = 0;
	visited[init_node] = 1;
	queue<int> bfsq;
	bfsq.push(init_node);

	//BFS
	while (!bfsq.empty())
	{
		int u = bfsq.front();

		bfsq.pop();

		//Now look at all neighbours of u
		for (int i = 0; i < adjlist[u].size(); i++)
		{
			int v = adjlist[u][i];
			//If v has not been processed yet, do that now
			if (!visited[v])
			{
				visited[v] = 1;
				distance[v] = distance[u] + 1;
				bfsq.push(v);
			}
		}
	}
	//We are done. We have the reachability and distances to all vertices

	return visited;
}

Path BFS_OD(ProblemInstance &p, int cdt, vector<set<int>> Neighbor, uint n_cores)
{

	int *visited = new int[p.NbNode];
	int *distance = new int[p.NbNode];
	int *pred = new int[p.NbNode];
	bool flag = false;

	Path path(&p, cdt);

	for (int i = 0; i < p.NbNode; ++i)
	{
		visited[i] = 0;
		distance[i] = -1;
		pred[i] = -1;
	}

	distance[p.commodity[cdt].orig] = 0;
	visited[p.commodity[cdt].orig] = 1;
	queue<int> bfsq;
	bfsq.push(p.commodity[cdt].orig);

	while (!bfsq.empty())
	{
		int u = bfsq.front();
		bfsq.pop();

		for (set<int>::iterator it = Neighbor[u].begin(); it != Neighbor[u].end(); ++it)
		{
			int v = (*it);

			// if v has not been processed yet, do that now
			if (!visited[v])
			{

				visited[v] = 1;
				distance[v] = distance[u] + 1;
				pred[v] = u;

				if (v == p.commodity[cdt].dest)
				{
					flag = true;
					break;
				}

				bfsq.push(v);
			}
		}

		if (flag)
		{
			break;
		};
	}

	//We are done. We have the reachability and distances to all vertices
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

	delete[] distance;
	delete[] pred;
	delete[] visited;

	return path;
}

Path BFS_OD_2(ProblemInstance &p, int cdt, vector<vector<int>> Neighbor)
{

	int *visited = new int[p.NbNode];
	int *distance = new int[p.NbNode];
	int *pred = new int[p.NbNode];
	bool flag = false;

	Path path(&p, cdt);

	for (int i = 0; i < p.NbNode; ++i)
	{
		visited[i] = 0;
		distance[i] = -1;
		pred[i] = -1;
	}

	distance[p.commodity[cdt].orig] = 0;
	visited[p.commodity[cdt].orig] = 1;
	queue<int> bfsq;
	bfsq.push(p.commodity[cdt].orig);

	//BFS
	while (!bfsq.empty())
	{
		int u = bfsq.front();

		bfsq.pop();
		//Now look at all neighbours of u

		for (vector<int>::iterator it = Neighbor[u].begin(); it != Neighbor[u].end(); ++it)
		{
			int v = (*it);

			//If v has not been processed yet, do that now
			if (!visited[v])
			{
				visited[v] = 1;
				distance[v] = distance[u] + 1;
				pred[v] = u;
				if (v == p.commodity[cdt].dest)
				{
					flag = true;
					break;
				}
				bfsq.push(v);
			}
		}
		if (flag)
		{
			break;
		}
	}
	//We are done. We have the reachability and distances to all vertices

	int key = p.commodity[cdt].dest;
	while (key != -1)
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

	delete[] distance;
	delete[] pred;
	delete[] visited;

	return path;
}
#endif /* BFS_CPP_ */
