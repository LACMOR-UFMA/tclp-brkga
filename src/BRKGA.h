/*
 * BRKGA.h
 *
 * This class encapsulates a Biased Random-key Genetic Algorithm (for minimization problems) with K
 * independent Populations stored in two vectors of Population, current and previous. It supports
 * multi-threading via OpenMP, and implements the following key methods:
 *
 * - BRKGA() constructor: initializes the populations with parameters described below.
 * - evolve() operator: evolve each Population following the BRKGA methodology. This method
 *                      supports OpenMP to evolve up to K independent Populations in parallel.
 *                      Please note that double Decoder::decode(...) MUST be thread-safe.
 *
 * Required hyperparameters:
 * - n: number of genes in each chromosome
 * - p: number of elements in each population
 * - pe: pct of elite items into each population
 * - pm: pct of mutants introduced at each generation into the population
 * - rhoe: probability that an offspring inherits the allele of its elite parent
 *
 * Optional parameters:
 * - K: number of independent Populations
 * - MAX_THREADS: number of threads to perform parallel decoding -- WARNING: Decoder::decode() MUST
 *                be thread-safe!
 *
 * Required templates are:
 * RNG: random number generator that implements the methods below.
 *     - RNG(unsigned long seed) to initialize a new RNG with 'seed'
 *     - double rand() to return a double precision random deviate in range [0,1)
 *     - unsigned long randInt() to return a >=32-bit unsigned random deviate in range [0,2^32-1)
 *     - unsigned long randInt(N) to return a unsigned random deviate in range [0, N] with N < 2^32
 *
 * Decoder: problem-specific decoder that implements any of the decode methods outlined below. When
 *          compiling and linking BRKGA with -fopenmp (i.e., with multithreading support via
 *          OpenMP), the method must be thread-safe.
 *     - double decode(const vector< double >& chromosome) const, if you don't want to change
 *       chromosomes inside the framework, or
 *     - double decode(vector< double >& chromosome) const, if you'd like to update a chromosome
 *
 *  Created on : Jun 22, 2010 by rtoso
 *  Last update: Sep 28, 2010 by rtoso
 *      Authors: Rodrigo Franco Toso <rtoso@cs.rutgers.edu>
 */

//Artigo refer??ncia: A C++ application programming interface for biased random-key genetic algorithms

#ifndef BRKGA_H
#define BRKGA_H

#include <omp.h>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include "Population.h"
#include "ProblemInstance.h"
#include "Solution.h"

template <class Decoder, class RNG>
class BRKGA
{
public:
	/*
	 * Default constructor
	 * Required hyperparameters:
	 * - n: number of genes in each chromosome
	 * - p: number of elements in each population
	 * - pe: pct of elite items into each population
	 * - pm: pct of mutants introduced at each generation into the population
	 * - rhoe: probability that an offspring inherits the allele of its elite parent
	 *
	 * Optional parameters:
	 * - K: number of independent Populations
	 * - MAX_THREADS: number of threads to perform parallel decoding
	 *                WARNING: Decoder::decode() MUST be thread-safe; safe if implemented as
	 *                + double Decoder::decode(std::vector< double >& chromosome) const
	 */
#include <list>
#include <vector>
#include <algorithm>
	BRKGA(double nC1, double nC2, double nC3,
		  ProblemInstance _instance, unsigned n, unsigned p, double pe,
		  double pm, double rhoe, const Decoder &refDecoder, RNG &refRNG,
		  unsigned K, unsigned MAX_THREADS);

	/**
	 * Destructor
	 */
	~BRKGA();

	/**
	 * Resets all populations with brand new keys
	 */
	void reset();

	/**
	 * Evolve the current populations following the guidelines of BRKGAs
	 * @param generations number of generations (must be even and nonzero)
	 * @param J interval to exchange elite chromosomes (must be even; 0 ==> no synchronization)
	 * @param M number of elite chromosomes to select from each population in order to exchange
	 */
	void evolve(unsigned generations = 1);

	/**
	 * Exchange elite-solutions between the populations
	 * @param M number of elite chromosomes to select from each population
	 */
	void exchangeElite(unsigned M);

	/**
	 * Returns the current population
	 */
	const Population &getPopulation(unsigned k = 0) const;

	/**
	 * Returns the chromosome with best fitness so far among all populations
	 */
	const std::vector<double> &getBestChromosome() const;

	/**
	 * Returns the best fitness found so far among all populations
	 */
	double getBestFitness() const;

	// Return copies to the internal parameters:
	unsigned getN() const;
	unsigned getP() const;
	unsigned getPe() const;
	unsigned getPm() const;
	unsigned getPo() const;
	double getRhoe() const;
	unsigned getK() const;
	unsigned getMAX_THREADS() const;

private:
	// Heuristics constructors
	double nC1, nC2, nC3;
	ProblemInstance instance;

	// Hyperparameters:
	const unsigned n;  // number of genes in the chromosome
	const unsigned p;  // number of elements in the population
	const unsigned pe; // number of elite items in the population
	const unsigned pm; // number of mutants introduced at each generation into the population
	const double rhoe; // probability that an offspring inherits the allele of its elite parent

