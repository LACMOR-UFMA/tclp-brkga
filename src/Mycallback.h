/*
 * Mycallback.h
 *
 *  Created on: 06/10/2017
 *      Author: user
 */

#include "gurobi_c++.h"
#include <iostream>
#include <cstdlib>
//#include <fstream>
#include <sstream>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <ctime>
#include <utility>      // std::pair
#include <math.h>

//using namespace std;

class Mycallback: public GRBCallback {
public:

	int n;
	GRBVar **vars;
	double **matriz_custo;
	int r;
	GRBVar* varsY;
	GRBModel *model;

	Mycallback(GRBVar **xvars, int _r, int qnd_n, double **_matriz_custo,
			GRBVar* _y, GRBModel *m) {
		vars = xvars;
		n = qnd_n;
		matriz_custo = _matriz_custo;
		r = _r;
		varsY = _y;
		model = m;
	}

protected:

	void cut_set(bool debug) {
	}

	void callback() {
		try {
			switch (where) {
			case GRB_CB_MIPNODE:
				if (getIntInfo(GRB_CB_MIPNODE_STATUS) == GRB_OPTIMAL) {

					//					cut_set_MIPNODE_7(false);

				}

				break;

			case GRB_CB_MIPSOL:

				//				cut_set_MIPSOL_7(false);


				break;
			}
		} catch (GRBException e) {
			cout << "Error number: " << e.getErrorCode() << endl;
			cout << e.getMessage() << endl;
		} catch (...) {
			cout << "Error during callback" << endl;
		}

	}
};
