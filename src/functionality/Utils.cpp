#include "../../include/functionality/Classes.h"

int Utils::getRandomDoubleNumber(){
	srand(time(NULL));
    return (double)rand() / RAND_MAX;
}

void Utils::exportMatrixToFile(Matrix *matrix, int completedEpochs,
			int maxEpochs, double initialLearningRate,
			double finalLearningRate){
	string fileName = Utils::buildFileName();

	ofstream file;

	file.open (fileName);

	int matrixSize = matrix->getSize();

	int totalWeigths =  matrix->getTotalWeights();

	file << matrixSize << "\n" << completedEpochs << "\n" << maxEpochs << "\n";
	file << initialLearningRate << "\n" << finalLearningRate << "\n";
	file << totalWeigths << "\n";
	for(int row=0; row<matrixSize; row++){
		string line = matrix->getNeuron(row, 0)->exportNeuron();
		for(int col=1; col<matrixSize; col++){
			line += " " + matrix->getNeuron(row, col)->exportNeuron();
		}
		line += "\n";
		file << line;
	}
	file.close();
	cout << "Se ha terminado de guardar el entrenamiento" << endl;
}

void Utils::exportDataChuckDataSet(int type, int userId, vector<DataChunck *> &dataSet){
	string filename = Utils::buildFileNameDataSet(type, userId);

	cout << "DataChunck " << filename << " start exporting..." << endl;

	ofstream file;
	file.open("chunks/" + filename);
	int totalElements = dataSet.size();
	file << totalElements;
	file << "\n";
	for(int i=0; i<totalElements; i++){
		string line = dataSet[i]->exportDataChunck() + "\n";
		file << line;
	}
	file.close();
	cout << "DataChunck " << filename << " exported correctly" << endl;
}

SelfOrganizingMaps* Utils::importSOMFromFiles(char *fileName){
    fstream infile;
    string line;
    vector<double> neuronWeights;

    // TODO: Check if the file exists otherwise return

    infile.open(fileName);

    getline(infile,line);
    int size = atoi(line.c_str());
    getline(infile,line);
    int executedEpochs = atoi(line.c_str());
    getline(infile,line);
    int maxEpochs = atoi(line.c_str());
    getline(infile,line);
    double initialLearningRate = atof(line.c_str());
    getline(infile,line);
    double currentLearningRate = atof(line.c_str());
    getline(infile,line);
    int totalWeights = atoi(line.c_str());
    
    neuronWeights.resize(totalWeights);
    
    SelfOrganizingMaps *som = new SelfOrganizingMaps(size, totalWeights,
		maxEpochs, initialLearningRate, 0, false);

    // TODO: Check if som was succesfully created, other wise return
	
	for(int row=0; row<size; row++){ // For every row
		getline(infile,line);
        stringstream ssin(line);
        for(int col=0; col<size; col++){ // for each column
			// Get all the weights of the neuron in (row, col)
            for(int weigths=0; weigths<totalWeights; weigths++){
				ssin >> neuronWeights[weigths];
            }
            som->setNeuron(new Neuron(row, col, neuronWeights));
        }
    }

    infile.close();

    return som;
}

Matrix* Utils::importMatrixFromFile(char *fileName){
    fstream infile;
    string line;
    vector<double> neuronRGB;
    int constructedIdUser = 0;
    int evaluatedIdUser = 0;
    int intEvaluated = 0;
    int intMatchingUser = 0;
    bool evaluated = false;
    bool matchingUser = false;

    infile.open(fileName);

    getline(infile,line);
    int size = atoi(line.c_str());
    getline(infile,line);
    int executedEpochs = atoi(line.c_str());
    getline(infile,line);
    int maxEpochs = atoi(line.c_str());
    getline(infile,line);
    double initialLearningRate = atof(line.c_str());
    getline(infile,line);
    double currentLearningRate = atof(line.c_str());
    getline(infile,line);
    int totalWeights = atoi(line.c_str());
    
    neuronRGB.resize(totalWeights);
    
    Matrix *matrix = new Matrix(size, totalWeights, false);
	
	for(int row=0; row<size; row++){ // For every row
		getline(infile,line);
        stringstream ssin(line);
        for(int col=0; col<size; col++){ // for each column
			// Get all the weights of the neuron in (row, col)
            for(int weigths=0; weigths<totalWeights; weigths++){
				ssin >> neuronRGB[weigths];
            }


//#ifdef DEBUG_STATICS
			ssin >> constructedIdUser;
			ssin >> evaluatedIdUser;
			ssin >> intEvaluated;
			ssin >> intMatchingUser;

			if(intEvaluated == 1){
				evaluated = true;
			}

			if(intMatchingUser == 1){
				matchingUser = true;
			}

#ifdef DEBUG_STATICS
			cout << constructedIdUser << " " << evaluatedIdUser << " ";
			cout << to_string(intEvaluated) <<  " ";
			cout << to_string(intMatchingUser) << endl;
#endif

            RGB *rgb = new RGB(neuronRGB[0], neuronRGB[1], neuronRGB[2]);
            Neuron * currentNeuron = new Neuron(row, col, totalWeights, rgb);
            currentNeuron -> setNeuronStatics(constructedIdUser, evaluatedIdUser, evaluated,
				matchingUser);
            matrix->setNeuron(currentNeuron);
        }
    }

    infile.close();

    return matrix;
}

