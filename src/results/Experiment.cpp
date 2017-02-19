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
	cout << "Experiment "<< _experimentId << ": " <<
	(samplesResultsCorrect() ? "correct" : "incorrect") << endl;
}