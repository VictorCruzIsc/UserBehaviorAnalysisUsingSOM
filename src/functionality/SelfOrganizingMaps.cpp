#include "../../include/functionality/Classes.h"

SelfOrganizingMaps::SelfOrganizingMaps(int size, int totalWeights,
	int maxEpochs, double initialLearningRate, int totalSamples, bool initialize):
	_iterations(0), _size(size), _totalWeigths(totalWeights),
	_maxEpochs(maxEpochs),
	_epochs(0), _initialLearningRate(initialLearningRate),
	_initialNeighbourhoodRadius(size/2), _totalSamples(totalSamples),
	_totalPositiveColitions(0), _totalNegativeColitions(0), _correct(0), _incorrect(0){
	_matrix =  new Matrix(_size, _totalWeigths, initialize);
	_radiusTimeConstant = _maxEpochs/log(_initialNeighbourhoodRadius);
	_learningRateTimeConstant = _maxEpochs/log(_initialLearningRate);
}

SelfOrganizingMaps::SelfOrganizingMaps(int size, int totalWeights,
	int maxEpochs, double initialLearningRate, vector<RGB* > dataSet,
	int totalSamples):
	_iterations(0), _size(size), _totalWeigths(totalWeights),
	_maxEpochs(maxEpochs),
	_epochs(0), _initialLearningRate(initialLearningRate),
	_initialNeighbourhoodRadius(size/2), _totalSamples(totalSamples),
	_totalPositiveColitions(0), _totalNegativeColitions(0), _correct(0), _incorrect(0){
	_matrix =  new Matrix(_size, _totalWeigths, dataSet);
	_radiusTimeConstant = _maxEpochs/log(_initialNeighbourhoodRadius);
	_learningRateTimeConstant = _maxEpochs/log(_initialLearningRate);
}

SelfOrganizingMaps::SelfOrganizingMaps(int size, int totalWeights,
			int maxEpochs, double initialLearningRate, Matrix *matrix,
			int totalSamples):
	_iterations(0), _size(size), _totalWeigths(totalWeights),
	_maxEpochs(maxEpochs),
	_epochs(0), _initialLearningRate(initialLearningRate),
	_initialNeighbourhoodRadius(size/2), _totalSamples(totalSamples),
	_totalPositiveColitions(0), _totalNegativeColitions(0), _correct(0), _incorrect(0){
	_matrix =  matrix;
	_radiusTimeConstant = _maxEpochs/log(_initialNeighbourhoodRadius);
	_learningRateTimeConstant = _maxEpochs/log(_initialLearningRate);
}

SelfOrganizingMaps::SelfOrganizingMaps(int size, int totalWeights,
			int maxEpochs, double initialLearningRate,
			vector<DataPackage* > dataSet, int totalSamples):
	_iterations(0), _size(size), _totalWeigths(totalWeights),
	_maxEpochs(maxEpochs), _epochs(0), _initialLearningRate(initialLearningRate),
	_initialNeighbourhoodRadius(size/2), _totalSamples(totalSamples),
	_totalPositiveColitions(0), _totalNegativeColitions(0), _correct(0), _incorrect(0){
	_matrix =  new Matrix(_size, _totalWeigths, dataSet);
	_radiusTimeConstant = _maxEpochs/log(_initialNeighbourhoodRadius);
	_learningRateTimeConstant = _maxEpochs/log(_initialLearningRate);
}

SelfOrganizingMaps::SelfOrganizingMaps(int size, int totalWeights,
			int maxEpochs, double initialLearningRate,
			vector<DataChunck* > dataSet, int totalSamples, int idUser):
	_iterations(0), _size(size), _totalWeigths(totalWeights),
	_maxEpochs(maxEpochs), _epochs(0), _initialLearningRate(initialLearningRate),
	_initialNeighbourhoodRadius(size/2), _totalSamples(totalSamples),
	_totalPositiveColitions(0), _totalNegativeColitions(0), _correct(0), _incorrect(0){
	_matrix =  new Matrix(_size, _totalWeigths, dataSet, idUser);
	_radiusTimeConstant = _maxEpochs/log(_initialNeighbourhoodRadius);
	_learningRateTimeConstant = _maxEpochs/log(_initialLearningRate);
}


