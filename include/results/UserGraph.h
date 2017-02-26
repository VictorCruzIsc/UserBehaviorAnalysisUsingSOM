#ifndef USERGRAPH_H
#define USERGRAPH_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "ExperimentAverageAnalysis.h"

using namespace std;

class UserGraph{
	public:
		UserGraph(int belongingUser, vector<ExperimentAverageAnalysis *> userResults):
			_belongingUser(belongingUser), _userResults(userResults){}
		void info();
	private:
		int _belongingUser;
		vector<ExperimentAverageAnalysis *> _userResults;
};

#endif