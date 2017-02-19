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
	_correctResult(correctResult){}
