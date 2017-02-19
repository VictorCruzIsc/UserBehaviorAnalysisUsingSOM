#ifndef RESULTS_CLASSES_H
#define RESULTS_CLASSES_H

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>

#include "../functionality/Classes.h"
#include "SamplesResult.h"

using namespace std;

class Results;

class Results{
	public:
		static SamplesResult* evaluateUsers(int samples, int sigma, int totalUsersEvaluated,
			SelfOrganizingMaps *som,
			vector<vector<DataChunck *> > &evaluateDataChunckSetCollection);
		static void getResults(int initialSamples, int finalSamples,
			int increment, int sigma, int experiments, int totalUsersEvaluated,
			SelfOrganizingMaps *som,
			vector<vector<DataChunck *> > &evaluateDataChunckSetCollection);
};

#endif