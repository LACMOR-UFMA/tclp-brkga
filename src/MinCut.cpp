#include "MinCut.h"
using namespace std;
using namespace lemon;

MinCut::MinCut() {
	g = NULL;
	alg = NULL;
	dist = NULL;
	cut = NULL;
}

MinCut::~MinCut() {
	delete(g);
	delete(alg);
	delete(dist);
	delete(cut);
}

void MinCut::init() {
	if (g != NULL)
		delete g;
	if (alg != NULL)
		delete alg;
	if (dist != NULL)
		delete dist;
	this->N = 0;
	this->g = new SmartGraph();
	this->dist = new SmartGraph::EdgeMap<double>(*g);
}

void MinCut::build_graph(int N, vector<set<int> > Neighbor){
	for(int i=0;i<N;++i){
			addNode();
	}

	for(int i=0;i<N;++i){
		for(set<int>::iterator j=Neighbor[i].begin();j!=Neighbor[i].end();++j){
				if(i<(*j)){
					int id = addArc(i,(*j));
					(*dist)[g->edgeFromId(id)] = 1;
				}
		}
	}
}

void MinCut::build_graph_2(int N, vector<set<int> > Neighbor){


	for(int i=0;i<N;++i){
		addNode();
	}

	for(int i=0;i<N;++i){
		for(set<int>::iterator j=Neighbor[i].begin();j!=Neighbor[i].end();++j){
			if(i<(*j)){

				int id = addArc(i,(*j));
				(*dist)[g->edgeFromId(id)] = 1;
			}else if(i==N-1){
				int id = addArc((*j),i);
				(*dist)[g->edgeFromId(id)] = 10*N;
			}
		}
	}


}

void MinCut::run_maxflow(int N, vector<set<int> > Neighbor){
	init();

	build_graph(N,Neighbor);

	run();
}

void MinCut::run_maxflow_2(int N, vector<set<int> > Neighbor){
	init();


	build_graph_2(N,Neighbor);


	run();


}

int MinCut::addNode() {
	SmartGraph::Node node = g->addNode();
	++N;
	return g->id(node);
}

int MinCut::addArc(int i, int j) {
	SmartGraph::Node node_i = g->nodeFromId(i);
	SmartGraph::Node node_j = g->nodeFromId(j);
	SmartGraph::Edge e = g->addEdge(node_i,node_j);
	return g->id(e);
}

void MinCut::addCost(int e_id, double cost){
	SmartGraph::Edge e = g->edgeFromId(e_id);
	(*dist)[e]=cost;
}

bool MinCut::is_node_in_cut(int n){
	SmartGraph::Node node = g->nodeFromId(n);
	return (*cut)[node];
}

double MinCut::generate_min_cut(int source, int target){
	if(cut!=NULL)
		delete cut;
	cut = new SmartGraph::NodeMap<bool>(*g);
	double val = this->alg->minCutMap(g->nodeFromId(source),g->nodeFromId(target),*cut);
	return val;
}

void MinCut::run(){
	if(alg!=NULL)
		delete alg;
	this->alg = new GomoryHu<SmartGraph,SmartGraph::EdgeMap<double> >(*g,*dist);
	alg->run();
}
