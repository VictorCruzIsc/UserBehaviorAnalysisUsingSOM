#ifndef SAMPLES_RESULT_H
#define SAMPLES_RESULT_H

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <map>

#include "../functionality/StadisticsResults.h"
#include "NumericAnalysis.h"
#include "PercentualAnalysis.h"

#define DEBUG_ANALYSIS

using namespace std;

class SamplesResult{
	public:
		SamplesResult(int samples, int totalUsersEvaluated, int sigma,
			int totalPositiveCollitions, int totalNegativeCollitions,
			int correct, int incorrect, map<int, StadisticsResults *> errorStadisticsResults,
			map<int, StadisticsResults *> correctStadisticsResults, bool correctStadisticResult);
		int getSamples(){ return _samples; }
		int getTotalUsersEvaluated(){ return _totalUsersEvaluated; }
		int getExpectedNeuronsEvaluated(){ return _expectedNeuronsEvaluated; };
		int getSigma(){ return _sigma; };
		int getTotalPositiveCollitions(){ return _totalPositiveCollitions; }
		int getTotalNegativeCollitions(){ return _totalNegativeCollitions; }
		int getCorrect(){ return _correct; }
		int getIncorrect(){ return _incorrect; }
		map<int, StadisticsResults *> getErrorStadisticsResults(){
			_errorStadisticsResults;
		}
		map<int, StadisticsResults *> getCorrectStadisticsResults(){
			_correctStadisticsResults;
		}
		bool isCorrect(){ return _correctResult; }
		map<int, NumericAnalysis *> getNumericAnalysis(){ return _numericAnalysis; }
		map<int, PercentualAnalysis *> getPercentualAnalysis(){ return _percentualAnalysis; }

	private:
		void processNumericAnalysis();
		void processPercentualAnalysis();

		int _samples;
		int _totalUsersEvaluated;
		int _expectedNeuronsEvaluated;
		int _sigma;
		int _totalPositiveCollitions;
		int _totalNegativeCollitions;
		int _correct;
		int _incorrect;
		bool _correctResult;
		map<int, StadisticsResults *> _errorStadisticsResults;
		map<int, StadisticsResults *> _correctStadisticsResults;
		map<int, NumericAnalysis *> _numericAnalysis;
		map<int, PercentualAnalysis *> _percentualAnalysis;
		bool _correctNumericalAnalysis;
		bool _correctPercentualAnalysis;
};
#endif