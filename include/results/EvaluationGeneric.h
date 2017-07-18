#ifndef EVALUATION_H
#define EVALUATION_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

/*
* This class saves the data that was obtained for each matrix evaluation
* e.g.
* 200 vectors were asked to evalute from the evaluation dataset.
* The selected user to evaluate is 1.
* After evaluation and matrix reading there is found that 112 vectors matched
* correctly and 88 match incorrectly.
* This is is an evaluation of 200 vectors for the user 1, and results are
* saved in this object.
*/

class EvaluationGeneric{
public:
	EvaluationGeneric(int relatedUSer, int totalEvaluatedVectors, int totalPositive,
		int totalNegative):_relatedUser(relatedUSer),
	_totalEvaluatedVectors(totalEvaluatedVectors),
	_totalPositive(totalPositive), _totalNegative(totalNegative){
		_correct = verifyEvaluationIsCorrect();

		if(_correct){
			_positivePercentage = processPositivePercentage();
			_negativePercentage = processNegativePercentage();
		}else{
			cout << "ERROR: User -> " << _relatedUser;
			cout << " totalPositive and totalNegative do not match. ";
			cout << (_totalPositive + totalNegative) << " != ";
			cout << _totalEvaluatedVectors << endl;
		}
	};

	int getRelatedUser(){ return _relatedUser; }
	int getTotalEvaluatedVectors(){ return _totalEvaluatedVectors; }
	int getTotalPositive(){ return _totalPositive; }
	int getTotalNegative(){ return _totalNegative; }
	double isEvaluationCorrect(){ return _correct; }

	double getPositivePercentage();
	double getNegativePercentage();
	void info();

private:
	bool verifyEvaluationIsCorrect();
	double processPositivePercentage();
	double processNegativePercentage();

	int _relatedUser;
	int _totalEvaluatedVectors;
	int _totalPositive;
	int _totalNegative;
	double _positivePercentage;
	double _negativePercentage;
	bool _correct;
};
#endif