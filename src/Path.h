#ifndef PATH_H_
#define PATH_H_

#include <iostream>
#include <stdio.h>
#include <vector>

#include "ProblemInstance.h"

using namespace std;

class Path
{
private:
	ProblemInstance *p;

	vector<int> path;

	friend ostream &operator<<(ostream &, Path &);

public:
	Path(ProblemInstance *_p)
	{
		p = _p;
	}

	~Path()
	{
		p = NULL;
		delete (p);
	}

	void add_Edge(int e);
	vector<int> getPath() { return path; };
};

#endif /* PATH_H_ */
