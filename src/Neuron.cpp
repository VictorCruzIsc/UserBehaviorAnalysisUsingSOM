#include "../include/Neuron.h"

Neuron::Neuron(int x, int y, int totalWeights) : _x(x), _y(y),
	_totalWeights(totalWeights){
	_weights.resize(_totalWeights);
	_neuronColor = new RGB();
	initializeNeuron();
}

Neuron::Neuron(int x, int y, vector<double> weigths) :
	_x(x), _y(y){
	_totalWeights = weigths.size();
	_weights.resize(_totalWeights);
	_neuronColor = new RGB();

	for(int i = 0; i < _totalWeights; i++){
		_weights[i] = weigths[i];
	}

	// Setting neuron initial color
	if(_totalWeights >= 3){
		_neuronColor->setRGB(_weights[0], _weights[1], _weights[2]);
	}
}

Neuron::Neuron(int x, int y, int totalWeights, RGB* rgb) :
	_x(x), _y(y), _totalWeights(totalWeights){
	_weights.resize(_totalWeights);
	_neuronColor = rgb;

	if(_totalWeights>=3){
		_weights[0] = rgb->getRed();
		_weights[1] = rgb->getGreen();
		_weights[2] = rgb->getBlue();
	}
}

Neuron::Neuron(int x, int y, int totalWeights, DataPackage* dataPackage) :
	_x(x), _y(y), _totalWeights(totalWeights){
	_weights.resize(_totalWeights);

	double destinationIp = dataPackage->getDestinationIp()/(double)255;
	double protocol = dataPackage->getProtocol()/(double)255;;
	double remotePort = dataPackage->getRemotePort()/(double)255;

	RGB* rgb = new RGB(destinationIp, protocol, remotePort);

	_neuronColor = rgb;

	if(_totalWeights>=3){
		_weights[0] = destinationIp;
		_weights[1] = protocol;
		_weights[2] = remotePort;
	}
}

Neuron::Neuron(int x, int y, int totalWeights, DataChunck* dataChunck) :
	_x(x), _y(y), _totalWeights(totalWeights){
	_weights.resize(_totalWeights);

	double metricTCPUDP = dataChunck->getProportionTCPUDPMetric() / (double)255;
	double metricInternalIP = dataChunck->getBytesToInternalIPMetric() / (double)255;
	double metricWeb = dataChunck->getBytesThroughWebMetric() / (double)255;

	double totalBytes = dataChunck->getTotalBytes();
	double totalTCP = dataChunck-> getTotalBytesThroughTCP();
	double totalUDP = dataChunck->getTotalBytesThroughUDP();

	RGB* rgb = new RGB(metricTCPUDP, metricInternalIP, metricWeb);

	_neuronColor = rgb;

	if(_totalWeights>=3){
		_weights[0] = metricTCPUDP;
		_weights[1] = metricInternalIP;
		_weights[2] = metricWeb;
	}
}

Neuron::~Neuron(){
	delete _neuronColor;
}

int Neuron::getX(){
	return _x;
}

int Neuron::getY(){
	return _y;
}

int Neuron::getTotalWeights(){
	return _totalWeights;
}

vector<double> Neuron::getWeights(){
	return _weights;
}

RGB *Neuron::getNeuronColor(){
	return _neuronColor;
}

void Neuron::setX(int x){
	_x = x;
}

void Neuron::setY(int y){
	_y = y;
}

void Neuron::setNeuronColor(double red, double green, double blue){
	_neuronColor->setRGB(red, green, blue);
}

void Neuron::setRGB(RGB *rgb){
	_neuronColor = rgb;
}

void Neuron::info(){
	cout << "Postion in matrix (" << _x << "," << _y << ") Total weigths: " << _totalWeights << "\n";
	for(int i=0; i<_totalWeights; i++){
		cout << "Weight " << i << ": " << _weights[i] << "\n";
	}
	cout << "\n";
}

