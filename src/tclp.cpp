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

        edge = findEdge(this->graph, uNode, vNode);
        if (edge == INVALID) {
            edge = this->graph.addEdge(uNode, vNode);
        }

        this->edgesToLemon.insert({it.first, this->graph.id(edge)});
    }
    this->discretizePodsToLemon();
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
    uint nPods = this->p->getNPods();

    for (uint i = 0; i < nPods - 1; ++i)
    {
        for (uint j = i + 1; j < nPods; ++j)
        {
            uint origin = this->podsToLemon[this->p->getPODByIndex(i)];
            uint destiny = this->podsToLemon[this->p->getPODByIndex(j)];
            this->discretizedPodsToLemon.push_back({origin, destiny});
        }
    }
}

bool TCLP::hasPath(SubGraph<SmartGraph> subgraph, uint uId, uint vId)
{
    SubGraph<SmartGraph>::Node u = subgraph.nodeFromId(uId);
    SubGraph<SmartGraph>::Node v = subgraph.nodeFromId(vId);
    Bfs<SubGraph<SmartGraph>> bfs(subgraph);

    bool result = bfs.run(u, v);

    return result;
}
