/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "SampleDecoder.h"

SampleDecoder::SampleDecoder(ProblemInstance &_p, TCLP &_tclp): tclp(_tclp), p(_p)
{
}

double SampleDecoder::decode(const std::vector<double> &chromosome) const
{
	Solution s;
	size_t chromosomeSize = chromosome.size();
	uint64_t cumSum = 0;
	double M = 9999.0;

	for (uint i = 0; i < chromosomeSize; i++)
	{
		if (chromosome[i] > 0.5)
		{
			cumSum += i;
			s.add_Edge(i);
		}
	}

	size_t edgeSize = s.getEdge().size();

	int violations = s.checkFeasibility(this->tclp, this->p);

	return (violations * M) + edgeSize;
}