SelfOrganizingMaps::~SelfOrganizingMaps(){
	delete _matrix;
}

int SelfOrganizingMaps::getIterations(){
	return _iterations;
}

int SelfOrganizingMaps::getEpochs(){
	return _epochs;
}

int SelfOrganizingMaps::getSize(){
	return _size;
}

int SelfOrganizingMaps::getTotalWeights(){
	return _totalWeigths;
}

Matrix* SelfOrganizingMaps::getMatrix(){
	return _matrix;
}

// Main functionality of the algorithm

void SelfOrganizingMaps::train(vector<double> inputVector){
	if(_epochs > _maxEpochs){
		return;
	}

	Neuron *currentNeuron = NULL;
	double distanceToNeuron = 0.0;
	double influence = 0.0;
	double distance = 0;
	double winnerNeuronX = 0.0;
	double winnerNeuronY = 0.0;
	double closestDistance = (255.0 * 255.0) + (255.0 * 255.0) + (255.0 * 255.0);

	// Getting BMU
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			distance = _matrix->getNeuron(row, col)->distanceToInputVector(inputVector);
			if(distance < closestDistance){
				winnerNeuronX = row;
				winnerNeuronY = col;
				closestDistance = distance;
			}
		}
	}

	// Adjusting weigths of neurons
	// Get current Learning Rate scientific method
	double currenLearningRate = getCurrenLearningRate();

	// Get neighbourhood radius scientific method
	double neighbourhoodRadius = getNeighbourhoodRadius();

	// Update the BMU neuron
	_matrix->updateWeightVector(1.0, currenLearningRate, inputVector, winnerNeuronX,
		winnerNeuronY);

	//Check which neurons should update its weight vector
	Neuron *bmu = _matrix->getNeuron(winnerNeuronX, winnerNeuronY);
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			currentNeuron = _matrix->getNeuron(row, col);
			distanceToNeuron = bmu->distanceToNeuron(currentNeuron);
			if(distanceToNeuron <= neighbourhoodRadius){
				// The current neuron is going to be updated

				// Get influence of the neuron based in its distance to BMU scientific method
				influence = getInfluence(distanceToNeuron, neighbourhoodRadius);

				// Update neuron
				_matrix->updateWeightVector(influence, currenLearningRate, inputVector,
					row, col);
			}
		}
	}

	_iterations++;
	if(_iterations == _totalSamples){
		_epochs++;
		_iterations = 0;
		if(_epochs%100 == 0)
			cout << "Epoch: " << _epochs << endl;
	}
}

void SelfOrganizingMaps::trainPlainCode(vector<double> inputVector){
	if(_epochs > _maxEpochs){
		return;
	}

	Neuron *currentNeuron = NULL;
	double distanceToBMU = 0.0;
	double influence = 0.0;
	double distance = 0;
	double winnerNeuronX = 0.0;
	double winnerNeuronY = 0.0;
	double gaussian = 0.0;
	double closestDistance = 3*(255.0 * 255.0);

	// Getting BMU
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			distance = _matrix->getNeuron(row, col)->distanceToInputVector(inputVector);
			if(distance < closestDistance){
				winnerNeuronX = row;
				winnerNeuronY = col;
				closestDistance = distance;
			}else if(distance == closestDistance){
				if(Utils::getRandomDoubleNumber() < 0.5){
					winnerNeuronX = row;
					winnerNeuronY = col;
				}
			}
		}
	}

	// Adjusting weigths of neurons
	// Get current Learning Rate
	double currenLearningRate = ((double)_epochs/_maxEpochs);
	if(currenLearningRate >= 1){
		currenLearningRate = 0.99;
	}
	currenLearningRate = 1 - currenLearningRate;

	// Update the BMU neuron
	_matrix->updateWeightVector(1.0, currenLearningRate, inputVector,
		winnerNeuronX, winnerNeuronY);

	//Check which neurons should update its weight vector
	Neuron *bmu = _matrix->getNeuron(winnerNeuronX, winnerNeuronY);
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			currentNeuron = _matrix->getNeuron(row, col);
			distanceToBMU = bmu->distanceToNeuron(currentNeuron);
			// The current neuron is going to be updated
			
			// Get influence of the neuron based in its distance to BMU scie
			influence = exp(-(distanceToBMU/(_initialNeighbourhoodRadius -
				(_epochs/_radiusTimeConstant))));
			
			// Update neuron
			_matrix->updateWeightVector(influence, currenLearningRate,
				inputVector, row, col);
		}
	}

	_iterations++;
	if(_iterations == _totalSamples){
		_epochs++;
		_iterations = 0;
		cout << "Epoch: " << _epochs << endl;
	}
}

