#include "ProblemInstance.h"

ProblemInstance::ProblemInstance()
{
}

ProblemInstance::ProblemInstance(Scanner &scanner)
{
	scanner.nextLine();
	NbNode = scanner.nextInt();
	NbEdge = scanner.nextInt();
	NbCities = scanner.nextInt();
	L = 300;

	NbK = NbCities * (NbCities - 1) / 2;

	City_ID = new int[NbCities];
	commodity = new Commodity[NbK];
	edge = new Edge[NbEdge];

	for (int i = 0; i < NbCities; ++i)
	{
		City_ID[i] = scanner.nextInt();
	}

	int cdt = 0;
	for (int i = 0; i < NbCities - 1; i++)
	{
		for (int j = i + 1; j < NbCities; j++)
		{
			commodity[cdt].orig = City_ID[i];
			commodity[cdt].dest = City_ID[j];
			cdt++;
		}
	}

	for (int e = 0; e < NbEdge; ++e)
	{
		edge[e].ID = scanner.nextInt();
		edge[e].tail = scanner.nextInt();
		edge[e].head = scanner.nextInt();

		Nodes.insert(edge[e].tail);
		Nodes.insert(edge[e].head);
	}

	int n_cont = 0;
	for (set<int>::iterator it = Nodes.begin(); it != Nodes.end(); ++it)
	{
		mapIdToNode[(*it)] = n_cont;
		mapNodeToId[n_cont] = (*it);
		n_cont++;
	}

	for (int i = 0; i < NbCities; ++i)
	{
		City_ID[i] = mapIdToNode[City_ID[i]];
	}

	for (int k = 0; k < NbK; ++k)
	{
		commodity[k].orig = mapIdToNode[commodity[k].orig];
		commodity[k].dest = mapIdToNode[commodity[k].dest];
	}

	Neighbor = new vector<int>[NbNode];

	for (int e = 0; e < NbEdge; ++e)
	{
		edge[e].tail = mapIdToNode[edge[e].tail];
		edge[e].head = mapIdToNode[edge[e].head];
		Neighbor[edge[e].tail].push_back(edge[e].head);
		Neighbor[edge[e].head].push_back(edge[e].tail);
		mapNodesToEdge[make_pair(edge[e].tail, edge[e].head)] = e;
		mapNodesToEdge[make_pair(edge[e].head, edge[e].tail)] = -1;
	}

	for (int i = 0; i < this->NbNode; ++i)
	{
		set<int> temp;
		Neighbor_Set.push_back(temp);
		for (vector<int>::iterator it = Neighbor[i].begin(); it != Neighbor[i].end(); ++it)
		{
			Neighbor_Set[i].insert(*it);
		}
	}
	for (int i = 0; i < NbCities - 1; i++)
	{

		set<pair<int, int>> temp;
		for (int j = i + 1; j < NbCities; j++)
		{
			temp.insert(make_pair(City_ID[i], City_ID[j]));
		}
		Set_OD_s.push_back(temp);
	}

	delete[] City_ID;
}

ProblemInstance::~ProblemInstance()
{
}

void ProblemInstance::printInstance()
{
	cout << NbNode << " " << NbEdge << " " << NbCities << endl;
	getchar();
	for (int i = 0; i < NbCities; ++i)
	{
		cout << mapNodeToId[City_ID[i]] << endl;
	}
	getchar();
	for (int e = 0; e < NbEdge; ++e)
	{
		cout << edge[e].ID << " " << mapNodeToId[edge[e].tail] << " " << mapNodeToId[edge[e].head] << " ";
		cout << endl;
	}
	getchar();
}
