/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "SampleDecoder.h"

SampleDecoder::SampleDecoder(ProblemInstance _p)
{
	p = _p;
}

SampleDecoder::~SampleDecoder()
{
}

uint64_t SampleDecoder::hashCode(uint v[], uint size) const
{
	uint64_t cumsum = 0;
	for (uint i = 0; i < size; ++i)
	{
		cumsum += v[i] * i;
	}
	return cumsum;
}

double SampleDecoder::decode(const std::vector<double> &chromosome, int cores) const
{
	Solution s(cores);
	size_t chromosomeSize = chromosome.size();
	uint c[chromosomeSize];
	double M = 9999.0;

	for (uint i = 0; i < chromosomeSize; i++)
	{
		if (chromosome[i] > 0.5)
		{
			c[i] = 1;
			s.add_Edge(i);
		}
		else
		{
			c[i] = 0;
		}
	}

	uint64_t hashcode = this->hashCode(c, chromosomeSize);
	map<uint64_t, double>::const_iterator it = this->memoization.find(hashcode);

	if (it != this->memoization.end())
	{
		return it->second;
	}
	else
	{
		size_t edgeSize = s.getEdge().size();
		int violations = s.checkFeasibility(p, cores);
		double result = (violations * M) + edgeSize;

		if (violations == 0)
		{
			this->memoization[hashcode] = result;
		}

		return result;
	}
}

std::vector<double> *SampleDecoder::localSearch(const std::vector<double> &chromosome, double fitness, int cores) const
{
	int randPosition;
	double oldGeneValue, newFitness;

	vector<double> *newChromossome = new vector<double>(chromosome.size(), 0.F);
	vector<double> edges;

	for (uint i = 0; i < chromosome.size(); i++)
	{
		if (chromosome[i] > 0.5)
		{
			edges.push_back(i);
		}
	}

	std::copy(chromosome.begin(), chromosome.end(), newChromossome->begin());

	for (uint i = 0; i < 30; i++)
	{
		randPosition = edges[rand() % edges.size()];

		oldGeneValue = newChromossome->at(randPosition);
		newChromossome->at(randPosition) = 0.F;
		newFitness = this->decode(*newChromossome, cores);

		if (newFitness < fitness)
		{
			cout << "local search found better solution. Old fitness: " << fitness << " New fitness: " << newFitness << endl;
			fitness = newFitness;
		}
		else
		{
			// put edge back to solution
			newChromossome->at(randPosition) = oldGeneValue;
		}
	}

	return newChromossome;
}