void SelfOrganizingMaps::trainSegmentedFunctions(vector<double> inputVector){
	if(_epochs > _maxEpochs){
		return;
	}

	Neuron *currentNeuron = NULL;
	double distanceToBMU = 0.0;
	double influence = 0.0;

	Neuron *bmu = getBMU(inputVector);
	
	updateMatrixWeigths(bmu, inputVector);

	_iterations++;
	if(_iterations == _totalSamples){
		_epochs++;
		_iterations = 0;
		cout << "Epoch: " << _epochs << endl;
	}
}

/*
* The SOM algorithm uses de exponential decay function, for getting
* the learning rate.
* Over time the learning rate will be reduced close to 0.
*/
double SelfOrganizingMaps::getCurrenLearningRate(){
	//return _initialLearningRate * exp(-(double)_epochs/_learningRateTimeConstant);
	return ((double)_epochs/_maxEpochs);
}

void SelfOrganizingMaps::evaluateIndependentVector(vector<double> inputVector){
	Neuron *bmu = getBMU(inputVector);
	_matrix->getNeuron(bmu->getX(), bmu->getY())->setNeuronColor(0,0,0);
/*
	cout << "A _bmuTestCases size = " << _bmuTestCases.size() << endl;
	_bmuTestCases.insert ( pair<Neuron *,Neuron *>(getBMU(inputVector),getBMU(inputVector)));
	cout << "B _bmuTestCases size = " << _bmuTestCases.size() << endl;
	cout << "Input Vector:" << endl;
	cout << inputVector[0] << " " << inputVector[1] << " " << inputVector[2] << endl;
	cout << "BMU:" << endl;
	bmu->info();
	
	double distanceToBMU = bmu->distanceToInputVector(inputVector);
	cout << "BMU distance to input vector: " << distanceToBMU << endl;
	int bmuX = bmu->getX();
	int bmuY = bmu->getY();

	if(((bmuX + 1 < _size) && (bmuX - 1 >= 0)) && ((bmuY + 1 < _size) && (bmuY - 1 >= 0))){
		Neuron *upLeft = _matrix->getNeuron(bmuX - 1, bmuY - 1);
		Neuron *up = _matrix->getNeuron(bmuX, bmuY - 1);
		Neuron *upRight = _matrix->getNeuron(bmuX + 1, bmuY - 1);
		Neuron *left = _matrix->getNeuron(bmuX - 1, bmuY);
		Neuron *right = _matrix->getNeuron(bmuX + 1, bmuY);
		Neuron *downLeft = _matrix->getNeuron(bmuX - 1, bmuY + 1);
		Neuron *down = _matrix->getNeuron(bmuX, bmuY + 1);
		Neuron *downRight = _matrix->getNeuron(bmuX + 1, bmuY + 1);

		double distUpLeft =  upLeft->distanceToInputVector(inputVector);
		double distUp = up->distanceToInputVector(inputVector);
		double distUpRight = upRight->distanceToInputVector(inputVector);
		double distLeft = left->distanceToInputVector(inputVector);
		double distRight = right->distanceToInputVector(inputVector);
		double distDownLeft = downLeft->distanceToInputVector(inputVector);
		double distDown = down->distanceToInputVector(inputVector);
		double distDownRight = downRight->distanceToInputVector(inputVector);

		cout << "Distances to side Neurons of the BMU" << endl;
		cout << distUpLeft << " " << distUp << " " << distUpRight << endl;
		cout << distLeft << "--" << distanceToBMU << "-- " << distRight << endl;
		cout << distDownLeft << " " << distDown << " " << distDownRight << endl; 
	}else{
		cout << "The BMU is in the borders" << endl;
	}
*/
}

