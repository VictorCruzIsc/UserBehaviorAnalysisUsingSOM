#include "../../include/results/Experiment.h"

Experiment::Experiment(int experimentId):_experimentId(experimentId){}

void Experiment::addResult(SamplesResult *result){
	_experimentResults.push_back(result);
}

SamplesResult * Experiment::getResult(int index){
	return _experimentResults[index];
}

bool Experiment::samplesResultsCorrect(){
	int totalResults = _experimentResults.size();
	for(int i=0; i<totalResults; i++){
		if(!_experimentResults[i]->isCorrect()){
			return false;
		}
	}
	return true;
}

void Experiment::experimentInfo(){
	cout << "======== Experiment "<< _experimentId << ": " <<
	(samplesResultsCorrect() ? "correct" : "incorrect") << " ========" << endl;

	map<int, ExperimentAverageAnalysis *>::iterator iterator;
	for(iterator = _experimentAverageAnalysis.begin();
		iterator != _experimentAverageAnalysis.end();
		iterator++){
			iterator->second->info();
		}
	cout << "===============" << endl;
}


void Experiment::processExperimentAverages(int totalUsersEvaluated){
	double correct = 0.0;
	double incorrect = 0.0;
	int totalSamples = _experimentResults.size();
	for(int i=1; i<=totalUsersEvaluated;  i++){
		for(int j=0; j<totalSamples; j++){
			map<int, PercentualAnalysis*> currentMap =
				_experimentResults[j]->getPercentualAnalysis();
			correct += currentMap[i]->getCorrectSamples();
			incorrect += currentMap[i]->getIncorrectSamples();
		}

		correct /= totalSamples;
		incorrect /= totalSamples;

		ExperimentAverageAnalysis *experimentAverageAnalysis = new ExperimentAverageAnalysis(
			i, correct, incorrect);

		_experimentAverageAnalysis[i] = experimentAverageAnalysis;

		correct = 0.0;
		incorrect = 0.0;
	}
}