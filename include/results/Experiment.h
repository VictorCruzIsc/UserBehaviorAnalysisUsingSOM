#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "SamplesResult.h"
#include "ExperimentAverageAnalysis.h"

using namespace std;

class Experiment{
	public:
		Experiment(int experimentId, vector<int> &userIds);
		int getExperimentId(){ return _experimentId; }
		vector<SamplesResult *> getExperimentResults(){
			return _experimentResults;
		}
		map<int, ExperimentAverageAnalysis *> getExperimentAverageAnalysis(){
			return _experimentAverageAnalysis;
		}
		void addResult(SamplesResult *result);
		SamplesResult * getResult(int index);
		bool samplesResultsCorrect();
		void processExperimentAverages(int totalUsersEvaluated);
		void experimentInfo();

	private:
		int _experimentId;
		vector<SamplesResult *> _experimentResults;
		map<int, ExperimentAverageAnalysis *> _experimentAverageAnalysis;
		vector<int> _userIds;
};

#endif