void SelfOrganizingMaps::evaluateIndependentRGBDataSet(vector<RGB *> inputDataset, int sigmaMultiplier){
	vector<double> weights;
	vector<double> distances;
	vector<Neuron *> bmus;
	double distance;
	double totalDistance;
	double average;
	double variance;
	double stdDeviation;
	double sigma;
	double lowerRange;
	double upperRange;
	int errors = 0;
	weights.resize(3);
	
	for(int i=0; i<inputDataset.size(); i++){
		weights[0] = inputDataset[i]->getRed();
		weights[1] = inputDataset[i]->getGreen();
		weights[2] = inputDataset[i]->getBlue();

		Neuron *bmu = getBMU(weights);
		distance = bmu->distanceToInputVector(weights);
		distances.push_back(distance);
		bmus.push_back(bmu);
		totalDistance += distance;
	}

	average = totalDistance/inputDataset.size();
	variance = Utils::getVariance(distances, average);
	stdDeviation = sqrt(variance);
	sigma = sigmaMultiplier*stdDeviation;
	lowerRange = average - sigma;
	upperRange = average + sigma;

	for(int i=0; i<inputDataset.size();i++){
		distance = distances[i];
		Neuron *bmu = bmus[i];
		if(distance < lowerRange || distance > upperRange){
			_matrix->getNeuron(bmu->getX(), bmu->getY())->setNeuronColor(255,0,0);
			errors++;
			cout << "Error out of " << sigmaMultiplier << " sigma" << endl;
		}else{
			_matrix->getNeuron(bmu->getX(), bmu->getY())->setNeuronColor(0,0,0);
			cout << "OK" << endl;
		}
	}
	cout << "Dataset results for " << sigmaMultiplier << " sigma " << errors;
	cout << "/" << inputDataset.size() << endl;
}

void SelfOrganizingMaps::evaluateIndependentDataChuckDataSet(
	vector<DataChunck *> inputDataset, int sigmaMultiplier,
	int totalNeuronsToEvaluate, double red, double green, double blue,
	int evaluatedIdUser){

	vector<double> weights;
	vector<double> distances;
	vector<Neuron *> bmus;

	int errors = 0;
	int totalElements = inputDataset.size();
	int initialIndex = 0;
	int finalIndex =0;
	int globalError = 0;

	double distance = 0.0;
	double totalDistance = 0.0;
	double average = 0.0;
	double variance = 0.0;
	double stdDeviation = 0.0;
	double sigma = 0.0;
	double lowerRange = 0.0;
	double upperRange = 0.0;
	double globalPercentileError = 0;

	weights.resize(3);

	// Get initial and final index of the iteration
	{
		initialIndex = rand() / totalElements;
		while((initialIndex + totalNeuronsToEvaluate) >= totalElements){
			initialIndex = rand() / totalElements;
		}
		finalIndex = initialIndex + totalNeuronsToEvaluate;
	}

	/*
	* Getting BMU of the dataset
	* Applying changes to get neuron stadistics
	*/
	{
		for(int i=initialIndex; i<finalIndex; i++){
			weights[0] = inputDataset[i]->getProportionTCPUDPMetric();
			weights[1] = inputDataset[i]->getBytesToInternalIPMetric();
			weights[2] = inputDataset[i]->getBytesThroughWebMetric();

			Neuron *bmu = getBMU(weights);
			distance = bmu->distanceToInputVector(weights);
			distances.push_back(distance);
			bmus.push_back(bmu);
			totalDistance += distance;

			// Applying bmu neuron stadistics
			bmu -> processNeuronAfterEvaluation(evaluatedIdUser);
		}
	}

	/*
	* Retrieving stadistical information
	*/
	{
		average = totalDistance/totalElements;
		variance = Utils::getVariance(distances, average);
		stdDeviation = sqrt(variance);
		sigma = sigmaMultiplier * stdDeviation;
		lowerRange = average - sigma;
		upperRange = average + sigma;
	}

	/*
	* Setting on matrix an indicator of BMU's matching neurons
	* if matrix are in the correct range, are marked as the color that was assigned if not
	* marked on black
	*/
	{
		for(int i=0; i<bmus.size();i++){
			distance = distances[i];
			Neuron *bmu = bmus[i];
			if(distance < lowerRange || distance > upperRange){
				_matrix->getNeuron(bmu->getX(), bmu->getY())->setNeuronColor(0, 0, 0);
				errors++;
			}else{
				_matrix->getNeuron(bmu->getX(), bmu->getY())->setNeuronColor(red, green, blue);
			}
		}
	}
}

