#include "../../include/results/ExperimentAverageAnalysis.h"

ExperimentAverageAnalysis::ExperimentAverageAnalysis(int user, double correctPercentage, double incorrectPercentage):
	_user(user), _correctPercentage(correctPercentage), _incorrectPercentage(incorrectPercentage){}

void ExperimentAverageAnalysis::info(){
	cout << _user << " " << _correctPercentage << " " << _incorrectPercentage << endl;
}