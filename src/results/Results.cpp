#include "../../include/results/Results.h"

SamplesResult* Results::evaluateUsers(int samples, int sigma, int totalUsersEvaluated,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection,
	vector<int> &userIds){
	for(int i=0; i<totalUsersEvaluated; i++){
		som->evaluateIndependentDataChuckDataSet(evaluateDataChunckSetCollection[i],
			sigma, samples, i, i, i, userIds[i]);
	}

	SamplesResult* samplesResult = som->getMatrixStadistics(samples, totalUsersEvaluated, sigma, userIds);

	return samplesResult;
}

/*
* Obtains the results based on the received parameters
* The results are obtained through experiments, which allow to evaluate incremental number
* of samples from initialSamples to finalSamples.
* Each experiment has a complete range of samples evaluation.
* userIds are received to identify the users
* requestedExperiments number is needed
*
* Each experiment is run for the needed user with a specific number if samples
* Example:
* totalUsersEvaluated: 4
* userIds: 4, 5, 6, 7
* requestedExperiments: 3
* initialSamples: 200
* finalSamples:2500
* increment 900
*
* Experiment 1
*	SampleResult[0] evaluates users(4,5,6,7) with 200 samples
*	SampleResult[1] evaluates users(4,5,6,7) with 1100 samples
*	SampleResult[2] evaluates users(4,5,6,7) with 2000 samples
* Experiment 2
*	SampleResult[0] evaluates users(4,5,6,7) with 200 samples
*	SampleResult[1] evaluates users(4,5,6,7) with 1100 samples
*	SampleResult[2] evaluates users(4,5,6,7) with 2000 samples
* Experiment 3
*	SampleResult[0] evaluates users(4,5,6,7) with 200 samples
*	SampleResult[1] evaluates users(4,5,6,7) with 1100 samples
*	SampleResult[2] evaluates users(4,5,6,7) with 2000 samples
*
* Due each experiment has in this case 3 different results for
* the user 4 evaluated with 200 samples, 3 different results with
* 1100 samples and 3 different results with 2000 this results are
* processed to get a complete analysis and report the status of the
* user in a graph that represents all the experiments
*/
void Results::getResults(int initialSamples, int finalSamples, int increment,
	int sigma, int requestedExperiments, int totalUsersEvaluated,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection,
	vector<int> &userIds){

	// Execute the total required experiment, but with different amount of samples
	for(int samples=initialSamples; samples<finalSamples; samples+=increment){
		cout << "Running experiments with " << samples << " neurons" << endl;

		for(int experiment=0; experiment<requestedExperiments; experiment++){
			cout << "Executing " << (experiment + 1) << "/";
			cout << requestedExperiments << endl;

			SamplesResult* samplesResult = evaluateUsers(samples, sigma,
				totalUsersEvaluated, som, evaluateDataChunckSetCollection, userIds);
		}
	}

	vector<Experiment *> experiments;
	vector<UserGraph *> graphics;
	bool error = false;

	int currentUser = 0;

	cout << "Total experiments required: " << requestedExperiments << endl;

	for(int i=0; i<requestedExperiments; i++){
		cout << "Start experiment " << (i + 1) << endl;

		Experiment *currentExperiment = new Experiment(i, userIds);

		for(int samples = initialSamples;
			samples < finalSamples;
			samples += increment){

			cout << "Samples: " << samples << endl;
			SamplesResult* samplesResult = evaluateUsers(samples, sigma,
				totalUsersEvaluated, som, evaluateDataChunckSetCollection, userIds);

			if(!samplesResult){
				som->resetMatrixStadistics();
				error = true;
				cout << "Error getting samples result for samples: " << samples << endl;
				break;
			}

			currentExperiment->addResult(samplesResult);
			som->resetMatrixStadistics();
		}

		if(error){
			cout << "Error was detected, no getting experimentAverages" << endl;
			break;
		}

		currentExperiment->processExperimentAverages(totalUsersEvaluated);
		currentExperiment->experimentInfo();
		experiments.push_back(currentExperiment);
		cout << "" << endl;
	}

	if(error){
		cout << "Error was detected, no processing graphs results, so no graphics either" << endl;
		return;
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

	// Execute python script
	getBarGraphs(graphics, requestedExperiments, userIds);
}

ExperimentGeneric * Results::processExperiment(int experimentId,
	int initialSamples, int finalSamples, int increment, vector<int> &userIds,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection){

	vector<EvaluationGeneric*> experimentEvaluations;
	vector<int> experimentEvaluatedVectors;
	int totalUsersEvaluated = userIds.size();

	for(int samples=initialSamples; samples<finalSamples; samples+=increment){
		experimentEvaluatedVectors.push_back(samples);

		// Evaluation of dataset by users
		for(int i=0; i<totalUsersEvaluated; i++){
			som->evaluateIndependentDataChuckDataSet(
				evaluateDataChunckSetCollection[i],
				0.05, samples, i, i, i, userIds[i]);
		}

		// Get positive matches by user to create evaluations
		for(int i=0; i<totalUsersEvaluated; i++){
			int currentUser = userIds[i];
			int positiveByUser = som->getPositiveMatchesByUser(currentUser);
			int negativeByUser = samples - positiveByUser;
			EvaluationGeneric *evaluation = new EvaluationGeneric(currentUser,
				samples, positiveByUser, negativeByUser);
			experimentEvaluations.push_back(evaluation);
		}

		som->resetMatrixStadistics();
	}

	ExperimentGeneric *experiment = new ExperimentGeneric(experimentId, userIds,
		experimentEvaluations, experimentEvaluatedVectors);

	return experiment;
}

vector<ExperimentGeneric *> Results::processExperimentResults(int initialSamples,
	int finalSamples, int increment, int requestedExperiments,
	SelfOrganizingMaps *som,
	vector<vector<DataChunck *> > &evaluateDataChunckSetCollection,
	vector<int> &userIds){
	vector<ExperimentGeneric *> experiments;
	for(int i=0; i<requestedExperiments; i++){
		int experimentId = (i + 1);
		cout << "Running experiment: " << experimentId << endl;
		experiments.push_back(processExperiment(experimentId, initialSamples,
			finalSamples, increment, userIds, som, evaluateDataChunckSetCollection));
		cout << "Finish running experiment: " << experimentId << endl;
	}

	if(experiments.size() == requestedExperiments){
		cout << "Processing experiment results finishes correctly." << endl;
	}else{
		cout << "ERROR: Processing experiment results." << endl;
	}

	return experiments;
}

/*
* Executes a python script that shows the information, correct vs incorrect percentages among all the
* experiments
*/
void Results::getBarGraphs(vector<UserGraph *> &graphics, int totalExperiments, vector<int> &userIds){
	int totalNeededGraphics = graphics.size();
	int totalAverages = 0;
	string baseCommand = "";
	string xAxis = "";
	string correct = "";
	string incorrect = "";
	string callingCommand = "";
	string name = "";

#ifdef MAC
	baseCommand = "python ~/Documents/git/UserBehaviorAnalysisUsingSOM/python/GraphicsMAC.py";
#else
	baseCommand = "python ~/Desktop/som/userBehaviorAnalysisUsingSom/python/Graphics.py";
#endif

	// Creating X-Axis labels
	for(int i=1; i<totalExperiments; i++){
		xAxis += "E" + to_string(i) + ',';
	}
	xAxis += "E" + to_string(totalExperiments);

	// Get correct and incorrect percentages for each user
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

		name = "User"+to_string(userIds[i]);

		callingCommand += baseCommand;
		callingCommand += " " + name;
		callingCommand += " " + xAxis;
		callingCommand += " " + correct;
		callingCommand += " " + incorrect;

		cout << "Calling command: " << callingCommand << endl;

		// Call python script
		system(callingCommand.c_str());

		// Reset values for the next user
		callingCommand = "";
		correct = "";
		incorrect = "";
		name = "";
		totalAverages = 0;
	}
}
