#ifndef MinCut_H_
#define MinCut_H_

#include <lemon/gomory_hu.h>
#include <lemon/smart_graph.h>
#include <vector>
#include <set>

using namespace std;

class MinCut
{
private:
	int N;
	lemon::SmartGraph *g;
	lemon::GomoryHu<lemon::SmartGraph, lemon::SmartGraph::EdgeMap<double>> *alg;
	lemon::SmartGraph::EdgeMap<double> *dist;
	lemon::SmartGraph::NodeMap<bool> *cut;

public:
	MinCut();
	void init();
	void build_graph(int N, vector<set<int>> Neighbor);
	void build_graph_2(int N, vector<set<int>> Neighbor);
	void run_maxflow(int N, vector<set<int>> Neighbor);
	void run_maxflow_2(int N, vector<set<int>> Neighbor);
	int addNode();
	int addArc(int i, int j);
	void addCost(int e_id, double cost);
	bool is_node_in_cut(int n);
	void run();
	double generate_min_cut(int source, int target);
	~MinCut();
};

#endif /* MinCut_H_ */
