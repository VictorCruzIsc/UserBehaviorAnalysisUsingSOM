#ifndef EXPERIMENT_GENERIC_H
#define EXPERIMENT_GENERIC_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "EvaluationGeneric.h"

using namespace std;

class ExperimentGeneric{
public:
	ExperimentGeneric(int experimentId, vector<int> &userIds,
		vector<EvaluationGeneric *> &evaluations,
		vector<int> &evaluatedVectors):_experimentId(experimentId),
	_userIds(userIds), _evaluations(evaluations),
	_evaluatedVectors(evaluatedVectors){
		_totalEvaluatedUsers = _userIds.size();
		_totalEvaluations = _evaluations.size();
	}
	int getExperimentId(){ return _experimentId; }
	int getTotalEvaluatedUsers(){ return _totalEvaluatedUsers; }
	int getTotalEvaluations(){return _totalEvaluations; }
	vector<int> getUserIds(){ return _userIds; }
	vector<EvaluationGeneric *> getEvaluations(){ return _evaluations; }
	vector<int> getEvaluatedVectors(){ return _evaluatedVectors; }

	vector<int> getMatrixResults(int userId, int vectorsEvaluated);
	vector<double> getMatrixResultsPercentages(int userId, int vectorsEvaluated);
	void info();
private:
	int _experimentId;
	int _totalEvaluatedUsers;
	int _totalEvaluations;
	vector<int> _userIds;
	vector<int> _evaluatedVectors;
	vector<EvaluationGeneric *> _evaluations;		
};
#endif