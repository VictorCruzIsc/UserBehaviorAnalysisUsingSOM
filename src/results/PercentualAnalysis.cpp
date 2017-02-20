#include "../../include/results/PercentualAnalysis.h"

PercentualAnalysis::PercentualAnalysis(int user, double correctPercentage, double incorrectPercentage):
	_user(user), _correctPercentage(correctPercentage), _incorrectPercentage(incorrectPercentage),
	_totalPecentage(_correctPercentage + _incorrectPercentage){}

void PercentualAnalysis::info(){
	cout << _user << " " << _correctPercentage << " " << _incorrectPercentage << " " << _totalPecentage << endl;
}
