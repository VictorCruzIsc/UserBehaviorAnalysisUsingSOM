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

void SelfOrganizingMaps::trainSegmentedFunctions(vector<double> inputVector){
	if(_epochs > _maxEpochs){
		return;
	}

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

/*
* Evaluates a complete data set in order to get statistical information, through
* getting BMU's on a "colcha" to identify which neurons math it's creation user
* and which are not able to do so.
* Also setting to each neuron which user they were confused to.
*/
void SelfOrganizingMaps::evaluateIndependentDataChuckDataSet(
	vector<DataChunck *> inputDataset, int sigmaMultiplier,
	int totalNeuronsToEvaluate, double red, double green, double blue,
	int evaluatedIdUser){

	int totalElements = inputDataset.size();
	if(totalElements == 0){
		cout << "SOM totalElements = 0 " << endl;
		return;
	}

	int errors = 0;
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

	vector<double> weights;
	vector<double> distances;
	vector<Neuron *> bmus;

	weights.resize(3);

	// Set seed for getting a real random number
	srand(time(NULL));
	if(totalElements >= totalNeuronsToEvaluate){
		// Normal execution
		// Get initial and final index of the iteration
		//cout << "Normal execution" << endl;

		{
			initialIndex = rand() % totalElements;
			while((initialIndex + totalNeuronsToEvaluate) >= totalElements){
				initialIndex = rand() % totalElements;
			}

			finalIndex = initialIndex + totalNeuronsToEvaluate;
		}

		/*
		* Getting a BMU for every element of the dataset
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
				// Each neuron is created with an unique user, this function
				// check if the neuron creation user matches the evaluated user
				// and also check if the neuron was previously selected as a BMU
				// to get the number of positive and negative collitions
				bmu -> processNeuronAfterEvaluation(evaluatedIdUser);
			}
		}
	}else{
		// cout << "Special execution" << endl;
		srand(time(NULL));
		int index = 0;
		for(int i=0; i<totalNeuronsToEvaluate; i++){
			index = rand() % totalElements;

			weights[0] = inputDataset[index]->getProportionTCPUDPMetric();
			weights[1] = inputDataset[index]->getBytesToInternalIPMetric();
			weights[2] = inputDataset[index]->getBytesThroughWebMetric();

			Neuron *bmu = getBMU(weights);
			distance = bmu->distanceToInputVector(weights);
			distances.push_back(distance);
			bmus.push_back(bmu);
			totalDistance += distance;

			// Applying bmu neuron stadistics
			// Each neuron is created with an unique user, this function
			// check if the neuron creation user matches the evaluated user
			// and also check if the neuron was previously selected as a BMU
			// to get the number of positive and negative collitions
			bmu -> processNeuronAfterEvaluation(evaluatedIdUser);
		}
	}

	/*
	* Retrieving statistical information
	*/
	{
		average = totalDistance/totalElements;
		if(average == 0){
			cout << "SOM average causing NAN" << endl;
		}
		

		variance = Utils::getVariance(distances, average);
		if(variance == 0){
			cout << "SOM variance causing NAN" << endl;
			variance = 0.1;
		}

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
		int totalBMUS = bmus.size();
		for(int i=0; i<totalBMUS; i++){
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

/*
* Get's an overview of the status of the complete matrix once it got evaluated
*/
SamplesResult* SelfOrganizingMaps::getMatrixStadistics(int samples, int totalUsersEvaluated, int sigma,
	vector<int> &userIds){
	int totalPositiveColitions = 0;
	int totalNegativeColitions = 0;
	int correct = 0;
	int incorrect = 0;
	int totalNeuronsToEvaluate = (samples * totalUsersEvaluated);
	bool match = false;
	bool correctStadisticResult = false;
	Neuron *neuron;

	_errorStadisticsResults.clear();
	_correctStadisticsResults.clear();

	// Iterates all over the matrix to get the postive and negative collitions,
	// evalaute how many neutons match the correct user and how many dont
	// also create a map to define to which user each neurons was mistaken or correct
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			neuron = _matrix->getNeuron(row, col);
			if(neuron->isEvaluated()){
				_totalPositiveColitions += neuron->getPositiveColitions();
				_totalNegativeColitions += neuron->getNegativeColitions();
				match = neuron->userMatches();
				match ? _correct++ : _incorrect++; 

				// Defines to which user the current neuron was mistaken or matches correctly
				setAnStadisticResult(neuron->getConstructedIdUser(), neuron->getEvaluatedIdUser(), match);
			}
		}
	}

	// Verifies that teh results of the different variables math and are the same of the evaluated neurons
	correctStadisticResult = ((_totalPositiveColitions + _totalNegativeColitions +
		_incorrect + _correct) == totalNeuronsToEvaluate);

	if(!correctStadisticResult){
		cout << "ERROR: Incorrect stadistic result" << endl;
		return NULL;
	}

	SamplesResult *samplesResult =  new SamplesResult(samples, totalUsersEvaluated, sigma, _totalPositiveColitions,
		_totalNegativeColitions, _correct, _incorrect, _errorStadisticsResults,
		_correctStadisticsResults, correctStadisticResult, userIds);

	return samplesResult;
}

int SelfOrganizingMaps::getTotalNeuronsEvaluated(){
	int matches = 0;
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			Neuron *neuron = _matrix->getNeuron(row, col);
			if(neuron->isEvaluated()){
				matches += neuron->getTotalMatches();
			}
		}
	}
	return matches;
}

int SelfOrganizingMaps::getMatchesByUser(int idUser){
	int matches = 0;
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			Neuron *neuron = _matrix->getNeuron(row, col);
			if(neuron->isEvaluated() &&
				neuron->getConstructedIdUser() == idUser){
				matches += neuron->getMatches(idUser);
			}
		}
	}
	return matches;
}

