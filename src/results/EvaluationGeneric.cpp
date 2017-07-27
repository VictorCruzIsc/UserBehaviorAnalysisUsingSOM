#include "../../include/results/EvaluationGeneric.h"

double EvaluationGeneric::getPositivePercentage(){
	double percentage = 0.0;
	if(_correct){
		percentage = _positivePercentage;
	}
	return percentage;
}

double EvaluationGeneric::getNegativePercentage(){
	double percentage = 0.0;
	if(_correct){
		percentage = _negativePercentage;
	}
	return percentage;
}

double EvaluationGeneric::getIdlePercentage(){
	double percentage = 0.0;
	if(_correct){
		percentage = _idlePercentage;
	}
	return percentage;
}

bool EvaluationGeneric::verifyEvaluationIsCorrect(){
	return (_totalEvaluatedVectors ==
		(_totalPositive + _totalNegative + _totalIdle));
}

double EvaluationGeneric::processPositivePercentage(){
	return (double) _totalPositive / (double) _totalEvaluatedVectors;
}

double EvaluationGeneric::processNegativePercentage(){
	return (double) _totalNegative / (double) _totalEvaluatedVectors;
}

double EvaluationGeneric::processIdlePercentage(){
	return (double) _totalIdle / (double) _totalEvaluatedVectors;
}

void EvaluationGeneric::info(){
	cout << "User: " << _relatedUser;
	cout << ", evaluated users: " << _totalEvaluatedVectors;
	cout << ", total postive " << _totalPositive;
	cout << ", total negative " << _totalNegative;
	cout << ", total idle " << _totalIdle;
	cout << ", total positive percentage" << _positivePercentage;
	cout << ", total negative percentage" << _negativePercentage;
	cout << ", total idle percentage" << _idlePercentage;
	cout << ", and is " << (_correct ? "correct" : "not correct") << endl;
}