#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <GL/glut.h>

#include "include/Classes.h"

#define BASEWIDTH				400
#define BASEHEIGHT 				400
#define BASEOPENGLFOVY			45.0
#define TOTALWEIGHTS 			3
#define MAXEPOCHS 				1000
#define NORMALSIZE 				100
#define INITIALLEARNINGRATE 	0.1
#define BUILD					0
#define TRAIN					1
#define EVALUATE				2
#define SIGMA 					3
#define CHUNCKTIMESIZE 			5	// Given in minutes
#define CHUNCKTIMEINTERVAL 		10	// Given in seconds
#define WINDOWTITLE 			"Self Organizing Maps"
//#define DEBUG

using namespace std;

// Package Variables
vector<DataPackage *>	_buildDataSet;
vector<DataPackage *>	_trainDataSet;
vector<DataChunck *>	_buildDataChunckSet;
vector<DataChunck *>	_trainDataChunckSet;
vector<DataChunck *>	_evaluateDataChunckSet;
vector<vector<DataChunck *> > _evaluateDataChunckSetCollection;
SelfOrganizingMaps		*_som;
double					_openGLFovy;
int						_dataSetSize;
int 					_sigma;
int 					_executionType;
int 					_width;
int 					_height;
int 					_countingSampling;
bool 					_training;
bool					_isTestingDataSetInitialized;

// Local Methods
void algorithmInitializationDataPackage(int size, int totalWeigths, int maxEpochs,
	double initialLearningRate, int dataSetType, int initial, int final);
void algorithmInitializationDataChunck(int size, int totalWeigths, int maxEpochs,
	double initialLearningRate, int dataSetType, int initial, int final,
	int chunckTimeSize, int chunckTimeInterval);
bool createTestingDataSetInitialization();
bool testingDataSetInitialization(vector<int> dataSetTypes, vector<int> initials,
	vector<int> finals, int chunckTimeSize, int chunckTimeInterval);


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

	_buildDataChunckSet = DataSet::createDataSetDataChunckFormat("user1", BUILD, 0, 1, 5, 10);
	cout << "DataSetSize: " << _buildDataChunckSet.size() << endl;
	_buildDataChunckSet[0]->info();
	_buildDataChunckSet[_buildDataChunckSet.size() - 1]->info();
*/

	if(argc < 3 ){
		cout << "Se requieren 3 argumentos para iniciar el programa" << endl;
		cout << "1: Programa" << endl;
		cout << "2: tipo de ejecucion [0 - Dataset | 1 - Cargar matrix entrenada]";
		cout << "3: Dataset deseado para crear y entrenar la matriz o archivos que comforman la matriz entrenada" << endl;
		return 0;
	}

	cout << "El numero de argumentos es valido" << endl;

	_training = false;
	_sigma = SIGMA;
	_executionType = atoi(argv[1]);

#ifdef DEBUG
	_countingSampling = 0;
#endif

	int dataSetType = 0;
	int initial = 0;
	int final = 0;
	int chunckTimeSize = 0;
	int chunckTimeInterval = 0;
	int totalFiles = 0;
	int matrixComposition = 0;

	switch(_executionType){
		case 0: // Analyze an user
			dataSetType = atoi(argv[2]);
			initial = atoi(argv[3]);
			final = atoi(argv[4]);
			chunckTimeSize = 5;
			chunckTimeInterval = 10;

			cout << "Se esta creando el dataset...";

			algorithmInitializationDataChunck(NORMALSIZE, TOTALWEIGHTS,
				MAXEPOCHS, INITIALLEARNINGRATE, dataSetType, initial, final, CHUNCKTIMESIZE, CHUNCKTIMEINTERVAL);

			cout << "El dataset fue creado correctamente";

			_width = BASEWIDTH;
			_height = BASEHEIGHT;
			_openGLFovy = BASEOPENGLFOVY;
			break;
		case 1: // Get the matrix from a previous training
			vector<char *> fileNames;
			totalFiles = 0;
			matrixComposition = 0;

			for(int files = 2; files<argc; files++){
				fileNames.push_back(argv[files]);
			}

			totalFiles = fileNames.size();
			matrixComposition = sqrt(totalFiles);

			// TODO:
			// Get one way to retrive all the parameters needed for creating
			// a testing dataset
			cout << "Se esta creando el dataset de evaluacion..." << endl;
			_isTestingDataSetInitialized = createTestingDataSetInitialization();
			if(!_isTestingDataSetInitialized)
				cout << "No fue posible crear el dataSet para evaluacion" << endl;
			else
				cout << "El dataset de evaluacion fue creado correctamente" << endl;
			
			_width = BASEWIDTH * matrixComposition;
			_height = BASEHEIGHT * matrixComposition;
			_openGLFovy = BASEOPENGLFOVY * matrixComposition;

			cout << "Se estan exportando " << totalFiles << " archivos..." << endl;

			_som = Utils::importSOMFromFiles(fileNames, matrixComposition,
				totalFiles);

			cout << "Los archivos fueron exportados correctamente" << endl;
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
	glutKeyboardFunc(keyboard); //TODO
	glutIdleFunc(idle);

	// OpenGL initalize parameters
	init();

	// OpenGL loop
	glutMainLoop();

	// Release memory
	delete _som;

	return 0;
}

