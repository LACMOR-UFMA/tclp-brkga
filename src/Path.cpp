#ifndef PATH_CPP_
#define PATH_CPP_
#include "Path.h"

void Path::add_Edge(int e){
	path.push_back(e);
	setImp(e);
}


void Path::setImp(int e){
	//if(impacto>p->edge[e].imp[commodity]){
	//	impacto = p->edge[e].imp[commodity];
	//}
}

ostream& operator<<(ostream& strm, Path& s){
	for(vector<int>::iterator it = s.path.end()-1; it!=s.path.begin()-1; --it){
			strm<<*it<<" ";
	}

	return strm;
}
#endif /* PATH_CPP_ */

//0 545 411 401 391 386 end()-1 begin()
//0 545 411 401 391 386 546 end()-1 begin()-1
//546 386 391 401 411 545 0  begin() end()