	// Templates:
	const Decoder &refDecoder; // reference to the problem-dependent Decoder
	RNG &refRNG;			   // reference to the random number generator

	// Parallel populations parameters:
	const unsigned K; // number of independent parallel populations
	int MAX_THREADS;  // number of threads for parallel decoding

	// Data:
	std::vector<Population *> previous; // previous populations
	std::vector<Population *> current;	// current populations

	// Local operations:
	void initialize(const unsigned i); // initialize current population 'i' with random keys
	void evolution(Population &curr, Population &next);
};

template <class Decoder, class RNG>
BRKGA<Decoder, RNG>::BRKGA(double nc1, double nc2, double nc3,
						   ProblemInstance _instance, unsigned _n, unsigned _p, double _pe,
						   double _pm, double _rhoe, const Decoder &decoder, RNG &rng, unsigned _K,
						   unsigned MAX) : nC1(nc1), nC2(nc2), nC3(nc3), instance(_instance), n(_n), p(_p), pe(unsigned(_pe *p)), pm(unsigned(_pm *p)), rhoe(_rhoe), refDecoder(decoder), refRNG(rng), K(_K), MAX_THREADS(MAX), previous(K, 0), current(K, 0)
{
	// Error check:
	using std::range_error;

	if (n == 0)
	{
		throw range_error("Chromosome size equals zero.");
	}
	if (p == 0)
	{
		throw range_error("Population size equals zero.");
	}
	if (pe == 0)
	{
		throw range_error("Elite-set size equals zero.");
	}
	if (pe > p)
	{
		throw range_error(
			"Elite-set size greater than population size (pe > p).");
	}
	if (pm > p)
	{
		throw range_error(
			"Mutant-set size (pm) greater than population size (p).");
	}
	if (pe + pm > p)
	{
		throw range_error(
			"elite + mutant sets greater than population size (p).");
	}
	if (K == 0)
	{
		throw range_error("Number of parallel populations cannot be zero.");
	}

	// Initialize and decode each chromosome of the current population, then copy to previous:
	for (unsigned i = 0; i < K; ++i)
	{
		// Allocate:
		current[i] = new Population(n, p);

		// Initialize:
		initialize(i);

		// Then just copy to previous:
		previous[i] = new Population(*current[i]);
	}
}

template <class Decoder, class RNG>
BRKGA<Decoder, RNG>::~BRKGA()
{
	for (unsigned i = 0; i < K; ++i)
	{
		delete current[i];
		delete previous[i];
	}
}

template <class Decoder, class RNG>
const Population &BRKGA<Decoder, RNG>::getPopulation(unsigned k) const
{
	return (*current[k]);
}

template <class Decoder, class RNG>
double BRKGA<Decoder, RNG>::getBestFitness() const
{
	double best = current[0]->fitness[0].first;
	for (unsigned i = 1; i < K; ++i)
	{
		if (current[i]->fitness[0].first < best)
		{
			best = current[i]->fitness[0].first;
		}
	}

	return best;
}

template <class Decoder, class RNG>
const std::vector<double> &BRKGA<Decoder, RNG>::getBestChromosome() const
{
	unsigned bestK = 0;
	for (unsigned i = 1; i < K; ++i)
	{
		if (current[i]->getBestFitness() < current[bestK]->getBestFitness())
		{
			bestK = i;
		}
	}

	return current[bestK]->getChromosome(0); // The top one :-)
}

template <class Decoder, class RNG>
void BRKGA<Decoder, RNG>::reset()
{
	for (unsigned i = 0; i < K; ++i)
	{
		initialize(i);
	}
}

template <class Decoder, class RNG>
void BRKGA<Decoder, RNG>::evolve(unsigned generations)
{
	if (generations == 0)
	{
		throw std::range_error("Cannot evolve for 0 generations.");
	}

	for (unsigned i = 0; i < generations; ++i)
	{
		for (unsigned j = 0; j < K; ++j)
		{
			evolution(*current[j], *previous[j]); // First evolve the population (curr, next)
			std::swap(current[j], previous[j]);	  // Update (prev = curr; curr = prev == next)
		}
	}
}

template <class Decoder, class RNG>
void BRKGA<Decoder, RNG>::exchangeElite(unsigned M)
{
	if (M == 0 || M >= p)
	{
		throw std::range_error("M cannot be zero or >= p.");
	}

	for (unsigned i = 0; i < K; ++i)
	{
		// Population i will receive some elite members from each Population j below:
		unsigned dest = p - 1; // Last chromosome of i (will be updated below)
		for (unsigned j = 0; j < K; ++j)
		{
			if (j == i)
			{
				continue;
			}

			// Copy the M best of Population j into Population i:
			for (unsigned m = 0; m < M; ++m)
			{
				// Copy the m-th best of Population j into the 'dest'-th position of Population i:
				const std::vector<double> &bestOfJ = current[j]->getChromosome(
					m);

				std::copy(bestOfJ.begin(), bestOfJ.end(),
						  current[i]->getChromosome(dest).begin());

				current[i]->fitness[dest].first = current[j]->fitness[m].first;

				--dest;
			}
		}
	}

	for (int j = 0; j < int(K); ++j)
	{
		current[j]->sortFitness();
	}
}

