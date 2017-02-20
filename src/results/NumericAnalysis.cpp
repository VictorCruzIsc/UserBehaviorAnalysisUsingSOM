#include "../../include/results/NumericAnalysis.h"

NumericAnalysis::NumericAnalysis(int user, int correctSamples, int incorrectSamples):
	_user(user), _correctSamples(correctSamples), _incorrectSamples(incorrectSamples),
	_uniqueEvaluatedSamples(_correctSamples + _incorrectSamples){}

void NumericAnalysis::info(){
	cout << _user << " " << _correctSamples << " " << _incorrectSamples << " " << _uniqueEvaluatedSamples << endl;
}