#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <GL/glut.h>

#include "include/Classes.h"

// OpenGL constants
#define BASEWIDTH				400
#define BASEHEIGHT 				400
#define BASEOPENGLFOVY			45.0
#define WINDOWTITLE 			"Self Organizing Maps"

// SOM constants
#define TOTALWEIGHTS 			3
#define MAXEPOCHS 				1000
#define NORMALSIZE 				100
#define INITIALLEARNINGRATE 	0.1
#define SIGMA 					1

// DataSet constants
#define CHUNCKTIMESIZE 			5	// Given in minutes
#define CHUNCKTIMEINTERVAL 		10	// Given in seconds

// Results constants
#define MAX_SAMPLES 			2300
#define EXPERIMET_REPETITION	1
#define INCREMENT 				700
#define TOTAL_USERS_EVALUATED 	3

// Developing constants
//#define DEBUG

using namespace std;

// Global variables

// OpenGL variables
double							_openGLFovy;

// SOM variables
int								_trainDataSetSize;
int 							_sigma;
int 							_executionType;
int 							_width;
int 							_height;
int 							_countingSampling;
int 							_evaluationSamples;
int 							_iterationsRequired;
bool 							_training;
bool							_isEvaluationDataSetInitialized;
SelfOrganizingMaps				*_som;

// DataSet variables
vector<DataChunck *>			_buildDataChunckSet;
vector<DataChunck *>			_trainDataChunckSet;
vector<DataChunck *>			_evaluateDataChunckSet;
vector<vector<DataChunck *> >	_evaluateDataChunckSetCollection;

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
void sendAllUsersToEvaluate();
void obtainResults(int increment, int samplesExperimentRepetiion);
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
		case 't':
			cout << "Total samples: " << _trainDataSetSize << endl;
			_training = !_training;
			if(_training)
				cout << "Training..." << endl;
			else
				cout << "Stopped training" << endl;
			break;
		case 'r':
			obtainResults(INCREMENT, EXPERIMET_REPETITION);
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
		case 'q': // Starts stadistics
			cout << "Starting stadistics" << endl;
			_som -> getMatrixStadistics();
			break;
		// Users evaluation
		case '1': // User type 1
			cout << "Evaluation dataset of user " << key << endl;
			cout << "Total samples used: " << _evaluationSamples << endl;
			_som->evaluateIndependentDataChuckDataSet(_evaluateDataChunckSetCollection[0],
				_sigma, _iterationsRequired, _evaluationSamples, 255, 0, 0, 1);
			glutPostRedisplay();
			break;
		case '2': // User type 2
			cout << "Evaluation dataset of user " << key << endl;
			cout << "Total samples used: " << _evaluationSamples << endl;
			_som->evaluateIndependentDataChuckDataSet(_evaluateDataChunckSetCollection[1],
				_sigma, _iterationsRequired, _evaluationSamples, 0, 255, 0, 2);
			glutPostRedisplay();
			break;
		case '3': // User type 3
			cout << "Evaluation dataset of user " << key << endl;
			cout << "Total samples used: " << _evaluationSamples << endl;
			_som->evaluateIndependentDataChuckDataSet(_evaluateDataChunckSetCollection[2],
				_sigma, _iterationsRequired, _evaluationSamples, 0, 0, 255, 3);
			glutPostRedisplay();
			break;
		case 'a':
			cout << "Start execution for all users" << endl;
			sendAllUsersToEvaluate();
			cout << "Finish execution for all users" << endl;
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
		int randInput = rand() % _trainDataSetSize;
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

void initializeDataSetsForUser(int idUser, int chunckTimeSize, int chunckTimeInterval){
	_buildDataChunckSet = DataSet::createDataSetDataChunckFormat(idUser, Utils::BUILD, chunckTimeSize, chunckTimeInterval);
	_trainDataChunckSet = DataSet::createDataSetDataChunckFormat(idUser, Utils::TRAIN, chunckTimeSize, chunckTimeInterval);
	_evaluateDataChunckSet = DataSet::createDataSetDataChunckFormat(idUser, Utils::EVALUATE, chunckTimeSize, chunckTimeInterval);

	_trainDataSetSize = _trainDataChunckSet.size();

	_isEvaluationDataSetInitialized = true;
}

bool createEvaluationDataSets(){
	int dataSetType = Utils::BUILD;
	for(int i=1; i<4; i++){
		if(i == 3){
			dataSetType = Utils::EVALUATE;
		}
		vector<DataChunck *> dataChunckSet =
			DataSet::createDataSetDataChunckFormat(i, dataSetType, CHUNCKTIMESIZE, CHUNCKTIMEINTERVAL);
		_evaluateDataChunckSetCollection.push_back(dataChunckSet);
	}

	if(_evaluateDataChunckSetCollection.size() != 3){
		cout << "Error en la construccion de la coleccion de dataSets";
		_evaluateDataChunckSetCollection.clear();;
		return false;
	}
	return true;
}

