#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <time.h>
#include "include/functionality/Classes.h"
#include "include/results/Results.h"

#define MAC
#define BASEWIDTH				400
#define BASEHEIGHT 				400
#define BASEOPENGLFOVY			45.0
#define WINDOWTITLE 			"Self Organizing Maps"
#define TOTALWEIGHTS 			3
#define INITIALLEARNINGRATE 	0.1
#define SIGMA 					1
//#define DEBUG

#ifdef MAC
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <OpenGL/glext.h>
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

using namespace std;

double							_openGLFovy;
int								_trainDataSetSize;
int 							_sigma;
int 							_width;
int 							_height;
int 							_countingSampling;
int 							_randInput;
int 							_user;
int 							_currentEpochs;
bool 							_training;
bool							_isEvaluationDataSetInitialized;
int 							_totalUsersEvaluated;
int 							_maximumSamples;
int 							_initialSamples;
int 							_totalExperiments;
int 							_samplesIncrement;
int 							_matrixSize;
int 							_maxEpochs;
int 							_elementsForTraining;
SelfOrganizingMaps				*_som;
vector<DataChunck *>			_buildDataChunckSet;
vector<DataChunck *>			_completeTrainDataChunckSet;
vector<DataChunck *>			_trainDataChunckSet;

bool initUserDataSet(int idUser, int elementsForTraining);
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);
void init();

bool initUserDataSet(int idUser, int elementsForTraining);

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
		// Training
		case 't':
				cout << "Execution type is training" << endl;
				cout << "Total samples: " << _trainDataSetSize << endl;

				_training = !_training;

				if(_training)
					cout << "Training..." << endl;
				else
					cout << "Stopped training" << endl;
			break;
		case 's':
				if(_training){
					_training = !_training;
				}
				cout << "Training has been stopped" << endl;
			break;
		case 'e':
			cout << "Export matrix" << endl;
			Utils::exportMatrixToFile(_som->getMatrix(), _som->getEpochs(),
				_maxEpochs, INITIALLEARNINGRATE, _som->getCurrenLearningRate());
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

bool initUserDataSet(int idUser, int elementsForTraining){
	int randomSelectedIndex = 0;
	int elementsInCompleteTrainDataSet = 0;
	int elementsInPartialTrainDataSet = 0;
	
	_buildDataChunckSet = DataSet::createDataSetDataChunckFormatFromFile(idUser, Utils::BUILD);
	_completeTrainDataChunckSet = DataSet::createDataSetDataChunckFormatFromFile(idUser, Utils::TRAIN);
	
	elementsInCompleteTrainDataSet = _completeTrainDataChunckSet.size();

	srand (time(NULL));
	
	while(elementsInPartialTrainDataSet < elementsForTraining){
		randomSelectedIndex = rand() % elementsInCompleteTrainDataSet;
		_trainDataChunckSet.push_back(_completeTrainDataChunckSet[randomSelectedIndex]);
		elementsInPartialTrainDataSet++;
	}

	return (_trainDataChunckSet.size() == elementsForTraining);
}

int main(int argc, char **argv){
	if(argc < 5 ){
		cout << "At least 5 parameters are required to use the program" << endl;
		cout << "1: Program name" << endl;
		cout << "2: User id" << endl;
		cout << "3: Matrix size" << endl;
		cout << "4: Max epochs for training" << endl;
		cout << "5: Elements for training" << endl;
		return 1;
	}

	_training = false;
	_sigma = SIGMA;
	_user = atoi(argv[1]);
	_matrixSize = atoi(argv[2]);
	_maxEpochs = atoi(argv[3]);
	_elementsForTraining = atoi(argv[4]);
	_trainDataSetSize = _elementsForTraining;
	_width = BASEWIDTH;
	_height = BASEHEIGHT;
	_openGLFovy = BASEOPENGLFOVY;

	if(initUserDataSet(_user, _elementsForTraining)){
		_som = new SelfOrganizingMaps(_matrixSize, TOTALWEIGHTS, _maxEpochs, INITIALLEARNINGRATE, _buildDataChunckSet, _trainDataSetSize, _user);
	}else{
		cout << "It was not possible to initialize SOM algorithm" << endl;
		return 1;
	}

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

	return 0;
}