vector<int> SelfOrganizingMaps::getNegativeMatchesByUser(int idUser){
	int totalNegativeMatches = 0;
	int negativeMatches = 0;
	int idleMatches = 0;
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			Neuron *neuron = _matrix->getNeuron(row, col);
			if(neuron->isEvaluated() &&
				neuron->getConstructedIdUser() != idUser){
				int localMatches = neuron->getMatches(idUser);
				totalNegativeMatches += localMatches;

				if(neuron->getConstructedIdUser() == 6){
					idleMatches += localMatches;
				}else{
					negativeMatches += localMatches;
				}
			}
		}
	}
	vector<int> data;
	data.push_back(idleMatches);
	data.push_back(negativeMatches);
	data.push_back(totalNegativeMatches);
	return data;
}

int SelfOrganizingMaps::getIdleMatchesByUser(int idUser, int idIdleUser){
	int matches = 0;
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			Neuron *neuron = _matrix->getNeuron(row, col);
			if(neuron->isEvaluated() &&
				neuron->getConstructedIdUser() == idIdleUser){
				matches += neuron->getMatches(idUser);
			}
		}
	}
	return matches;
}

int SelfOrganizingMaps::getNegativeMatchesByTrainedLattice(){
	int incorrect = 0;
	for(int row=0; row<_size; row++){
		for(int col=0; col<_size; col++){
			Neuron *neuron = _matrix->getNeuron(row, col);
			if(neuron->isEvaluated()){
				incorrect += neuron->getNegativeColitions();
			}
		}
	}
	return incorrect;
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

/*
* Defines to which user the current neuron was mistaken or matches correctly
* Example:
* The matrix was created with the users 4,5,6,7
* Each neuron after evaluation has the creator user id, and the matching user id, if
* both ids are the same the neuron matches correctly, otherwise it was mistaken.
* In both cases it is needed to know which user id the neuron match
* Initial and final represent the creator user id and the matching user id after evaluation
* the correct defines if the neuron matched correctly it's ccreator user or not.
* In order to have a complete stadistic, the idea to get how many neurons of one creator user id
* match with other user id, so two maps are created, one for correct and one for incorrect neurons
* where the key is defined defined by the sum of initial and final
* Example:
* Creators users are 3, 4, 5, 6
* A neuron was created by user 3 and matches user 3 the key is 6 and the value stored is one in the
* map of the correct, at the the correct map will only have as many keys as evaluated users.
* Another neuron is created the by user 5 but matches user 6 the key is 11 and the value stores is 1
* another neuron is created the by user 6 but matches user 5 the key is also 11 and the value now is 2
* both entries are stored in the incorrect map.
*
* So by the example you can see that is is possible to know which user is getting similar to other
* and which users are completly correct
*
* The following data are the keys created for the example
*
* Created user	Matching user 	Created key
		3			3				6			[Correct map]
		3			4				7			[Incorrect map]
		3			5				8			[Incorrect map]
		3			6				9			[Incorrect map]
		3			7				10			[Incorrect map]
		4			3				7			[Incorrect map] and as it was previously created in the map, increments map value in 1
		4			4				8			[Correct map]
		4			5				9			[Incorrect map] and as it was previously created in the map, increments map value in 1
		4			6				10			[Incorrect map] and as it was previously created in the map, increments map value in 1
		4			7				11			[Incorrect map]
		5			3				8			[Incorrect map] and as it was previously created in the map, increments map value in 1
		5			4				9			[Incorrect map] and as it was previously created in the map, increments map value in 1
		5			5				10			[Correct map]
		5			6				11			[Incorrect map] and as it was previously created in the map, increments map value in 1
		5			7				12			[Incorrect map]
		6			3				9			[Incorrect map] and as it was previously created in the map, increments map value in 1
		6			4				10			[Incorrect map] and as it was previously created in the map, increments map value in 1
		6			5				11			[Incorrect map] and as it was previously created in the map, increments map value in 1
		6			6				12			[Correct map]
		6			7				13			[Incorrect map]
		7			3				10			[Incorrect map] and as it was previously created in the map, increments map value in 1
		7			4				11			[Incorrect map] and as it was previously created in the map, increments map value in 1
		7			5				12			[Incorrect map] and as it was previously created in the map, increments map value in 1
		7			6				13			[Incorrect map] and as it was previously created in the map, increments map value in 1
		7			7				14			[Correct map]
*/
void SelfOrganizingMaps::setAnStadisticResult(int initial, int final, bool correct){
	map<int, StadisticsResults *> *stadisticsResults = &_correctStadisticsResults;
	if(!correct){
		stadisticsResults = &_errorStadisticsResults;
	}
	int key = initial + final;
	if(stadisticsResults->find(key) == stadisticsResults->end()){
		StadisticsResults *stadisticalResult = new StadisticsResults(initial, final);
		(*stadisticsResults)[key] = stadisticalResult;
	}else{
		(*stadisticsResults)[key]->addToValue(1);
	}	
}

void SelfOrganizingMaps::setWeightVector(vector<double> weightVector, int x, int y){
	_matrix->updateWeightVector(weightVector, x, y);
}

void SelfOrganizingMaps::setNeuron(Neuron *neuron){
	_matrix->setNeuron(neuron);
}

// OpenGL needed functions
void SelfOrganizingMaps::displayUsingNeuronColor(){
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