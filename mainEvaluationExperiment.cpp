#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "include/functionality/Classes.h"
#include "include/results/Results.h"

#define MAC

#ifdef MAC
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <OpenGL/glext.h>
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

// OpenGL constants
#define BASEWIDTH				400
#define BASEHEIGHT 				400
#define BASEOPENGLFOVY			45.0
#define WINDOWTITLE 			"Self Organizing Maps"

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

// OpenGL variables
double							_openGLFovy;

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

vector<DataChunck *>			_trainDataChunckSet;
vector<vector<DataChunck *> >	_evaluateDataChunckSetCollection;
vector<int> 					_userIds;
vector<ExperimentGeneric *>		_experiments;

// ===================== Local Method Headers =====================
// OpenGl methods
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);
void init();

void display(){
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Compute eye position
	glLoadIdentity();

	// TODO: check what does this function do
	gluLookAt(50,50,200, 50,50,0, 0,1,0);

	// Draw the current map
	_som->displayUsingNeuronColor();

	// Draw to screen
	glutSwapBuffers();
}

void reshape(int width, int height){
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

void keyboard(unsigned char key, int mouseX, int mouseY){ 
	switch (key){
		case 'r':
			cout << "Results obtention..." << endl;
			_experiments = Results::processExperimentResults(_initialSamples,
				_maximumSamples, _samplesIncrement, _totalExperiments, _som,
					_evaluateDataChunckSetCollection, _userIds);

			for(int i=0; i<_experiments.size(); i++){
				_experiments[i]->info();
			}


			if(_experiments.size() == _totalExperiments){
				cout << "Results obtention finished" << endl;
			}else{
				cout << "ERROR: Results do not match" << endl;
			}
			
			glutPostRedisplay();
			break;
		case 'g':
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
				for(int z=0; z<totalSamples; z++){
					int currentSample = samples[z];

					vector<double> vectorsPositives;
					vector<double> vectorsNegatives;

					int positiveTotalByVectors = 0;
					int negativetotalByVectors = 0;
					for(int experiment=0; experiment<_totalExperiments; experiment++){
						vector<int> results =
							_experiments[experiment]->getMatrixResults(
								currentUserId, currentSample);

						// Something is needed to recdord this results for the graphic
						int positives = results[1];
						int negatives = results[2];

						vectorsPositives.push_back((positives/(double)currentSample) * 100);
						vectorsNegatives.push_back((negatives/(double)currentSample) * 100);

						// Track for obtaining averages
						positiveTotalByVectors += positives;
						negativetotalByVectors += negatives;
					}

					if(vectorsPositives.size() != _totalExperiments || vectorsNegatives.size() != _totalExperiments){
						cout << "ERROR: Data obtentention for user - evaluated vectors | SAMPLES: " << currentSample << " - " << "USER:" << currentUserId;
						cout << " data will not be precessed, so no individual graphs will be obtained either" << endl;
						continue;
					}

					// Graph obtention is here
					/*
					for(int i=0; i<_totalExperiments; i++){
						cout << "[" << vectorsPositives[i] << "," << vectorsNegatives[i] << "]" << endl;
					}
					*/
					vector<string> xAxisLabels;
					for(int i=0; i<_totalExperiments; i++){
						string label = "E" + to_string(i);
						xAxisLabels.push_back(label);
					}

					Results::getBarGraphs(vectorsPositives, vectorsNegatives, xAxisLabels, "User" + to_string(currentUserId) + "-" + to_string(currentSample));

					double avgPositiveByVectors = ((positiveTotalByVectors/(double)_totalExperiments)/currentSample) * 100;
					double avgNegativeByVectors = ((negativetotalByVectors/(double)_totalExperiments)/currentSample) * 100;

					avgPositives.push_back(avgPositiveByVectors);
					avgNegatives.push_back(avgNegativeByVectors);
				}

				if(avgPositives.size() != totalSamples || avgNegatives.size() != totalSamples){
					cout << "ERROR: Data AVG obtentions USER:" << currentUserId;
					cout << " AVG will not be obtained" << endl;
				}

				// Process user average graph
				/*
				for(int i=0; i<totalSamples; i++){
					double positiveAVG = avgPositives[i];
					double negativeAVG = avgNegatives[i];
					double total = positiveAVG + negativeAVG;
					cout << "[" << positiveAVG << "," << negativeAVG << "] = " << total << endl;
				}
				*/

				vector<string> xAxisLabelsAVG;
				for(int i=0; i<totalSamples; i++){
					string label = "Sample" + to_string(samples[i]);
					xAxisLabelsAVG.push_back(label);
				}

				Results::getBarGraphs(avgPositives, avgNegatives, xAxisLabelsAVG, "User" + to_string(currentUserId) + "AVGs");
			}
			break;
	}
}

