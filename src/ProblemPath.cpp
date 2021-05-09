#ifndef PROBLEMPATH_CPP_
#define PROBLEMPATH_CPP_
#include "ProblemPath.h"

void ProblemPath::add_Edge(int e)
{
	path.push_back(e);
	setImp(e);
}

void ProblemPath::setImp(int e)
{
	//if(impacto>p->edge[e].imp[commodity]){
	//	impacto = p->edge[e].imp[commodity];
	//}
}

ostream &operator<<(ostream &strm, ProblemPath &s)
{
	for (vector<int>::iterator it = s.path.end() - 1; it != s.path.begin() - 1; --it)
	{
		strm << *it << " ";
	}

	return strm;
}
#endif /* PROBLEMPATH_CPP_ */