void SelfOrganizingMaps::getMatrixStadistics(int totalNeuronsToEvaluate){
	int totalPositiveColitions = 0;
	int totalNegativeColitions = 0;
	int correct = 0;
	int incorrect = 0;
	map<int, int> totalBMU;
	Neuron *neuron;

	_errorStadisticsResults.clear();
	_correctStadisticsResults.clear();

	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			neuron = _matrix->getNeuron(row, col);
			if(neuron->isEvaluated()){
				_totalPositiveColitions += neuron->getPositiveColitions();
				_totalNegativeColitions += neuron->getNegativeColitions();
				if(!neuron->userMatches()){
					_incorrect++;
					errorStadisticsResults(neuron->getConstructedIdUser(), neuron->getEvaluatedIdUser());
				}else if(neuron->userMatches()){
					_correct++;
					correctStadisticsResults(neuron->getConstructedIdUser(), neuron->getEvaluatedIdUser());
				}
			}
		}
	}

	// Print Results
	cout << "Total Positive Colitions: " << _totalPositiveColitions << endl;
	cout << "Total Negative Colitions: " << _totalNegativeColitions << endl;
	
	cout << "Correct:" << _correct << endl;
	map<int, StadisticsResults *>::iterator ite;
	for(ite = _correctStadisticsResults.begin(); ite != _correctStadisticsResults.end(); ite++){
		cout << ite->second->info() << endl;
	}

	cout << "Errors:" << _incorrect << endl;
	map<int, StadisticsResults *>::iterator it;
	for(it = _errorStadisticsResults.begin(); it != _errorStadisticsResults.end(); it++){
		cout << it->second->info() << endl;
	}

	if((_totalPositiveColitions + _totalNegativeColitions +
		_incorrect + _correct) == totalNeuronsToEvaluate){
		cout << "Correct stadistic result" << endl;
	}else{
		cout << "Incorrect stadistic result" << endl;
	}
}

void SelfOrganizingMaps::resetMatrixStadistics(){
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			_matrix->getNeuron(row, col)->resetNeuronStatics();
		}
	}

	_incorrect = 0;
	_correct = 0;
	_totalPositiveColitions = 0;
	_totalNegativeColitions = 0;
	_errorStadisticsResults.clear();
	_correctStadisticsResults.clear();
}

void SelfOrganizingMaps::errorStadisticsResults(int initial, int final){
	int key = initial + final;
	if(_errorStadisticsResults.find(key) == _errorStadisticsResults.end()){
		StadisticsResults *stadisticalResult = new StadisticsResults(initial, final);
		_errorStadisticsResults[key] = stadisticalResult;
	}else{
		_errorStadisticsResults[key]->addToValue(1);
	}
}

void SelfOrganizingMaps::correctStadisticsResults(int initial, int final){
	int key = initial + final;
	if(_correctStadisticsResults.find(key) == _correctStadisticsResults.end()){
		StadisticsResults *stadisticalResult = new StadisticsResults(initial, final);
		_correctStadisticsResults[key] = stadisticalResult;
	}else{
		_correctStadisticsResults[key]->addToValue(1);
	}
}

void SelfOrganizingMaps::setWeightVector(vector<double> weightVector, int x, int y){
	_matrix->updateWeightVector(weightVector, x, y);
}

void SelfOrganizingMaps::setNeuron(Neuron *neuron){
	_matrix->setNeuron(neuron);
}

// OpenGL needed functions

