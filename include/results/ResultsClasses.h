#ifndef RESULTS_CLASSES_H
#define RESULTS_CLASSES_H

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <string>

#include "../functionality/Classes.h"
#include "SamplesResult.h"
#include "Experiment.h"
#include "UserGraph.h"
#include "ExperimentAverageAnalysis.h"

#define MAC

using namespace std;

class Results;

class Results{
	public:
		static SamplesResult* evaluateUsers(int samples, int sigma, int totalUsersEvaluated,
			SelfOrganizingMaps *som,
			vector<vector<DataChunck *> > &evaluateDataChunckSetCollection,
			vector<int> &userIds);
		static void getResults(int initialSamples, int finalSamples,
			int increment, int sigma, int experiments, int totalUsersEvaluated,
			SelfOrganizingMaps *som,
			vector<vector<DataChunck *> > &evaluateDataChunckSetCollection,
			vector<int> &userIds);
		static void getBarGraphs(vector<UserGraph *> &graphics, int totalExperiments,
			vector<int> &userIds);
};

#endif