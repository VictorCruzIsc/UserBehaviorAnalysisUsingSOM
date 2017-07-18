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
			vector<ExperimentGeneric *> experiments = 
				Results::processExperimentResults(_initialSamples, _maximumSamples,
					_samplesIncrement, _totalExperiments, _som,
					_evaluateDataChunckSetCollection, _userIds);

			int totalExperiments = experiments.size();

			for(int i=0; i<totalExperiments; i++){
				experiments[i]->info();
			}
			
			glutPostRedisplay();
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