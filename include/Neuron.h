#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <string>

#include "RGB.h"
#include "DataPackage.h"
#include "DataChunck.h"

using namespace std;

class Neuron{
	public:
		Neuron(int x, int y, int totalWeights);
		Neuron(int x, int y, vector<double> weigths);
		Neuron(int x, int y, int totalWeights, RGB* rgb);
		Neuron(int x, int y, int totalWeights, DataPackage* dataPackage);
		Neuron(int x, int y, int totalWeights, DataChunck* dataChunck);
		~Neuron();

		int getX();
		int getY();
		int getTotalWeights();
		vector<double> getWeights();
		RGB *getNeuronColor();
		int getConstructedIdUser();
		int getEvaluatedIdUser();
		bool isEvaluated();
		bool userMatches();
		int getColitions();

		void setX(int x);
		void setY(int y);
		void setWeigths(vector<double> weigths);
		void setNeuronColor(double red, double green, double blue);
		void setNeuronColor(vector<double> rgb);
		void setRGB(RGB *rgb);
		void setNeuronStatics(int idUser);
		void resetNeuronStatics();
		void setNeuronStatics(int idUser, int evaluatedIdUser, bool evaluated,
			bool matchingUser);
		void setEvaluatedIdUser(int idUser);

		void info();
		string exportNeuron();
		double distanceToNeuron(Neuron *neuron);
		double distanceToInputVector(vector<double> inputVector);
		void processNeuronAfterEvaluation();

	private:
		int 			_x;
		int				_y;
		int				_totalWeights;
		int 			_constructedIdUser;
		int 			_evaluatedIdUser;
		int 			_colitions;
		bool			_evaluated;
		bool			_matchingUser;
		vector<double> 	_weights;
		RGB 			*_neuronColor;

		// Methods
		void initializeNeuron();
};
#endif