void algorithmInitializationDataPackage(int size, int totalWeigths, int maxEpochs,
	double initialLearningRate, int dataSetType, int initial, int final){

	string user =  "user" + to_string(dataSetType);

	_buildDataSet = DataSet::createDataSetPackageFormat(user, BUILD, initial, final);
	_trainDataSet = DataSet::createDataSetPackageFormat(user, TRAIN, initial, final);

	_dataSetSize = _trainDataSet.size();
	_isTestingDataSetInitialized = false;

	_som = new SelfOrganizingMaps(size, totalWeigths, maxEpochs, initialLearningRate, _buildDataSet, _dataSetSize);
}

void algorithmInitializationDataChunck(int size, int totalWeigths, int maxEpochs,
	double initialLearningRate, int dataSetType, int initial, int final,
	int chunckTimeSize, int chunckTimeInterval){

	string user =  "user" + to_string(dataSetType);

	_buildDataChunckSet = DataSet::createDataSetDataChunckFormat(user, BUILD, initial, final, chunckTimeSize, chunckTimeInterval);
	_trainDataChunckSet = DataSet::createDataSetDataChunckFormat(user, TRAIN, initial, final, chunckTimeSize, chunckTimeInterval);
	_evaluateDataChunckSet = DataSet::createDataSetDataChunckFormat(user, EVALUATE, initial, final, chunckTimeSize, chunckTimeInterval);

	_dataSetSize = _trainDataChunckSet.size();
	_isTestingDataSetInitialized = true;

	_som = new SelfOrganizingMaps(size, totalWeigths, maxEpochs, initialLearningRate, _buildDataChunckSet, _dataSetSize);
}

bool createTestingDataSetInitialization(){
	vector<int> dataSetTypes;
	vector<int> initials;
	vector<int> finals;

	dataSetTypes.push_back(3);
	initials.push_back(0);
	finals.push_back(9);

	return testingDataSetInitialization(dataSetTypes, initials, finals, CHUNCKTIMESIZE, CHUNCKTIMEINTERVAL);
}

bool testingDataSetInitialization(vector<int> dataSetTypes, vector<int> initials,
	vector<int> finals, int chunckTimeSize, int chunckTimeInterval){
	int totalDataSets = dataSetTypes.size();
	int initial = 0;
	int final = 0;
	string user =  "";

	for(int i=0; i<totalDataSets; i++){
		user =  "user" + to_string(dataSetTypes[i]);
		initial = initials[i];
		final = finals[i];
		vector<DataChunck *> dataChunckSet =
			DataSet::createDataSetDataChunckFormat(user, EVALUATE, initial, final, chunckTimeSize, chunckTimeInterval);
		_evaluateDataChunckSetCollection.push_back(dataChunckSet);
	}

	if(_evaluateDataChunckSetCollection.size() != totalDataSets){
		cout << "Error en la construccion de la coleccion de dataSets";
		_evaluateDataChunckSetCollection.clear();;
		return false;
	}

	return true;
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
			cout << "Export matrix..." << endl;
			Utils::exportMatrixToFile(_som->getMatrix(), _som->getEpochs(),
				MAXEPOCHS, INITIALLEARNINGRATE, _som->getCurrenLearningRate());
			break;
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
		_som->trainSegmentedFunctions(_trainDataChunckSet[randInput]->dataChunckToVector());
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