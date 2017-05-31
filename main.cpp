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
#define NORMALSIZE 				50
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
int 							_executionType;
int 							_width;
int 							_height;
int 							_countingSampling;
int 							_randInput;
int 							_version;
int 							_currentEpochs;
bool 							_training;
bool							_isEvaluationDataSetInitialized;
SelfOrganizingMaps				*_som;

// Results variables
int 							_totalUsersEvaluated;
int 							_maximumSamples;
int 							_initialSamples;
int 							_totalExperiments;
int 							_samplesIncrement;

// DataSet variables
vector<DataChunck *>			_buildDataChunckSet;
vector<DataChunck *>			_trainDataChunckSet;
vector<DataChunck *>			_evaluateDataChunckSet;
vector<vector<DataChunck *> >	_evaluateDataChunckSetCollection;
vector<int> 					_userIds;

// ===================== Local Method Headers =====================
// OpenGl methods
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);
void init();

// Algorithm methods
void initializeDataSetsForUser(int idUser, int chunckTimeSize, int chunckTimeInterval);
bool createEvaluationDataSets();
// ===================== Local Method Headers =====================

// ===================== Method Declaration =======================
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
			if(_executionType == 0){
				cout << "Execution type is training" << endl;
				cout << "Total samples: " << _trainDataSetSize << endl;

				_training = !_training;

				if(_training)
					cout << "Training..." << endl;
				else
					cout << "Stopped training" << endl;
			}else{
				cout << "Train function is only valid for Train Matrix, current execution type is Load Matrix" << endl;
			}
			break;
		// Results obtention
		case 'r':
			if(_executionType == 0){
				cout << "Obtain results function is only valid for Load Matrix execution type, current execution type is Train Matrix" << endl;
			}else{
				cout << "Results obtention..." << endl;
				Results::getResults(_initialSamples, _maximumSamples,
					_samplesIncrement, _sigma, _totalExperiments,
					_totalUsersEvaluated, _som, _evaluateDataChunckSetCollection, _userIds);
				cout << "Results obtention finished" << endl;
				glutPostRedisplay();
			}
			break;
		// Stop training
		case 's':
			if(_executionType == 0){
				if(_training){
					_training = !_training;
				}
				cout << "Training has been stopped" << endl;
			}else{
				cout << "Stop training function is only valid for Train Matrix, current execution type is Load Matrix" << endl;
			}
			break;
		// Export matrix
		case 'e':
			cout << "Export matrix" << endl;
			Utils::exportMatrixToFile(_som->getMatrix(), _som->getEpochs(),
				MAXEPOCHS, INITIALLEARNINGRATE, _som->getCurrenLearningRate());
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

/*
* Initialize build, train and evaluate dataSets for a specific user
* enables isEvaluationDataSetInitialized flag
*/
void initUserDataSet(int idUser, int chunckTimeSize, int chunckTimeInterval, int version){
	cout << "Start building Build DataSet..." << endl;
	_buildDataChunckSet = DataSet::createDataSetDataChunckFormat(idUser, Utils::BUILD, chunckTimeSize, chunckTimeInterval, version);
	cout << "Finish building Build DataSet" << endl;
	cout << "Start building Train DataSet..." << endl;
	_trainDataChunckSet = DataSet::createDataSetDataChunckFormat(idUser, Utils::TRAIN, chunckTimeSize, chunckTimeInterval, version);
	cout << "Finish building Train DataSet" << endl;
	cout << "Start building Evaluation DataSet..." << endl;
	_evaluateDataChunckSet = DataSet::createDataSetDataChunckFormat(idUser, Utils::EVALUATE, chunckTimeSize, chunckTimeInterval, version);
	cout << "Finish building Evaluation DataSet" << endl;

	_trainDataSetSize = _trainDataChunckSet.size();

	_isEvaluationDataSetInitialized = true;
}

/*
* Create evaluation datasets for the three users
* if no data is available, Utils:BUILD data is used
*/
bool createEvaluationDataSets(){
	if(_version == 1){
		int dataSetType = Utils::BUILD;
		for(int i=1; i<4; i++){
			if(i == 3){
				dataSetType = Utils::EVALUATE;
			}
			vector<DataChunck *> dataChunckSet =
				DataSet::createDataSetDataChunckFormat(i, dataSetType, CHUNCKTIMESIZE, CHUNCKTIMEINTERVAL, _version);
			_evaluateDataChunckSetCollection.push_back(dataChunckSet);
		}

		if(_evaluateDataChunckSetCollection.size() != 3){
			cout << "Error initializing de evaluation datasets";
			_evaluateDataChunckSetCollection.clear();;
			return false;
		}
		return true;
	}else{
		for(int i=0; i<_totalUsersEvaluated; i++){
			vector<DataChunck *> dataChunckSet =
				DataSet::createDataSetDataChunckFormat(_userIds[i], Utils::EVALUATE, CHUNCKTIMESIZE, CHUNCKTIMEINTERVAL, _version);
			_evaluateDataChunckSetCollection.push_back(dataChunckSet);
		}
		return true;
	}
}