template <class Decoder, class RNG>
inline void BRKGA<Decoder, RNG>::initialize(const unsigned i)
{
	Construtivo builder(instance, MAX_THREADS);
	uint j = 0;
	set<int> solution;

	//	initialize
	if (nC1 > 0)
	{
		while (j < nC1)
		{
			solution = builder.C1().getEdge();
			for (set<int>::iterator it = solution.begin(); it != solution.end(); ++it)
			{
				unsigned index = *it;
				(*current[i])(j, index) = 0.999;
			}
			j++;
		}
	}

	int cum = j;
	if (nC2 > 0)
	{
		while (j < nC2 + cum)
		{
			solution = builder.C2().getEdge();
			for (set<int>::iterator it = solution.begin(); it != solution.end(); ++it)
			{
				unsigned index = *it;
				(*current[i])(j, index) = 0.999;
			}
			j++;
		}
	}

	cum = j;
	if (nC3 > 0)
	{
		while (j < nC3 + cum)
		{
			solution = builder.C3().getEdge();
			for (set<int>::iterator it = solution.begin(); it != solution.end(); ++it)
			{
				unsigned index = *it;
				(*current[i])(j, index) = 0.999;
			}
			j++;
		}
	}

	while (j < p)
	{
		for (unsigned k = 0; k < n; ++k)
		{
			(*current[i])(j, k) = refRNG.rand();
		}
		j++;
	}

	int k = 0;

#pragma omp parallel for if (MAX_THREADS > 1) schedule(dynamic) num_threads(MAX_THREADS) private(k)
	for (k = 0; k < int(p); ++k)
	{
		const vector<double> &cromossomo = (*current[i])(k);
		current[i]->setFitness(k, refDecoder.decode(cromossomo));
	}

	// Sort:
	current[i]->sortFitness();
}

template <class Decoder, class RNG>
inline void BRKGA<Decoder, RNG>::evolution(Population &curr, Population &next)
{
	// We now will set every chromosome of 'current', iterating with 'i':
	unsigned i = 0; // Iterate chromosome by chromosome
	unsigned j = 0; // Iterate allele by allele

	// 2. The 'pe' best chromosomes are maintained, so we just copy these into 'current':
	while (i < pe)
	{
		for (j = 0; j < n; ++j)
		{
			next(i, j) = curr(curr.fitness[i].second, j);
		}

		next.fitness[i].first = curr.fitness[i].first;
		next.fitness[i].second = i;
		++i;
	}

	// 3. We'll mate 'p - pe - pm' pairs; initially, i = pe, so we need to iterate until i < p - pm:
	while (i < p - pm)
	{
		// Select anunsigned elite parent:
		const unsigned eliteParent = (refRNG.randInt(pe - 1));

		// Select a non-elite parent:
		const unsigned noneliteParent = pe + (refRNG.randInt(p - pe - 1));

		// Mate:
		for (j = 0; j < n; ++j)
		{
			const unsigned sourceParent = ((refRNG.rand() < rhoe) ? eliteParent : noneliteParent);

			next(i, j) = curr(curr.fitness[sourceParent].second, j);
		}

		++i;
	}

	// We'll introduce 'pm' mutants:
	while (i < p)
	{
		for (j = 0; j < n; ++j)
		{
			next(i, j) = refRNG.rand();
		}
		++i;
	}

	int k;

#pragma omp parallel for if (MAX_THREADS > 1) schedule(dynamic) num_threads(MAX_THREADS) private(k)
	for (k = int(pe); k < int(p); ++k)
	{
		const vector<double> &cromossomo = next.population[k];
		next.setFitness(k, refDecoder.decode(cromossomo));
	}

	// Now we must sort 'current' by fitness, since things might have changed:
	next.sortFitness();
}

template <class Decoder, class RNG>
unsigned BRKGA<Decoder, RNG>::getN() const
{
	return n;
}

template <class Decoder, class RNG>
unsigned BRKGA<Decoder, RNG>::getP() const
{
	return p;
}

template <class Decoder, class RNG>
unsigned BRKGA<Decoder, RNG>::getPe() const
{
	return pe;
}

template <class Decoder, class RNG>
unsigned BRKGA<Decoder, RNG>::getPm() const
{
	return pm;
}

template <class Decoder, class RNG>
unsigned BRKGA<Decoder, RNG>::getPo() const
{
	return p - pe - pm;
}

template <class Decoder, class RNG>
double BRKGA<Decoder, RNG>::getRhoe() const
{
	return rhoe;
}

template <class Decoder, class RNG>
unsigned BRKGA<Decoder, RNG>::getK() const
{
	return K;
}

template <class Decoder, class RNG>
unsigned BRKGA<Decoder, RNG>::getMAX_THREADS() const
{
	return MAX_THREADS;
}

#endif
