#include "../../include/results/ExperimentGeneric.h"

vector<int> ExperimentGeneric::getMatrixResults(int userId, int vectorsEvaluated){
	for(int i=0 ; i<_totalEvaluations; i++){
		EvaluationGeneric *currentEvaluation = _evaluations[i];
		if(!currentEvaluation->isEvaluationCorrect()){
			continue;
		}
		if(currentEvaluation->getRelatedUser() == userId &&
			currentEvaluation->getTotalEvaluatedVectors() == vectorsEvaluated){
			//cout << "Match " << userId << ":" << vectorsEvaluated << endl;
			vector<int> evaluationData;
			evaluationData.push_back(i);
			evaluationData.push_back(currentEvaluation->getTotalPositive());
			evaluationData.push_back(currentEvaluation->getTotalNegative());
			evaluationData.push_back(currentEvaluation->getTotalIdle());
			return evaluationData;
		}
	}
	cout << "ERRROR: User -> " << userId << " with " << vectorsEvaluated;
	cout << " was not found on the experiment " << _experimentId << endl;
}

vector<double> ExperimentGeneric::getMatrixResultsPercentages(int userId,
	int vectorsEvaluated){	
	for(int i=0 ; i<_totalEvaluations; i++){
		EvaluationGeneric *currentEvaluation = _evaluations[i];
		if(!currentEvaluation->isEvaluationCorrect()){
			continue;
		}
		if(currentEvaluation->getRelatedUser() == userId &&
			currentEvaluation->getTotalEvaluatedVectors() == vectorsEvaluated){
			vector<double> evaluationData;
			evaluationData.push_back(i);
			evaluationData.push_back(currentEvaluation->getPositivePercentage());
			evaluationData.push_back(currentEvaluation->getNegativePercentage());
			evaluationData.push_back(currentEvaluation->getIdlePercentage());
			return evaluationData;
		}
	}
	cout << "ERRROR: User -> " << userId << " with " << vectorsEvaluated;
	cout << " was not found on the experiment " << _experimentId << endl;
}

void ExperimentGeneric::info(){
	cout << "Experiment id: " << _experimentId;
	cout << ", total evaluated users: " << _totalEvaluatedUsers;
	cout << ", _totalEvaluations: " << _totalEvaluations << endl;

	cout << "Evaluations in experiment:" << endl;
	for(int i=0 ; i<_totalEvaluations; i++){
		_evaluations[i]->info();
	}
}