#include "../../include/results/ResultsClasses.h"

SamplesResult* Results::evaluateUsers(int samples, int sigma, int totalUsersEvaluated,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection){
	for(int i=0; i<totalUsersEvaluated; i++){
		cout << "Running evaluation for user " << i << " ..." << endl;
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

	cout << "Total users evaluated: " << totalUsersEvaluated << endl;
	cout << "Total samples evaluated per user: " << samples << endl;
	cout << "Expected neurons evaluated: " << totalNeuronsEvaluated << endl;
	cout << "Used sigma: " << sigma << endl;

	SamplesResult* samplesResult = som->getMatrixStadistics(totalNeuronsEvaluated);
	samplesResult->setSamples(samples);
	samplesResult->setTotalUsersEvaluated(totalUsersEvaluated);
	samplesResult->setExpectedNeuronsEvaluated(samples * totalUsersEvaluated);
	samplesResult->setSigma(sigma);

	return samplesResult;
}

void Results::getResults(int initialSamples, int finalSamples,
	int increment, int sigma, int experiments, int totalUsersEvaluated,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection){
	cout << "=== Starting results obtention ===" << endl;
	cout << "\n\n" << endl;
	for(int i=0; i<experiments; i++){
		cout << "--------- Experiment (" << (i + 1) << "/" << experiments << ") ---------" << endl;
		for(int samples = initialSamples;
			samples < finalSamples;
			samples += increment){
			cout << "Samples: " << samples << endl;
			Results::evaluateUsers(samples, sigma, totalUsersEvaluated, som,
				evaluateDataChunckSetCollection);
			som->resetMatrixStadistics();
			cout << "\n" << endl;
		}
		cout << "\n\n" << endl;
	}
	cout << "=== Finish results obtention ===" << endl;
}