void SelfOrganizingMaps::displayUsingWeigths(){
	vector<double> weigths;
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			if( _bmuTestCases.find( _matrix->getNeuron(row,col) ) == _bmuTestCases.end() ){
				weigths = _matrix->getNeuron(row, col)->getWeights();
				glColor3f(weigths[0], weigths[1], weigths[2]);
			}else{
				glColor3f(0,0,0);
			}
			glBegin(GL_QUADS);
				glVertex3f(row, col, 0);			// upper left
				glVertex3f(row, col-1, 0);			// lower left
				glVertex3f(row+1, col-1, 0);		// lower right
				glVertex3f(row+1, col, 0);			// upper right
			glEnd();
		}
	}
}

void SelfOrganizingMaps::displayUsingNeuronColor(){
	vector<double> weigths;
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			RGB *neuronColor = _matrix->getNeuron(row, col)->getNeuronColor();
			glColor3f(neuronColor->getRed(), neuronColor->getGreen(), neuronColor->getBlue());
			glBegin(GL_QUADS);
				glVertex3f(row, col, 0);			// upper left
				glVertex3f(row, col-1, 0);			// lower left
				glVertex3f(row+1, col-1, 0);		// lower right
				glVertex3f(row+1, col, 0);			// upper right
			glEnd();
		}
	}
}

void SelfOrganizingMaps::reset(){
	_bmuTestCases.clear();
	_matrix =  new Matrix(_size, _totalWeigths, true);
	_iterations = 0;
	_epochs = 0;
}


// Private methods
/*
* The SOM algorithm uses de exponential decay function, for getting
* the influence radius once the BMU is obtained.
* Over time the neighbourhood will shrink to the size of just one node.
*/
double SelfOrganizingMaps::getNeighbourhoodRadius(){
	return _initialNeighbourhoodRadius * exp(-(double)_epochs/_radiusTimeConstant);
}

/*
* Determines the influence that the input vector will have in a neuron
* based in its distance.
*/
double SelfOrganizingMaps::getInfluence(double distanceToBMU, double radius){
	double exponent = - ((distanceToBMU * distanceToBMU) / 2 * (radius * radius));
	return exp(exponent);
}

Neuron* SelfOrganizingMaps::getBMU(vector<double> inputVector){
	double distance = 0.0;
	double closestDistance = 3*(255.0 * 255.0);
	int winnerNeuronX = 0;
	int winnerNeuronY = 0;
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			distance = _matrix->getNeuron(row, col)->distanceToInputVector(inputVector);
			if(distance < closestDistance){
				winnerNeuronX = row;
				winnerNeuronY = col;
				closestDistance = distance;
			}else if(distance == closestDistance){
				if(Utils::getRandomDoubleNumber() < 0.5){
					winnerNeuronX = row;
					winnerNeuronY = col;
				}
			}
		}
	}
	return _matrix->getNeuron(winnerNeuronX, winnerNeuronY);
}

void SelfOrganizingMaps::updateMatrixWeigths(Neuron *bmu, vector<double> inputVector){
	Neuron *currentNeuron = NULL;
	double distanceToBMU = 0.0;
	double influence = 0.0;

	// Adjusting weigths of neurons
	// Get current Learning Rate
	double currenLearningRate = getCurrenLearningRate();
	if(currenLearningRate >= 1){
		currenLearningRate = 0.99;
	}
	currenLearningRate = 1 - currenLearningRate;

	// Update the BMU neuron
	_matrix->updateWeightVector(1.0, currenLearningRate, inputVector,
		bmu->getX(), bmu->getY());

	//Check which neurons should update its weight vector
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			currentNeuron = _matrix->getNeuron(row, col);
			distanceToBMU = bmu->distanceToNeuron(currentNeuron);
			// The current neuron is going to be updated
			
			// Get influence of the neuron based in its distance to BMU scie
			influence = exp(-(distanceToBMU/(_initialNeighbourhoodRadius -
				(_epochs/_radiusTimeConstant))));
			
			// Update neuron
			_matrix->updateWeightVector(influence, currenLearningRate,
				inputVector, row, col);
		}
	}
}