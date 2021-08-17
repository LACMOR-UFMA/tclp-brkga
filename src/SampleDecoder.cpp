/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "SampleDecoder.h"

SampleDecoder::SampleDecoder(ProblemInstance *_p, TCLP *_tclp)
{
	p = _p;
	tclp = _tclp;
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
		int violations = s.checkFeasibility(this->tclp, this->p);
		double result = (violations * M) + edgeSize;

		if (violations == 0)
		{
			this->memoization[hashcode] = result;
		}

		return result;
	}
}