// ===================== Method Declaration =======================

// ===================== Main Declaration =========================
int main(int argc, char **argv){
	if(argc < 3 ){
		cout << "At least 4 parameters are required to use the program" << endl;
		cout << "1: Program name" << endl;
		cout << "2: Execution type [0 - Train Matrix | 1 - Load Matrix]" << endl;
		cout << endl;
		cout << "0 - Train Matrix mode" << endl;
		cout << "3: User required to train matrix" << endl;
		cout << "4: First/Second Version" << endl;
		cout << endl;
		cout << "1 - Load Matrix mode" << endl;
		cout << "3: First/Second Version" << endl;
		cout << "4: Initial amount of samples to evaluate" << endl;
		cout << "5: Maximun amount of samples to evaluate" << endl;
		cout << "6: Increment of samples unit" << endl;
		cout << "7: Total experiments required" << endl;
		cout << "To define the users selected to evaluate the format is as follows" << endl;
		cout << "<savedTraining> <user_id>" << endl;
		return 1;
	}

	// Variable initialization
	_training = false;
	_sigma = SIGMA;
	_executionType = atoi(argv[1]);

	_countingSampling = 0;

	switch(_executionType){
		case 0:{ // Analyze an user
			cout << "Verifying that arguments are valid..." << endl;

			if(argc < 4){
				cout << "Missing arguments for Train matrix mode" << endl;
				return 1;
			}

			int user = atoi(argv[2]);
			_version = atoi(argv[3]);

			cout << "Argumentos validos para la ejecucion por DataSet de usuario" << endl;

			cout << "Se esta creando el dataset desde los archivos..." << endl;

			initUserDataSet(user, CHUNCKTIMESIZE, CHUNCKTIMEINTERVAL, _version);

			cout << "El dataset fue creado correctamente" << endl;

			cout << "Inicializando el algoritmo de SOM..." << endl;
			if(_trainDataSetSize > 0){
				_som = new SelfOrganizingMaps(NORMALSIZE, TOTALWEIGHTS, MAXEPOCHS,
					INITIALLEARNINGRATE, _buildDataChunckSet, _trainDataSetSize, user);
				cout << "El algoritmo de SOM fue inicializado correctamente" << endl;
			}else{
				cout << "No fue posible inicial el algoritmo de SOM, el DataSet de entrenamiento esta vacio" << endl;
				return 1;
			}

			_width = BASEWIDTH;
			_height = BASEHEIGHT;
			_openGLFovy = BASEOPENGLFOVY;
		}
			break;
		case 1:{ // Get the matrix from a previous training
			if(argc < 7){
				cout << "For Load Matrix mode at least 7 parameters are required:" << endl;
				cout << "0 Program name" << endl;
				cout << "1 Execution type" << endl;
				cout << "2 First/Second Version" << endl;
				cout << "3 Initial amount of samples to evaluate" << endl;
				cout << "4 Maximun amount of samples to evaluate" << endl;
				cout << "5 Increment of samples unit" << endl;
				cout << "6 Total experiments required" << endl;
				cout << "7... N Files that conform the matrix" << endl;
			}

			vector<char *> fileNames;
			vector<int> userIds;
			_version = atoi(argv[2]);
			_initialSamples = atoi(argv[3]);
			_maximumSamples = atoi(argv[4]);
			_samplesIncrement = atoi(argv[5]);
			_totalExperiments = atoi(argv[6]);

			for(int files=7; files<argc; files+=2){
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
			}else{
				cout << "Evaluation dataset was created correctly" << endl;
			}

			_width = BASEWIDTH * matrixComposition;
			_height = BASEHEIGHT * matrixComposition;
			_openGLFovy = BASEOPENGLFOVY * matrixComposition;

			cout << _totalUsersEvaluated << " Are about to be exported..." << endl;

			_som = Utils::importSOMFromFiles(fileNames, matrixComposition,
				_totalUsersEvaluated);

			cout << "Files were exported correctly" << endl;
		}
			break;
	}

	// OpenGL window configuration
	glutInitWindowSize(_width, _height);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(WINDOWTITLE);

	// OpenGL register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	// OpenGL initalize parameters
	init();

	// OpenGL loop
	glutMainLoop();

	// Release memory
	delete _som;

	return 0;
}
// ===================== Main Declaration =========================
