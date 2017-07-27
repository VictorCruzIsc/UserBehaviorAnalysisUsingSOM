#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <string>
#include <map>

#include "RGB.h"
#include "DataPackage.h"
#include "DataChunck.h"

using namespace std;

class Neuron{
	public:
		Neuron(int x, int y, int totalWeights);
		Neuron(int x, int y, vector<double> weigths);
		Neuron(int x, int y, int totalWeights, RGB* rgb);
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
		int getPositiveColitions();
		int getNegativeColitions();

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

		void info();
		string exportNeuron();
		double distanceToNeuron(Neuron *neuron);
		double distanceToInputVector(vector<double> inputVector);
		void processNeuronAfterEvaluation(int idUser);

		map<int,int> getEvaluationResults(){ return _evaluationResults; }
		int getTotalMatches();
		void clearEvaluationResults(){ _evaluationResults.clear(); }

		void increaseEvaluationResults(int matchingId);
		int getIdleResultsMatch(int idleId);
		int getCorrectResultMatch();
		int getIncorrectResultMatch();
		int getMatches(int matchingId);



	private:
		int 			_x;
		int				_y;
		int				_totalWeights;
		int 			_constructedIdUser;
		int 			_evaluatedIdUser;
		int 			_positiveColitions;
		int 			_negativeColitions;
		bool			_evaluated;
		bool			_matchingUser;
		vector<double> 	_weights;
		RGB 			*_neuronColor;
		map<int, int>	_evaluationResults;

		// Methods
		void initializeNeuron();
};
#endif