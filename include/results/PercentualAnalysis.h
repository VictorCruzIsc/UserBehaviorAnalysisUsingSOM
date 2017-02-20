#ifndef PERCENTUAL_ANALYSYS_H
#define PERCENTUAL_ANALYSYS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class PercentualAnalysis{
public:
	PercentualAnalysis(int user, double correctPercentage, double incorrectPercentage);
	int getUser(){ return _user; }
	double getCorrectSamples(){ return _correctPercentage; }
	double getIncorrectSamples(){ return _incorrectPercentage; }
	double getTotalPercentage(){ return _totalPecentage; }
	void info();
private:
	int _user;
	double _correctPercentage;
	double _incorrectPercentage;
	double _totalPecentage;
};

#endif