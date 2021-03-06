#ifndef PROBLEMPATH_H_
#define PROBLEMPATH_H_

#include <iostream>
#include <stdio.h>
#include <vector>

#include "ProblemInstance.h"

using namespace std;

class ProblemPath
{
private:
	ProblemInstance *p;

	vector<int> path;
	double impacto;
	int commodity;

	friend ostream &operator<<(ostream &, ProblemPath &);

public:
	ProblemPath(ProblemInstance *_p, int k)
	{
		p = _p;
		impacto = 1000;
		commodity = k;
	}

	~ProblemPath()
	{
		p = NULL;
		delete (p);
	}

	void add_Edge(int e);
	void setImp(int e);

	vector<int> getPath() { return path; };
	double getImp() { return impacto; };
	int getCommodity() { return commodity; };
};

#endif /* PROBLEMPATH_H_ */
