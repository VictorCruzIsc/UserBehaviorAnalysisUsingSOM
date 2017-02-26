#ifndef EXPERIMENT_AVERAGE_ANALYSYS_H
#define EXPERIMENT_AVERAGE_ANALYSYS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class ExperimentAverageAnalysis{
public:
	ExperimentAverageAnalysis(int user, double correctPercentage, double incorrectPercentage);
	int getUser(){ return _user; }
	double getCorrectPercentage(){ return _correctPercentage; }
	double getIncorrectPercentage(){ return _incorrectPercentage; }
	void info();
private:
	int _user;
	double _correctPercentage;
	double _incorrectPercentage;
};

#endif