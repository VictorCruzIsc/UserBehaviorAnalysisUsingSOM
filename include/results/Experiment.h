#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "SamplesResult.h"

using namespace std;

class Experiment{
	public:
		Experiment(int experimentId);
		int getExperimentId(){ return _experimentId; }
		vector<SamplesResult *> getExperimentResults(){ return _experimentResults; }
		void addResult(SamplesResult *result);
		SamplesResult * getResult(int index);
		bool samplesResultsCorrect();
		void experimentInfo();

	private:
		int _experimentId;
		vector<SamplesResult *> _experimentResults;
};

#endif