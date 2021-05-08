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

double SampleDecoder::decode(const std::vector<double> &chromosome, int cores) const
{
	unsigned n = chromosome.size();
	unsigned i, nContadores = 0;
	double fitness = 0.0;
	double M = 9999.0;
	Solution s(cores);

	for (i = 0; i < n; i++)
	{
		if (chromosome[i] > 0.5)
		{
			s.add_Edge(i);
		}
	}

	int inviavel = s.CheckFeas(p, cores);

	return (inviavel * M) + s.getEdge().size();
}
