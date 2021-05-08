#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "ProblemInstance.h"
#include "Solution.h"
#include "FWChrono.h"
#include "BFS.h"
#include "Construtivo.h"
#include "BRKGA.h"
#include "MTRand.h"
#include "SampleDecoder.h"
#include <ctime>
#include "Mycallback.h"

using namespace std;

int main(int argc, char *argv[])
{
	long seed = time(NULL);

	if (argc < 7)
	{
		cerr << "<path_to_instance> <% c1> <% c2> <% c3> <seed> <n_threads>" << endl;
		exit(1);
	}

	const char *instanceFile = argv[1];
	double nc1, nc2, nc3;
	unsigned _MAXT;

	sscanf(argv[2], "%lf", &nc1);
	sscanf(argv[3], "%lf", &nc2);
	sscanf(argv[4], "%lf", &nc3);
	sscanf(argv[5], "%ld", &seed);
	sscanf(argv[6], "%d", &_MAXT);

	if (nc1 + nc3 + nc2 > 100)
	{
		throw range_error("Construction heuristics cumulative percentage sum greater than 100.");
	}

	time_t start, end;

	Scanner scanner(new File(instanceFile));

	ProblemInstance p(scanner);
	ProblemInstance _problema = p;

	unsigned _n = p.NbEdge;
	unsigned _p = 100;
	double _pe = 0.30;
	double _pm = 0.20;
	double _rhoe = 0.70;
	unsigned _K = 1;
	unsigned _generation = 0;
	unsigned _X_INTVL = 15;
	unsigned _X_NUMBER = 2;
	unsigned _MAX_GENS = 200;

	SampleDecoder decoder(_problema); // initialize the decoder

	const long unsigned rngSeed = 0; // seed to the random number generator
	MTRand rng(rngSeed);			 // initialize the random number generator

	nc1 = _p * nc1 / 100.0;
	nc2 = _p * nc2 / 100.0;
	nc3 = _p * nc3 / 100.0;

	time(&start);

	// initialize the BRKGA-based heuristic
	BRKGA<SampleDecoder, MTRand> algorithm(nc1, nc2, nc3, _problema, _n, _p, _pe, _pm, _rhoe, decoder, rng, _K, _MAXT);

	clock_t initialClock, finalClock;
	initialClock = clock();
	do
	{
		algorithm.evolve(); // evolve the population for one generation
		if ((++_generation) % _X_INTVL == 0)
		{
			algorithm.exchangeElite(_X_NUMBER); // exchange top individuals
		}

		finalClock = clock();
	} while (_generation < _MAX_GENS && ((double)finalClock - initialClock) / CLOCKS_PER_SEC < 3600);

	time(&end);

	printf("%s;%.2f;%.2f\n", instanceFile, algorithm.getBestFitness(), double(end - start));

	return 0;
}
