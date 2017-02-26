#include "../../include/results/SamplesResult.h"

SamplesResult::SamplesResult(int samples, int totalUsersEvaluated, int sigma,
	int totalPositiveCollitions, int totalNegativeCollitions,
	int correct, int incorrect,
	map<int, StadisticsResults *> errorStadisticsResults,
	map<int, StadisticsResults *> correctStadisticsResults,
	bool correctResult):
	_samples(samples), _totalUsersEvaluated(totalUsersEvaluated),
	_expectedNeuronsEvaluated (_samples * _totalUsersEvaluated),
	_sigma(sigma), _totalPositiveCollitions(totalPositiveCollitions),
	_totalNegativeCollitions(totalNegativeCollitions), _correct(correct),
	_incorrect(incorrect),
	_errorStadisticsResults(errorStadisticsResults),
	_correctStadisticsResults(correctStadisticsResults),
	_correctResult(correctResult), _correctNumericalAnalysis(false){
		processNumericAnalysis();
		processPercentualAnalysis();
	}

void SamplesResult::processNumericAnalysis(){
	vector<int> users;
	vector<int> correct;
	vector<int> incorrect;
	map<int, StadisticsResults *>::iterator iterator;
	int currentValue = 0;
	int currentUser = 0;

	for(int i=1; i<=_totalUsersEvaluated; i++){
		iterator = _correctStadisticsResults.find(i+i);
		if(iterator != _correctStadisticsResults.end()){
			currentValue = _correctStadisticsResults[(i+i)]->getValue();
		}else{
			currentValue = 0;
		}
		correct.push_back(currentValue);
	}

	for(int i=1; i<=_totalUsersEvaluated; i++){
		int userTotalIncorrect = 0;
		for(iterator = _errorStadisticsResults.begin(); iterator != _errorStadisticsResults.end(); iterator++){
			StadisticsResults *currentResult = iterator->second;
			if(currentResult->getFinalUserAssigned() == i){
				userTotalIncorrect += currentResult->getValue();
			}
		}
		incorrect.push_back(userTotalIncorrect);
	}

	if((_totalUsersEvaluated != correct.size()) || (_totalUsersEvaluated != incorrect.size())){
		cout << "ERROR: NumericAnalysis is not correct" << endl;
		return;
	}

	for(int i=0; i<_totalUsersEvaluated; i++){
		currentUser = i + 1;
		NumericAnalysis *numericAnalysis = new NumericAnalysis(currentUser, correct[i], incorrect[i]);
		_numericAnalysis[currentUser] = numericAnalysis;
	}

	_correctNumericalAnalysis = true;

#ifdef DEBUG_ANALYSIS
	cout << "Total Positive Colitions: " << _totalPositiveCollitions << endl;
	cout << "Total Negative Colitions: " << _totalNegativeCollitions << endl;
	
	cout << "Correct:" << _correct << endl;
	for(iterator = _correctStadisticsResults.begin(); iterator != _correctStadisticsResults.end(); iterator++){
		cout << iterator->second->info() << endl;
	}

	cout << "Errors:" << _incorrect << endl;
	for(iterator = _errorStadisticsResults.begin(); iterator != _errorStadisticsResults.end(); iterator++){
		cout << iterator->second->info() << endl;
	}

	cout << "" << endl;

	for(int i=1; i<=_totalUsersEvaluated; i++){
		_numericAnalysis[i]->info();	
	}
#endif
}

void SamplesResult::processPercentualAnalysis(){
	if(!_correctNumericalAnalysis){
		cout << "Numerical Analysis is not correct, cannot go further" << endl;
		return;
	}

	for(int i=1; i<=_totalUsersEvaluated; i++){
		NumericAnalysis *numericAnalysis = _numericAnalysis[i];
		int user = numericAnalysis->getUser();
		int totalUnique = numericAnalysis->getUniqueEvaluatedSamples();
		int correct = numericAnalysis->getCorrectSamples();
		int incorrect =  numericAnalysis->getIncorrectSamples();

		double pCorrect = ((double)correct/totalUnique) * 100;
		double pIncorrect = ((double)incorrect/totalUnique) * 100;

		PercentualAnalysis *percentual =  new PercentualAnalysis(user, pCorrect, pIncorrect);

		_percentualAnalysis[i] = percentual;
	}

#ifdef DEBUG_ANALYSIS
	for(int i=1; i<=_totalUsersEvaluated; i++){
		_percentualAnalysis[i]->info();	
	}
#endif
}