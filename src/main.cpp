#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <lemon/time_measure.h>

#include "ProblemInstance.h"
#include "Solution.h"
#include "BFS.h"
#include "Construtivo.h"
#include "BRKGA.h"
#include "MTRand.h"
#include "SampleDecoder.h"
#include <ctime>
#include "Mycallback.h"
#include "parameters.h"
#include <lemon/graph_to_eps.h>

using namespace std;
using namespace lemon;

int main(int argc, char *argv[])
{
	if (argc < 7)
	{
		cerr << "<path_to_instance> <% c1> <% c2> <% c3> <seed> <n_threads>" << endl;
		exit(1);
	}

	uint64_t seed;
	const char *instance_file = argv[1];
	double nc1, nc2, nc3;
	uint MAX_THREADS;

	sscanf(argv[2], "%lf", &nc1);
	sscanf(argv[3], "%lf", &nc2);
	sscanf(argv[4], "%lf", &nc3);
	sscanf(argv[5], "%ld", &seed);
	sscanf(argv[6], "%d", &MAX_THREADS);

	if (nc1 + nc3 + nc2 > 100)
	{
		throw range_error("Construction heuristics cumulative percentage sum greater than 100.");
	}

	Scanner scanner(new File(instance_file));
	ProblemInstance problem_instance(scanner);
	SampleDecoder decoder(problem_instance);

	uint _n = problem_instance.NbEdge;			// number of genes in each chromosome
	uint _p = 20;								// number of elements in each population
	double _pe = 0.30;							// percentage of elite items into each population
	double _pm = 0.20;							// percentage of mutants introduced at each generation into the population
	double _rhoe = 0.70;						// probability that an offspring inherits the allele of its elite parent
	uint _K = 3;								// number of independent Populations
	
	uint generation = 0;
	const uint EXCHANGE_INTERVAL = 15;
	const uint EXCHANGE_NUMBER = 2;
	const uint MAX_GENERATIONS = 100;
	ProblemInstance p(scanner);
	ProblemInstance _problema = p;
	Parameters params(instanceFile);
	TCLP tclp(&params);

	const double LIMIT_TIME = 3600;
	Timer timer(false);
	const uint64_t cSeed = seed;
	unsigned _n = p.NbEdge;
	unsigned _p = 20;
	double _pe = 0.30;
	double _pm = 0.20;
	double _rhoe = 0.70;
	unsigned _K = 1;
	unsigned _generation = 0;
	unsigned _X_INTVL = 15;
	unsigned _X_NUMBER = 2;
	unsigned _MAX_GENS = 100;

	MTRand rng(cSeed);			 				// initialize the random number generator - BRKGA
	srand(cSeed);				 				// initialize the random number generator - Construtivo
	SampleDecoder decoder(_problema, &tclp); 	// initialize the decoder

	timer.start();

	nc1 = _p * nc1 / 100.0;
	nc2 = _p * nc2 / 100.0;
	nc3 = _p * nc3 / 100.0;

	BRKGA<SampleDecoder, MTRand> algorithm(nc1, nc2, nc3, problem_instance, _n, _p, _pe, _pm, _rhoe, decoder, rng, _K, MAX_THREADS);

	do
	{
		algorithm.evolve(); // evolve the population for one generation

		generation++;

		if (generation % EXCHANGE_INTERVAL == 0 && _K > 1)
		{
			algorithm.exchangeElite(EXCHANGE_NUMBER); // exchange top individuals
		}
	} while (generation < MAX_GENERATIONS && timer.realTime() < LIMIT_TIME);

	timer.halt();

	printf("%s;%.2f;%.2f\n", instance_file, algorithm.getBestFitness(), timer.realTime());

	return 0;
}
