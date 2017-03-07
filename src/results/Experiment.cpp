#include "../../include/results/Experiment.h"

Experiment::Experiment(int experimentId, vector<int> &userIds):_experimentId(experimentId),
	_userIds(userIds){}

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
	cout << "======== Experiment "<< (_experimentId + 1) << ": " <<
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
	int currentUser = 0;
	int totalSamples = _experimentResults.size();

	for(int i=0; i<totalUsersEvaluated;  i++){
		currentUser = _userIds[i];
		for(int j=0; j<totalSamples; j++){
			map<int, PercentualAnalysis*> currentMap =
				_experimentResults[j]->getPercentualAnalysis();

			correct += currentMap[currentUser]->getCorrectSamples();
			incorrect += currentMap[currentUser]->getIncorrectSamples();
		}

		if(totalSamples == 0){
			cout << "Experiment totalSamples may cause NAN excpetion" << endl;  
		}

		correct /= totalSamples;
		incorrect /= totalSamples;

		cout << "Values to create experiment average: " << currentUser << " " << correct << " " << incorrect << endl; 
		ExperimentAverageAnalysis *experimentAverageAnalysis = new ExperimentAverageAnalysis(
			currentUser, correct, incorrect);

		_experimentAverageAnalysis[currentUser] = experimentAverageAnalysis;

		correct = 0.0;
		incorrect = 0.0;
	}
}