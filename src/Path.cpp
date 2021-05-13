#ifndef PATH_CPP_
#define PATH_CPP_

#include "Path.h"

void Path::add_Edge(int e)
{
	this->path.push_back(e);
}

ostream &operator<<(ostream &strm, Path &s)
{
	for (vector<int>::iterator it = s.path.end() - 1; it != s.path.begin() - 1; --it)
	{
		strm << *it << " ";
	}

	return strm;
}

#endif /* PATH_CPP_ */