string Neuron::exportNeuron(){
	string weigths = "" + to_string(_weights[0]);
	for(int i=1; i<_totalWeights; i++){
		weigths += " " + to_string(_weights[i]);
	}
	weigths += " " + to_string(_constructedIdUser);
	weigths += " " + to_string(_evaluatedIdUser);
	if(_evaluated){
		weigths += " " + to_string(1);
	}else{
		weigths += " " + to_string(0);
	}
	if(_matchingUser){
		weigths += " " + to_string(1);
	}else{
		weigths += " " + to_string(0);
	}
	weigths += "";
	return weigths;
}

/*
* Gets the euclidean distance from any neuron to this neuron
* This method is used to check if this neuron is going to be affected
* by the update of the weights
*/
double Neuron::distanceToNeuron(Neuron *neuron){
	double xDistance =  neuron->getX() - _x;
	xDistance *= xDistance;
	double yDistance = neuron->getY() - _y;
	yDistance *= yDistance;
	return xDistance + yDistance;
}

/*
* Gets the distance from an input vector with a total of N weights
* to this neuron.
* If the total weights of the input vector are not the same, the operation
* is not done, and the execution fails.
* This methos is used to get the BMU.
*/
double Neuron::distanceToInputVector(vector<double> inputVector){
	double totalDistance = 0.0;
	double currentWeight = 0.0;
	
	if(inputVector.size() != _totalWeights){
		// TODO:
		// Lanzar una excepcion de pesos diferentes
		cout << "ERROR: The total weights in the input vector differ from the ones in the neuron";
		return 1;
	}

	for(int i=0; i<_totalWeights; i++){
		//cout << inputVector[i] << " " << _weights[i] << endl; 
		currentWeight = inputVector[i] - _weights[i];
		currentWeight *= currentWeight;
		totalDistance += currentWeight;
	}

	return totalDistance;
}

void Neuron::setWeigths(vector<double> weigths){
	for(int i=0; i<_totalWeights; i++){
		_weights[i] = weigths[i];
	}
}

void Neuron::setNeuronColor(vector<double> rgb){
	if(_totalWeights>=3){
		_neuronColor->setRGB(rgb[0], rgb[1], rgb[2]);
	}
}

// Private methods

void Neuron::initializeNeuron(){
	for(int i=0; i<_totalWeights; i++){
		_weights[i] = (double)rand() / (double)RAND_MAX;
	}

	// Setting neuron initial color
	if(_totalWeights >= 3){
		_neuronColor->setRGB(_weights[0], _weights[1], _weights[2]);
	}
}

int Neuron::getConstructedIdUser(){
	return _constructedIdUser;
}

int Neuron::getEvaluatedIdUser(){
	return _evaluatedIdUser;
}

bool Neuron::isEvaluated(){
	return _evaluated;
}

bool Neuron::userMatches(){
	return _matchingUser;
}

int Neuron::getPositiveColitions(){
	return _positiveColitions;
}

int Neuron::getNegativeColitions(){
	return _negativeColitions;
}

void Neuron::setNeuronStatics(int idUser){
	_constructedIdUser = idUser;
	_evaluatedIdUser = 0;
	_positiveColitions = 0;
	_negativeColitions = 0;
	_evaluated = false;
	_matchingUser = false;
}

void Neuron::resetNeuronStatics(){
	_evaluatedIdUser = 0;
	_positiveColitions = 0;
	_negativeColitions = 0;
	_evaluated = false;
	_matchingUser = false;
}

void Neuron::setNeuronStatics(int idUser, int evaluatedIdUser, bool evaluated,
			bool matchingUser){
	_constructedIdUser = idUser;
	_evaluatedIdUser = evaluatedIdUser;
	_evaluated = evaluated;
	_matchingUser = matchingUser;
	_positiveColitions = 0;
	_negativeColitions = 0;
}

void Neuron::processNeuronAfterEvaluation(int idUser){
	_evaluatedIdUser = idUser;
	_matchingUser = (_constructedIdUser == _evaluatedIdUser);

	if(_evaluated){
		if(_matchingUser){
			_positiveColitions++;
		}else{
			_negativeColitions++;
		}
	}else{
		_evaluated = true;
	}
}