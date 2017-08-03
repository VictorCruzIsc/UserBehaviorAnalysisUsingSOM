#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "include/functionality/Classes.h"
#include "include/results/Results.h"

// SOM constants
#define TOTALWEIGHTS 			3
#define MAXEPOCHS 				10
#define NORMALSIZE 				75
#define INITIALLEARNINGRATE 	0.1
#define SIGMA 					1

// DataSet constants
#define CHUNCKTIMESIZE 			5	// Given in minutes
#define CHUNCKTIMEINTERVAL 		10	// Given in seconds

// Results constants

// Developing constants
//#define DEBUG

using namespace std;

// SOM variables
int								_trainDataSetSize;

int 							_sigma;
int 							_width;
int 							_height;
int 							_randInput;
bool 							_training;
bool							_isEvaluationDataSetInitialized;
SelfOrganizingMaps				*_som;

// Results variables
int 							_totalUsersEvaluated;
int 							_maximumSamples;
int 							_initialSamples;
int 							_totalExperiments;
int 							_samplesIncrement;
int 							_evMatrix;

vector<DataChunck *>			_trainDataChunckSet;
vector<vector<DataChunck *> >	_evaluateDataChunckSetCollection;
vector<int> 					_userIds;
vector<ExperimentGeneric *>		_experiments;

string 							_identifier;

// ===================== Local Method Headers =====================
void resultsObtention(){
	_experiments = Results::processExperimentResults(_initialSamples,
	_maximumSamples, _samplesIncrement, _totalExperiments, _som,
	_evaluateDataChunckSetCollection, _userIds);

	if(_experiments.size() != _totalExperiments){
		cout << "ERROR: Results do not match" << endl;
	}
}

void graphicsObtention(){
// When experiments are obtained the graphs can be processed
			if(_experiments.size() != _totalExperiments){
				cout << "ERROR: No available data" << endl;
				return;
			}

			// Represent all the samples that the experiments have
			vector<int> samples = _experiments[0]->getEvaluatedVectors();
			int totalSamples = samples.size();

			for(int userId=0; userId<_totalUsersEvaluated; userId++){
				int currentUserId = _userIds[userId];

				vector<double> avgPositives;
				vector<double> avgNegatives;
				vector<double> avgIdles;

				for(int z=0; z<totalSamples; z++){
					int currentSample = samples[z];

					vector<double> vectorsPositives;
					vector<double> vectorsNegatives;
					vector<double> vectorsIdles;

					int positiveTotalByVectors = 0;
					int negativetotalByVectors = 0;
					int idleTotalByVectors = 0;
					for(int experiment=0; experiment<_totalExperiments; experiment++){
						vector<int> results =
							_experiments[experiment]->getMatrixResults(
								currentUserId, currentSample);

						// Something is needed to recdord this results for the graphic
						int positives = results[1];
						int negatives = results[2];
						int idles = results[3];

						vectorsPositives.push_back((positives/(double)currentSample) * 100);
						vectorsNegatives.push_back((negatives/(double)currentSample) * 100);
						vectorsIdles.push_back((idles/(double)currentSample) * 100);

						// Track for obtaining averages
						positiveTotalByVectors += positives;
						negativetotalByVectors += negatives;
						idleTotalByVectors += idles;
					}

					if(vectorsPositives.size() != _totalExperiments || vectorsNegatives.size() != _totalExperiments || vectorsIdles.size() != _totalExperiments){
						cout << "ERROR: Data obtentention for user - evaluated vectors | SAMPLES: " << currentSample << " - " << "USER:" << currentUserId;
						cout << " data will not be precessed, so no individual graphs will be obtained either" << endl;
						continue;
					}

					double avgPositiveByVectors = ((positiveTotalByVectors/(double)_totalExperiments)/currentSample) * 100;
					double avgNegativeByVectors = ((negativetotalByVectors/(double)_totalExperiments)/currentSample) * 100;
					double avgIdleByVectors = ((idleTotalByVectors/(double)_totalExperiments)/currentSample) * 100;

					avgPositives.push_back(avgPositiveByVectors);
					avgNegatives.push_back(avgNegativeByVectors);
					avgIdles.push_back(avgIdleByVectors);
				}

				if(avgPositives.size() != totalSamples || avgNegatives.size() != totalSamples || avgIdles.size() != totalSamples){
					cout << "ERROR: Data AVG obtentions USER:" << currentUserId;
					cout << " AVG will not be obtained" << endl;
				}

				vector<string> xAxisLabelsAVG;

				Results::getBarGraphsOnlyCommand(avgPositives, avgNegatives, avgIdles, xAxisLabelsAVG, to_string(_evMatrix) + "_" + _identifier);

				// Plot only the average of the average results
				{
					int totalAVGPositives = avgPositives.size();
					int totalAVGNegatives = avgNegatives.size();
					int totalAVGIdle = avgIdles.size();

					double secAVGPos = 0.0;
					double secAVGNeg = 0.0;
					double secAVGIdle = 0.0;

					for(int i=0; i<totalAVGPositives; i++){
						secAVGPos += avgPositives[i];
					}

					secAVGPos = secAVGPos/totalAVGPositives;

					for(int i=0; i<totalAVGNegatives; i++){
						secAVGNeg += avgNegatives[i];
					}
					secAVGNeg = secAVGNeg/totalAVGNegatives;

					for(int i=0; i<totalAVGIdle; i++){
						secAVGIdle += avgIdles[i];
					}
					secAVGIdle = secAVGIdle/totalAVGIdle;

					vector<double> pos_;
					pos_.push_back(secAVGPos);

					vector<double> neg_;
					neg_.push_back(secAVGNeg);

					vector<double>idle_;
					idle_.push_back(secAVGIdle);

					vector<string> _axis;
					_axis.push_back("Samples_results_avgs");

					Results::getBarGraphs(pos_, neg_, idle_, _axis, _identifier +
						"_User_" + to_string(currentUserId) + "_samples_avgs" + "_" + to_string(_evMatrix));
				}
			}
}

