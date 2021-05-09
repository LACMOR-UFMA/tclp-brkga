#include "tclp.h"

TCLP::TCLP(Parameters *p)
{
    this->p = p;
    this->initialize();
}

TCLP::~TCLP()
{
}

void TCLP::initialize()
{
    for (auto &it : this->p->getEdges())
    {
        SmartGraph::Node uNode;
        SmartGraph::Node vNode;
        SmartGraph::Edge edge;

        uNode = this->buildNode(it.second.first);
        vNode = this->buildNode(it.second.second);
        edge = this->graph.addEdge(uNode, vNode);

        this->edgesToLemon.insert({it.first, this->graph.id(edge)});
    }
    this->discretizePodsToLemon();
    this->nodeMap = new SmartGraph::NodeMap<bool>(this->graph, true);
    this->edgeMap = new SmartGraph::EdgeMap<bool>(this->graph, false);
}

SmartGraph::Node TCLP::buildNode(unsigned int nodeId)
{
    SmartGraph::Node node;

    if (this->nodesToLemon.find(nodeId) == this->nodesToLemon.end())
    {
        node = this->graph.addNode();
        this->nodesToLemon.insert({nodeId, this->graph.id(node)});
    }
    else
    {
        node = this->graph.nodeFromId(this->nodesToLemon[nodeId]);
    }

    if (this->p->getPods().find(nodeId) != this->p->getPods().end())
    {
        this->podsToLemon[nodeId] = this->graph.id(node);
    }

    return node;
}

void TCLP::discretizePodsToLemon()
{
    uint nPods, count = 0;

    nPods = this->p->getNPods() * (this->p->getNPods() - 1) / 2;

    this->discretizedPodsToLemon = new vector<pair<uint, uint>>[nPods];

    for (uint i = 0; i < this->p->getNPods() - 1; i++)
    {
        for (uint j = i + 1; j < this->p->getNPods(); j++)
        {
            uint origin = this->podsToLemon.operator[](this->p->getPODByIndex(i));
            uint destiny = this->podsToLemon.operator[](this->p->getPODByIndex(j));
            this->discretizedPodsToLemon->push_back({origin, destiny});
            ++count;
        }
    }
}

bool TCLP::hasPath(SubGraph<SmartGraph>::Node u, SubGraph<SmartGraph>::Node v)
{
    Bfs<SubGraph<SmartGraph>> bfs(*this->subGraph);
    return bfs.run(u, v);
}