void sendAllUsersToEvaluate(){
	cout << "Running evaluation for user 1..." << endl;
	_som->evaluateIndependentDataChuckDataSet(_evaluateDataChunckSetCollection[0],
		_sigma, _iterationsRequired, _evaluationSamples, 255, 0, 0, 1);
	cout << "Running evaluation for user 2..." << endl;
	_som->evaluateIndependentDataChuckDataSet(_evaluateDataChunckSetCollection[1],
		_sigma, _iterationsRequired, _evaluationSamples, 0, 255, 0, 2);
	cout << "Running evaluation for user 3..." << endl;
	_som->evaluateIndependentDataChuckDataSet(_evaluateDataChunckSetCollection[2],
		_sigma, _iterationsRequired, _evaluationSamples, 0, 0, 255, 3);

	cout << "Displaying results..." << endl;
	// Display Results
	glutPostRedisplay();

	int totalNeurons = ((_iterationsRequired * _evaluationSamples) * TOTAL_USERS_EVALUATED);

	cout << "Total expected users: " << TOTAL_USERS_EVALUATED << endl;
	cout << "Iterations: " << _iterationsRequired << endl;
	cout << "Evaluation Samples: " << _evaluationSamples << endl;
	cout << "Total neurons evaluated per user: " << _iterationsRequired * _evaluationSamples << endl;
	cout << "Expected neurons evaluated: " << totalNeurons << endl;
	_som->getMatrixStadistics();
	cout << "Unique matching neurons: " << _som->_incorrect + _som->_correct << endl;
	if((_som->_incorrect + _som->_correct + _som->_totalColitions) == totalNeurons){
		cout << "---> CORRECT <---" << endl;
	}else{
		cout << "---> INCORRECT <---" << endl;
	}
}

void obtainResults(int increment, int samplesExperimentRepetiion){
	cout << "=== Starting results obtention ===\n\n\n" << endl;
	while(_evaluationSamples <= MAX_SAMPLES){
		cout << "*** Starting results obtaining with " << _evaluationSamples << " samples ***" << endl;
		for(int i=0; i<samplesExperimentRepetiion; i++){
			cout << "# Start Experiment: " << i+1 << "/" << samplesExperimentRepetiion << " #" << endl;
				sendAllUsersToEvaluate();
				_som->resetMatrixStadistics();
				_som->_incorrect = 0;
				_som-> _correct = 0;
				_som->_totalColitions = 0;
				_som->_errorStadisticsResults.clear();
				_som->_correctStadisticsResults.clear();
			cout << "# Finish Experiment: " << i+1 << "/" << samplesExperimentRepetiion << " #" << endl;
		}
		cout << "*** Finish results obtaining with " << _evaluationSamples << " samples ***\n\n\n" << endl;
		_evaluationSamples += increment;
	}
	cout << "=== Finish results obtention ===" << endl;
}

// ===================== Method Declaration =======================

// ===================== Main Declaration =========================
int main(int argc, char **argv){
	if(argc < 3 ){
		cout << "Se requieren al menos 3 argumentos para iniciar el programa" << endl;
		cout << "1: Programa" << endl;
		cout << "2: Tipo de ejecucion [0 - Dataset | 1 - Cargar matriz entrenada]" << endl;
		cout << "Mod 0 - 3: Usuario requerido para crear y entrenar la matriz" << endl;
		cout << "Mod 1 - 3: Numero de elementos que se van a usar en la evaluacion de el patron general" << endl;
		cout << "Mod 1 - 4: Numero de iteracion para la evaluacion de datasets" << endl;
		cout << "Mod 1 - 5: Archivos que comformaran el patron general" << endl;
		return 1;
	}

	_training = false;
	_sigma = SIGMA;
	_executionType = atoi(argv[1]);

#ifdef DEBUG
	_countingSampling = 0;
#endif

	switch(_executionType){
		case 0:{ // Analyze an user
			cout << "Verificando argumentos validos para ejecucion por DataSet de usuario..." << endl;
			if(argc < 3){ // 0: Program, 1: Execution type, 2: User to be analyzed
				cout << "Hacen falta argumentos para la ejecucion del por DataSet de usuario" << endl;
				return 1;
			}

			int user = atoi(argv[2]);

			cout << "Argumentos validos para la ejecucion por DataSet de usuario" << endl;

			cout << "Se esta creando el dataset desde los archivos..." << endl;

			initializeDataSetsForUser(user, CHUNCKTIMESIZE, CHUNCKTIMEINTERVAL);

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
		case 1: // Get the matrix from a previous training
			vector<char *> fileNames;
			_evaluationSamples = atoi(argv[2]);
			_iterationsRequired = atoi(argv[3]);


			for(int files = 4; files<argc; files++){
				fileNames.push_back(argv[files]);
			}

			int totalFiles = fileNames.size();
			int matrixComposition = sqrt(totalFiles);

			// TODO:
			// Get one way to retrive all the parameters needed for creating
			// a testing dataset
			cout << "Se esta creando el dataset de evaluacion..." << endl;
			_isEvaluationDataSetInitialized = createEvaluationDataSets();
			if(!_isEvaluationDataSetInitialized)
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
// ===================== Main Declaration =========================