void idle(void){
	if(_training){
		_randInput = rand() % _trainDataSetSize;
		_som->trainSegmentedFunctions(_trainDataChunckSet[_randInput]->dataChunckToVector());
		glutPostRedisplay();
	}
}

void init(){
	// Initialize viewing system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(_openGLFovy, 1.0, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	// Initialize background color to black
	glClearColor(0.0,0.0,0.0,0.0);

	// Enable depth buffering
	glEnable(GL_DEPTH_TEST);
}

bool createEvaluationDataSets(){
	for(int i=0; i<_totalUsersEvaluated; i++){
		vector<DataChunck *> dataChunckSet = DataSet::createDataSetDataChunckFormatFromFile(_userIds[i], Utils::EVALUATE);
		int totalElements = dataChunckSet.size();
		cout << "Elements in user: " << _userIds[i] << "is " << totalElements << endl;
		if(totalElements > 0){
			_evaluateDataChunckSetCollection.push_back(dataChunckSet);
		}else{
			cout << "Not possible to add chucnkset user " << _userIds[i] << endl;
		}
	}
	return true;
}

int main(int argc, char **argv){
	if(argc < 5){
		cout << "Program needs at least the following elements" << endl;
		cout << "0 Program" << endl;
		cout << "1 Init samples" << endl;
		cout << "2 Max samples" << endl;
		cout << "3 Samples increment" << endl;
		cout << "4 Total experiments" << endl;
		cout << "(<trained_matrix_file> <user_id>)+" << endl;
	}
	vector<char *> fileNames;
	vector<int> userIds;
	_initialSamples = atoi(argv[1]);
	_maximumSamples = atoi(argv[2]);
	_samplesIncrement = atoi(argv[3]);
	_totalExperiments = atoi(argv[4]);

	_sigma = SIGMA;

	for(int files=5; files<argc; files+=2){
		fileNames.push_back(argv[files]);
		_userIds.push_back(atoi(argv[(files+1)]));
	}

	_totalUsersEvaluated = fileNames.size();
	cout << "Total evaluated users: " << _totalUsersEvaluated << endl;

	int matrixComposition = sqrt(_totalUsersEvaluated);

	cout << "Creating evaluation dataset..." << endl;
	_isEvaluationDataSetInitialized = createEvaluationDataSets();
	
	if(!_isEvaluationDataSetInitialized){
		cout << "It was not possible to create the evaluation dataset" << endl;
		return 1;
	}else{
		cout << "Evaluation dataset was created correctly" << endl;
	}

	_width = BASEWIDTH * matrixComposition;
	_height = BASEHEIGHT * matrixComposition;
	_openGLFovy = BASEOPENGLFOVY * matrixComposition;

	cout << _totalUsersEvaluated << " Are about to be exported..." << endl;

	_som = Utils::importSOMFromFiles(fileNames, matrixComposition, _totalUsersEvaluated);

	cout << "Files were exported correctly" << endl;
		
	glutInitWindowSize(_width, _height);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(WINDOWTITLE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	init();
	glutMainLoop();
	delete _som;
	return 0;
}