bool createEvaluationDataSets(){
	for(int i=0; i<_totalUsersEvaluated; i++){
		vector<DataChunck *> dataChunckSet =
			DataSet::createDataSetDataChunckFormatFromFile(_userIds[i], Utils::EVALUATE);
		int totalElements = dataChunckSet.size();
		if(totalElements > 0){
			_evaluateDataChunckSetCollection.push_back(dataChunckSet);
		}else{
			cout << "Not possible to add chucnkset user " << _userIds[i] << endl;
		}
	}
	return true;
}

int main(int argc, char **argv){
	vector<char *> fileNames;
	vector<int> userIds;
	_evMatrix = atoi(argv[1]);
	_initialSamples = atoi(argv[2]);
	_maximumSamples = atoi(argv[3]);
	_samplesIncrement = atoi(argv[4]);
	_totalExperiments = atoi(argv[5]);

	_sigma = SIGMA;

	_identifier = "";

	for(int files=6; files<argc; files+=2){
		fileNames.push_back(argv[files]);
		_userIds.push_back(atoi(argv[(files+1)]));
	}

	_totalUsersEvaluated = fileNames.size();

	for(int y=0; y<_totalUsersEvaluated; y++){
		_identifier += to_string(_userIds[y]);
	}

	int matrixComposition = sqrt(_totalUsersEvaluated);

	_isEvaluationDataSetInitialized = createEvaluationDataSets();
	
	if(!_isEvaluationDataSetInitialized){
		cout << "It was not possible to create the evaluation dataset" << endl;
		return 1;
	}

	_som = Utils::importSOMFromFiles(fileNames, matrixComposition, _totalUsersEvaluated);

	cout << "OBTAINING RESULTS" << endl;
	resultsObtention();

	cout << "\nOBTAINING GRAPHICS" << endl;
	graphicsObtention();

	return 0;
}