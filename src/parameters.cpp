#include "parameters.h"

Parameters::Parameters(const char *fileName)
{
    this->file.open(fileName);
    this->load();
}

void Parameters::load()
{
    if (!this->file)
    {
        std::cout << "Error on open instance file!" << endl;
        exit(1);
    }

    this->readInstanceHead();
    this->readInstanceParameters();

    this->readInstancePods();
    this->readInstanceEdges();

    this->file.close();
}

void Parameters::readInstanceHead()
{
    string leitor;
    for (uint i = 0; i < 3; i++)
    {
        if (!(this->file >> leitor))
        {
            cout << "Error on reading instance file!" << endl;
            exit(1);
        }
    }
}

void Parameters::readInstanceParameters()
{
    uint n[3];
    for (uint i = 0; i < 3; i++)
    {
        if (!(this->file >> n[i]))
        {
            cout << "Error on reading instance file!" << endl;
            exit(1);
        }
    }

    this->nNodes = n[0];
    this->nEdges = n[1];
    this->nPODS = n[2];
}

void Parameters::readInstancePods()
{
    uint readPods = 0;
    uint pod;
    this->vectorPods = new vector<uint>[this->nPODS];

    do
    {
        this->file >> pod;
        this->pods.insert({pod, readPods + 1});
        this->vectorPods->push_back(pod);
        readPods++;
    } while (readPods < this->nPODS);
}

void Parameters::readInstanceEdges()
{
    uint readPairs = 0;
    uint i, linePosition[3];

    do
    {
        i = 0;
        do
        {
            if (!(this->file >> linePosition[i]))
            {
                cout << "Error on reading instance file!" << endl;
                exit(1);
            }
            i++;
        } while (i < 3);

        this->edges.insert({linePosition[0], {linePosition[1], linePosition[2]}});
        readPairs++;
    } while (readPairs < this->nEdges);
}

unordered_map<uint, uint> Parameters::getPods()
{
    return this->pods;
}

unordered_map<uint, pair<uint, uint>> Parameters::getEdges()
{
    return this->edges;
}

uint Parameters::getNEdges()
{
    return this->nEdges;
}

uint Parameters::getNPods()
{
    return this->nPODS;
}

uint Parameters::getPODByIndex(uint index)
{
    return this->vectorPods->at(index);
}

uint Parameters::getEdgeByIndex(uint index)
{
    return this->vectorEdges[index];
}

vector<uint> *Parameters::getVectorPods()
{
    return this->vectorPods;
}
