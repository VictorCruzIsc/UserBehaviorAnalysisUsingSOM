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

	SamplesResult* samplesResult = som->getMatrixStadistics(samples, totalUsersEvaluated, sigma);

	return samplesResult;
}

void Results::getResults(int initialSamples, int finalSamples,
	int increment, int sigma, int requestedExperiments, int totalUsersEvaluated,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection){
	vector<Experiment *> experiments;
	vector<UserGraph *> graphics;
	cout << "Total experiments required: " << requestedExperiments << endl;
	for(int i=0; i<requestedExperiments; i++){
		cout << "Start experiment " << (i + 1) << endl;
		Experiment *currentExperiment = new Experiment(i);
		for(int samples = initialSamples;
			samples < finalSamples;
			samples += increment){
			cout << "Samples: " << samples << endl;
			currentExperiment->addResult(Results::evaluateUsers(samples, sigma, totalUsersEvaluated, som,
				evaluateDataChunckSetCollection));
			som->resetMatrixStadistics();
		}
		currentExperiment->processExperimentAverages(totalUsersEvaluated);
		currentExperiment->experimentInfo();
		experiments.push_back(currentExperiment);
		cout << "" << endl;
	}

	// Get graphics elements

	for(int i=1; i<=totalUsersEvaluated; i++){
		vector<ExperimentAverageAnalysis *> averages;
		for(int j=0; j<requestedExperiments; j++){
			averages.push_back(experiments[j]->getExperimentAverageAnalysis()[i]);
		}
		graphics.push_back(new UserGraph(i, averages));
	}

	// Print graphics results

	cout << "\n\n" << endl;
	cout << "Graphics" << endl;
	int totalGraphics = graphics.size();
	for(int i=0; i<totalGraphics; i++){
		graphics[i]->info();
	}

	// Import results to pyhton
	string command = "python ~/Desktop/som/userBehaviorAnalysisUsingSom/python/Graphics.py";
	system(command.c_str());
}