SelfOrganizingMaps* Utils::importSOMFromFiles(vector<char *> fileNames,
	int matrixComposition, int totalFiles){

	// Final matrix variables
	int diferential = matrixComposition - 1;
	int matrixSize = 0;
	int matrixTotalWeights = 0;
	int matrixRow = 0;
	int matrixCol = 0;

	// Individual matrix variables
	vector<Matrix *> matrices;
	int matricesIndex = 0;
	int singleMatrixSize = 0;
	int singleMatrixColumn = 0;
	int singleMatrixRow = 0;

	for(int i=0; i<totalFiles; i++){
		matrices.push_back(Utils::importMatrixFromFile(fileNames[i]));
	}

	//cout << "1" << endl;

	// TODO: get the minimum size of all the matrixes and with that
	// set the single matrix size
	singleMatrixSize = matrices[0]->getSize();
	matrixSize = matrixComposition * singleMatrixSize;
	matrixTotalWeights = matrices[0]->getTotalWeights();

	Matrix *matrix =  new Matrix(matrixSize, matrixTotalWeights, false);

	//cout << "2" << endl;

	for(int row=0; row< matrixSize; row++){
		for(int col=0; col<matrixSize; col++){
			// Gets the individual matrix that corresponds to
			// the current row
			matrixRow = row/singleMatrixSize;

			// Gets the individual matrix that corresponds to
			// the current col
			matrixCol = col/singleMatrixSize;

			singleMatrixRow = row%singleMatrixSize;
			singleMatrixColumn = col%singleMatrixSize;

			// From which matrix I'll take the neuron, based on the current
			//row and column
			matricesIndex = (matrixRow + matrixCol) + (matrixRow * diferential);
			
#ifdef DEBUG
			cout << "Utils CH: " << row << " "<< matricesIndex << "(" << singleMatrixRow << "," << singleMatrixColumn << ")   -   ";
			cout << "(" << row << "," << col << ")" << endl;
#endif
			
			Neuron *currentNeuron = matrices[matricesIndex]->getNeuron(singleMatrixRow,singleMatrixColumn);
			currentNeuron->setY(row);
			currentNeuron->setX(col);
			matrix->setNeuron(currentNeuron);
#ifdef DEBUG
			cout << "Utils CH: " << row << " " << matricesIndex << "(" << singleMatrixRow << "," << singleMatrixColumn << ")   -   ";
			cout << "(" << row << "," << col << ")" << endl;
#endif
		}
	}

	//cout << "3" << endl;

	// TODO: get maxEpochs and initial learning rate
	SelfOrganizingMaps *som = new SelfOrganizingMaps(matrixSize, matrixTotalWeights, 0, 0, matrix, 0);

    return som;
}

string Utils::currentTime(){
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
	return buf;
}

string Utils::currentDate(){
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%B %A ", &tstruct);
	return buf;
}

string Utils::buildFileName(){
	time_t t = time(0);
	struct tm * now = localtime( & t );
	int month = (now->tm_mon + 1);
    int day = (now->tm_mday  + 1);
    int year = (now->tm_year + 1900);
    string fileName = to_string(year) + "." + to_string(month) + "." +
		to_string(day) + "-" + Utils::currentTime() + ".txt";
	return fileName;
}

string Utils::buildFileNameDataSet(int type, int userId){
	time_t t = time(0);
	struct tm * now = localtime( & t );
	int month = (now->tm_mon + 1);
    int day = (now->tm_mday  + 1);
    int year = (now->tm_year + 1900);
    string fileName = to_string(type) + "." + to_string(userId) + ".txt";
	return fileName;
}

double Utils::getVariance(vector<double> samples, double average){
	double variance;
	double deviation;
	int samplesSize =  samples.size();
	for(int currentSample=0; currentSample<samplesSize; currentSample++){
		deviation = samples[currentSample] - average;
		deviation *= deviation;
		variance += deviation;
	}
	variance = variance/samplesSize;
	return variance;
}