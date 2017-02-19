#include "../../include/results/ResultsClasses.h"

SamplesResult* Results::evaluateUsers(int samples, int sigma, int totalUsersEvaluated,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection){
	for(int i=0; i<totalUsersEvaluated; i++){
		switch(i){
			case 0:
				// dataSet, sigma, # samples, r, g, b, user
				som->evaluateIndependentDataChuckDataSet(evaluateDataChunckSetCollection[i],
					sigma, samples, 255, 0, 0, (i + 1));
				break;
			case 1:
				// dataSet, sigma, # samples, r, g, b, user
				som->evaluateIndependentDataChuckDataSet(evaluateDataChunckSetCollection[i],
					sigma, samples, 0, 255, 0, (i + 1));
				break;
			case 2:
				// dataSet, sigma, # samples, r, g, b, user
				som->evaluateIndependentDataChuckDataSet(evaluateDataChunckSetCollection[i],
					sigma, samples, 0, 0, 255, (i + 1));
				break;
		}
	}

	int totalNeuronsEvaluated = (samples * totalUsersEvaluated);

	SamplesResult* samplesResult = som->getMatrixStadistics(totalNeuronsEvaluated);
	samplesResult->setSamples(samples);
	samplesResult->setTotalUsersEvaluated(totalUsersEvaluated);
	samplesResult->setExpectedNeuronsEvaluated(samples * totalUsersEvaluated);
	samplesResult->setSigma(sigma);

	return samplesResult;
}

vector<Experiment*> Results::getResults(int initialSamples, int finalSamples,
	int increment, int sigma, int requestedExperiments, int totalUsersEvaluated,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection){
	vector<Experiment *> experiments;
	experiments.resize(requestedExperiments);

	for(int i=0; i<requestedExperiments; i++){
		Experiment *currentExperiment = new Experiment(i);
		for(int samples = initialSamples;
			samples < finalSamples;
			samples += increment){
			currentExperiment->addResult(Results::evaluateUsers(samples, sigma, totalUsersEvaluated, som,
				evaluateDataChunckSetCollection));
			som->resetMatrixStadistics();
		}
		currentExperiment->experimentInfo();
		experiments.push_back(currentExperiment);
	}

	return experiments;
}