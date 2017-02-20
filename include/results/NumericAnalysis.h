#ifndef NUMERIC_ANALYSYS_H
#define NUMERIC_ANALYSYS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class NumericAnalysis{
public:
	NumericAnalysis(int user, int correctSamples, int incorrectSamples);
	int getUser(){ return _user; }
	int getCorrectSamples(){ return _correctSamples; }
	int getIncorrectSamples(){ return _incorrectSamples; }
	int getUniqueEvaluatedSamples(){ return _uniqueEvaluatedSamples; }
	void info();
private:
	int _user;
	int _correctSamples;
	int _incorrectSamples;
	int _uniqueEvaluatedSamples;
};

#endif