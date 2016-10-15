#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <GL/glut.h>

#include "include/Classes.h"

// Package Defines
#define BASEWIDTH				400
#define BASEHEIGHT 				400
#define BASEOPENGLFOVY			45.0
#define TOTALWEIGHTS 			3
#define MAXEPOCHS 				1000
#define NORMALSIZE 				100
#define INITIALLEARNINGRATE 	0.1
#define WINDOWTITLE 			"Self Organizing Maps"
#define BUILD					0
#define TRAIN					1
#define EVALUATE				2
#define DEBUG

using namespace std;

// Package Variables
vector<DataPackage *>	_buildDataSet;
vector<DataPackage *>	_trainDataSet;
SelfOrganizingMaps		*_som;
double					_openGLFovy;
int						_dataSetSize;
int 					_sigma;
int 					_executionType;
int 					_width;
int 					_height;
int 					_countingSampling;
bool 					_training;

// Package Methods
void algorithmInitialization(int size, int totalWeigths, int maxEpochs,
	double initialLearningRate, int dataSetType, int initial, int final);

// Global variables
vector<vector<double> > _trainingDataSet;
vector<vector<double> > _testDataset;
vector<RGB* > _initializationDataSet;
vector<RGB*> _rgbTrainingDataSet;
vector<vector<RGB*> > _evaluationDataSet;
int _dataSetType;
char *_fileName;

// OpenGl methods
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);
void init();

int main(int argc, char **argv){
/*
	_buildDataSet = DataSet::createDataSetPackageFormat("user1", BUILD, 0, 1);
	_trainDataSet = DataSet::createDataSetPackageFormat("user1", TRAIN, 0, 1);

	cout << _buildDataSet.size() << endl;
	cout << _trainDataSet.size() << endl;
*/
	
	if(argc < 3 ){
		cout << "Se requieren 3 argumentos para iniciar el programa" << endl;
		cout << "1: Programa" << endl;
		cout << "2: tipo de ejecucion [0 - Dataset | 1 - Cargar matrix entrenada]";
		cout << "3: Dataset deseado para crear y entrenar la matriz o archivos que comforman la matriz entrenada" << endl;
		return 0;
	}

	cout << "Los argumentos para la ejecucion son validos" << endl;

	_training = false;
	_executionType = atoi(argv[1]);
	_dataSetType = atoi(argv[2]);
	_sigma = 3;
	_countingSampling = 0;

	cout << "El dataset de prueba fue creado" << endl;

	switch(_executionType){
		case 0: // Get the matrix randomly from a dataset
			algorithmInitialization(NORMALSIZE * 2, TOTALWEIGHTS,
				MAXEPOCHS, INITIALLEARNINGRATE, _dataSetType, 0, 1);
			_width = BASEWIDTH * 2;
			_height = BASEHEIGHT * 2;
			_openGLFovy = BASEOPENGLFOVY * 2;
			break;
		case 1: // Get the matrix from a previous training
			vector<char *> fileNames;
			int totalFiles = 0;
			int matrixComposition = 0;

			for(int files = 2; files<argc; files++){
				fileNames.push_back(argv[files]);
			}

			totalFiles = fileNames.size();
			matrixComposition = sqrt(totalFiles);
			
			_width = BASEWIDTH * matrixComposition;
			_height = BASEHEIGHT * matrixComposition;
			_openGLFovy = BASEOPENGLFOVY * matrixComposition;

			_som = Utils::importSOMFromFiles(fileNames, matrixComposition,
				totalFiles);

			cout << "Los archivos fueron exportados" << endl;
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

void algorithmInitialization(int size, int totalWeigths, int maxEpochs,
	double initialLearningRate, int dataSetType, int initial, int final){

	string user =  "user" + to_string(dataSetType);

	_buildDataSet = DataSet::createDataSetPackageFormat(user, BUILD, initial, final);
	_trainDataSet = DataSet::createDataSetPackageFormat(user, TRAIN, initial, final);

	_dataSetSize = _trainDataSet.size();

	_som = new SelfOrganizingMaps(size, totalWeigths, maxEpochs, initialLearningRate, _buildDataSet, _dataSetSize);
}

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
		case 't':
			cout << "Total samples: " << _dataSetSize << endl;
			_training = !_training;
			if(_training)
				cout << "Training..." << endl;
			else
				cout << "Stopped training" << endl;
			break;
		case 'r':
			/*
				_training = false;
				_som->reset();
				cout << "Map reset" << endl;
				glutPostRedisplay();
			*/
			cout << "No functionality implemented for reset until now" << endl;
			break;
		case '0':
			cout << "Independent dataset not implemented" << endl;
			/*
				_som->evaluateIndependentVector(_testDataset[0]);
				glutPostRedisplay();
			*/
			break;
		case 's':
			if(_training)
				_training = !_training;
			cout << "Entrenamiento detenido" << endl;
			break;
		case 'e':
			cout << "Export matrix not implemented yet" << endl;
			/*
				Utils::exportMatrixToFile(_som->getMatrix(), _som->getEpochs(),
				MAXEPOCHS, INITIALLEARNINGRATE, _som->getCurrenLearningRate());
				break;
			*/
		case 'b':
			cout << "Blue dataset evaluation not implemented" << endl;
			/*
				_som->evaluateIndependentRGBDataSet(_evaluationDataSet[0], _sigma);
				glutPostRedisplay();
			*/
			break;
	}
}

void idle(void){
	if (_training && (_som->getEpochs() < MAXEPOCHS)){
#ifdef DEBUG
		_countingSampling++;
		if(_countingSampling % 500 == 0)
			cout << _countingSampling << endl;
#endif
		int randInput = rand() % _dataSetSize;
		_som->trainSegmentedFunctions(_trainDataSet[randInput]->dataPackageToVector());
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