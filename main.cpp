#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <GL/glut.h>

#include "include/Classes.h"

#define BASEWIDTH 400
#define BASEHEIGHT 400
#define BASEOPENGLFOVY 45.0
#define TOTALWEIGHTS 3
#define MAXEPOCHS 1000
#define NORMALSIZE 100
#define IMPORTSIZE 200
#define INITIALLEARNINGRATE 0.1
#define WINDOWTITLE "Self Organizing Maps"

using namespace std;

// Global variables
vector<vector<double> > _trainingDataSet;
vector<vector<double> > _testDataset;
vector<RGB* > _initializationDataSet;
vector<RGB*> _rgbTrainingDataSet;
vector<vector<RGB*> > _evaluationDataSet;
SelfOrganizingMaps *_som;
int _dataSetType;
int _dataSetSize;
int _width;
int _height;
int _executionType;
int _sigma;
double _openGLFovy;
char *_fileName;
bool _training;

// main methods
void createEvaluationDataSet();
void algorithmInitialization(int dataSetType, int size, int totalWeigths,
	int maxEpochs, double initialLearningRate);

// OpenGl methods
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);
void init();

int main(int argc, char **argv){
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

	createEvaluationDataSet();

	cout << "El dataset de prueba fue creado" << endl;

	switch(_executionType){
		case 0: // Get the matrix randomly from a dataset
			algorithmInitialization(_dataSetType, NORMALSIZE, TOTALWEIGHTS,
				MAXEPOCHS, INITIALLEARNINGRATE);
			_width = BASEWIDTH;
			_height = BASEHEIGHT;
			_openGLFovy = BASEOPENGLFOVY;
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

void algorithmInitialization(int dataSetType, int size, int totalWeigths,
	int maxEpochs, double initialLearningRate){
	switch(dataSetType){
		// Many colors dataset
		case 1:
			_trainingDataSet = Utils::createColorDataSet(48, totalWeigths);
			_testDataset = Utils::createColorTestDataSet();
			_dataSetSize = _trainingDataSet.size();
			_som = new SelfOrganizingMaps(size, totalWeigths, maxEpochs, initialLearningRate, _dataSetSize, true);
			break;

		// Blue colors dataset
		case 2:
			_initializationDataSet = Utils::createBlueColorBuildMatrixDataSet();
			_rgbTrainingDataSet = Utils::createBlueColorTrainMatrixDataSet();
			_dataSetSize = _rgbTrainingDataSet.size();
			_som = new SelfOrganizingMaps(size, totalWeigths, maxEpochs, initialLearningRate, _initializationDataSet, _dataSetSize);
			break;
		case 3:
			_initializationDataSet = Utils::createRedColorBuildMatrixDataSet();
			_rgbTrainingDataSet = Utils::createRedColorTrainMatrixDataSet();
			_dataSetSize = _rgbTrainingDataSet.size();
			_som = new SelfOrganizingMaps(size, totalWeigths, maxEpochs, initialLearningRate, _initializationDataSet, _dataSetSize);
			break;
		case 4:
			_initializationDataSet = Utils::createGreenColorBuildMatrixDataSet();
			_rgbTrainingDataSet = Utils::createGreenColorTrainMatrixDataSet();
			_dataSetSize = _rgbTrainingDataSet.size();
			_som = new SelfOrganizingMaps(size, totalWeigths, maxEpochs, initialLearningRate, _initializationDataSet, _dataSetSize);
			break;
		case 5:
			_initializationDataSet = Utils::createYellowColorBuildMatrixDataSet();
			_rgbTrainingDataSet = Utils::createYellowColorTrainMatrixDataSet();
			_dataSetSize = _rgbTrainingDataSet.size();
			_som = new SelfOrganizingMaps(size, totalWeigths, maxEpochs, initialLearningRate, _initializationDataSet, _dataSetSize);
			break;
		default:
			_initializationDataSet = Utils::createMultipleColorBuildMatrixDataSet();
			_rgbTrainingDataSet = Utils::createMultipleColorTrainMatrixDataSet();
			_dataSetSize = _rgbTrainingDataSet.size();
			_som = new SelfOrganizingMaps(size, totalWeigths, maxEpochs, initialLearningRate, _initializationDataSet, _dataSetSize);
			break;
	}
}

void createEvaluationDataSet(){
	_evaluationDataSet.push_back(Utils::createBlueColorTestMatrixDataSet());
	_evaluationDataSet.push_back(Utils::createGreenColorTestMatrixDataSet());
	_evaluationDataSet.push_back(Utils::createYellowColorTestMatrixDataSet());
	_evaluationDataSet.push_back(Utils::createRedColorTestMatrixDataSet());
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
			_training = !_training;
			if(_training)
				cout << "Training..." << endl;
			else
				cout << "Stopped training" << endl;
			break;
		case 'r':
			_training = false;
			_som->reset();
			cout << "Map reset" << endl;
			glutPostRedisplay();
			break;
		case '0':
			cout << "0" << endl;
			//0 0 255 Azul
			_som->evaluateIndependentVector(_testDataset[0]);
			glutPostRedisplay();
			break;
		case '1':
			cout << "1" << endl;
			// 255 0 0 Rojo
			_som->evaluateIndependentVector(_testDataset[1]);
			glutPostRedisplay();
			break;
		case '2':
			// 204 0 204 Violeta
			cout << "2" << endl;
			_som->evaluateIndependentVector(_testDataset[2]);
			glutPostRedisplay();
			break;
		case '3':
			// 0 255 0 Verde 
			cout << "3" << endl;
			_som->evaluateIndependentVector(_testDataset[3]);
			glutPostRedisplay();
			break;
		case 's':
			if(_training)
				_training = !_training;
			break;
		case 'e':
			Utils::exportMatrixToFile(_som->getMatrix(), _som->getEpochs(),
				MAXEPOCHS, INITIALLEARNINGRATE, _som->getCurrenLearningRate());
			break;
		case 'y':
			cout << "Yellow dataset" << endl;
			_som->evaluateIndependentRGBDataSet(_evaluationDataSet[2], _sigma);
			glutPostRedisplay();
			break;
		case 'g':
			cout << "Green dataset" << endl;
			_som->evaluateIndependentRGBDataSet(_evaluationDataSet[1], _sigma);
			glutPostRedisplay();
			break;
		case 'd':
			cout << "Red dataset" << endl;
			_som->evaluateIndependentRGBDataSet(_evaluationDataSet[3], _sigma);
			glutPostRedisplay();
			break;
		case 'b':
			cout << "Blue dataset" << endl;
			_som->evaluateIndependentRGBDataSet(_evaluationDataSet[0], _sigma);
			glutPostRedisplay();
			break;
	}
}

void idle(void){
	if (_training && (_som->getEpochs() < MAXEPOCHS)){
		int randInput = rand() % _dataSetSize;
		if(_dataSetType == 1){
			_som->trainSegmentedFunctions(_trainingDataSet[randInput]);
		}else{
			_som->trainSegmentedFunctions(_rgbTrainingDataSet[randInput]->rgbToVector());
		}
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