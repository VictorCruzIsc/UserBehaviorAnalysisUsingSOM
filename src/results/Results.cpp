#include "../../include/results/ResultsClasses.h"

SamplesResult* Results::evaluateUsers(int samples, int sigma, int totalUsersEvaluated,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection,
	vector<int> &userIds){
	for(int i=0; i<totalUsersEvaluated; i++){
		som->evaluateIndependentDataChuckDataSet(evaluateDataChunckSetCollection[i],
			sigma, samples, i, i, i, userIds[i]);
	}

	int totalNeuronsEvaluated = (samples * totalUsersEvaluated);

	SamplesResult* samplesResult = som->getMatrixStadistics(samples, totalUsersEvaluated, sigma, userIds);

	return samplesResult;
}

void Results::getResults(int initialSamples, int finalSamples,
	int increment, int sigma, int requestedExperiments, int totalUsersEvaluated,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection,
	vector<int> &userIds){

	vector<Experiment *> experiments;
	vector<UserGraph *> graphics;

	int currentUser = 0;

	cout << "Total experiments required: " << requestedExperiments << endl;

	for(int i=0; i<requestedExperiments; i++){
		cout << "Start experiment " << (i + 1) << endl;

		Experiment *currentExperiment = new Experiment(i, userIds);

		for(int samples = initialSamples;
			samples < finalSamples;
			samples += increment){

			cout << "Samples: " << samples << endl;

			currentExperiment->addResult(
				Results::evaluateUsers(samples, sigma, totalUsersEvaluated, som,
					evaluateDataChunckSetCollection, userIds));
			som->resetMatrixStadistics();
		}

		currentExperiment->processExperimentAverages(totalUsersEvaluated);
		currentExperiment->experimentInfo();
		experiments.push_back(currentExperiment);
		cout << "" << endl;
	}

	// Get graphics elements
	for(int i=0; i<totalUsersEvaluated; i++){
		currentUser = userIds[i];
		vector<ExperimentAverageAnalysis *> averages;
		for(int j=0; j<requestedExperiments; j++){
			averages.push_back(experiments[j]->getExperimentAverageAnalysis()[currentUser]);
		}
		graphics.push_back(new UserGraph(currentUser, averages));
	}

	// Print graphics results

	cout << "\n\n" << endl;
	cout << "Graphics" << endl;
	int totalGraphics = graphics.size();
	for(int i=0; i<totalGraphics; i++){
		graphics[i]->info();
	}

	getBarGraphs(graphics, requestedExperiments, userIds);
}

void Results::getBarGraphs(vector<UserGraph *> &graphics, int totalExperiments, vector<int> &userIds){
	// Import results to pyhton
	string baseCommand = "";

#ifdef MAC
	baseCommand = "python ~/Documents/git/UserBehaviorAnalysisUsingSOM/python/GraphicsMAC.py";
#else
	baseCommand = "python ~/Desktop/som/userBehaviorAnalysisUsingSom/python/Graphics.py";
#endif

	// X-Axis labels
	string xAxis = "";
	for(int i=1; i<totalExperiments; i++){
		xAxis += "E" + to_string(i) + ',';
	}
	xAxis += "E" + to_string(totalExperiments);

	// Correct and incorrect values
	int totalNeededGraphics = graphics.size();
	string correct = "";
	string incorrect = "";
	string callingCommand = "";
	string name = "";
	int totalAverages = 0;

	for(int i=0; i<totalNeededGraphics; i++){
		UserGraph *currentUserGraph = graphics[i];
		vector<ExperimentAverageAnalysis *> averages = currentUserGraph->getUserResults();
		totalAverages = averages.size();
		for(int j=0; j<(totalAverages - 1); j++){
			correct += to_string(averages[j]->getCorrectPercentage()) + ',';
			incorrect += to_string(averages[j]->getIncorrectPercentage()) + ',';
		}
		correct += to_string(averages[totalAverages - 1]->getCorrectPercentage());
		incorrect += to_string(averages[totalAverages - 1]->getIncorrectPercentage());

		name += "User"+to_string(userIds[i]);

		callingCommand += baseCommand;
		callingCommand += " " + name;
		callingCommand += " " + xAxis;
		callingCommand += " " + correct;
		callingCommand += " " + incorrect;

		cout << "Calling command: " << callingCommand << endl;

		// Call python script
		system(callingCommand.c_str());

		callingCommand = "";
		correct = "";
		incorrect = "";
		name = "";
		totalAverages = 0;
	}
}
