#ifndef CLASSES_H
#define CLASSES_H

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <ctime>
#include <sstream>
#include <map>
#include <limits>
#include <math.h>
#include <cfloat>
#include <GL/glut.h>
#include <algorithm>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <iterator>
#include <regex>

#include "Matrix.h"
#include "DataPackage.h"
#include "DataChunck.h"

//#define DEBUG
//#define DEBUG_DATCHUNCK

using namespace std;

class Utils;
class SelfOrganizingMaps;
class DataSet;

class Utils{
	public:
		static const int BUILD = 1;
		static const int TRAIN = 2;
		static const int EVALUATE = 3;

		static int getRandomDoubleNumber();
		static void exportMatrixToFile(Matrix *matrix, int completedEpochs,
			int maxEpochs, double initialLearningRate,
			double finalLearningRate);
		static Matrix* importMatrixFromFile(char *fileName);
		static SelfOrganizingMaps* importSOMFromFiles(char *fileName);
		static SelfOrganizingMaps* importSOMFromFiles(vector<char *> fileNames,
			int matrixComposition, int totalFiles);

		// DataSets
		static vector<vector<double> > createColorDataSet(int dataSetSize, int totalWeigths);
		static vector<vector<double> > createColorTestDataSet();

		// Build Matrix Datasets
		static vector<RGB* > createBlueColorBuildMatrixDataSet();
		static vector<RGB* > createRedColorBuildMatrixDataSet();
		static vector<RGB* > createGreenColorBuildMatrixDataSet();
		static vector<RGB* > createYellowColorBuildMatrixDataSet();
		static vector<RGB* > createMultipleColorBuildMatrixDataSet();

		// Train Matrix Datasets
		static vector<RGB* > createBlueColorTrainMatrixDataSet();
		static vector<RGB* > createRedColorTrainMatrixDataSet();
		static vector<RGB* > createGreenColorTrainMatrixDataSet();
		static vector<RGB* > createYellowColorTrainMatrixDataSet();
		static vector<RGB* > createMultipleColorTrainMatrixDataSet();

		// Test Matrix Datasets
		static vector<RGB* > createBlueColorTestMatrixDataSet();
		static vector<RGB* > createRedColorTestMatrixDataSet();
		static vector<RGB* > createGreenColorTestMatrixDataSet();
		static vector<RGB* > createYellowColorTestMatrixDataSet();


		// Test BMU on Matrix Datasets

		// Time functions
		static string currentTime();
		static string currentDate();

		// Others
		static string buildFileName();
		static double getVariance(vector<double> samples, double average);
};

class SelfOrganizingMaps{
	public:
		// Create SOM completely from scratch
		SelfOrganizingMaps(int size, int totalWeights, int maxEpochs,
			double learningRate, int totalSamples, bool initialize);
		// Create SOM from a specific color dataset
		SelfOrganizingMaps(int size, int totalWeights,
			int maxEpochs, double initialLearningRate, vector<RGB* > dataSet,
			int totalSamples);
		// Create SOM from a Matrix already trained
		SelfOrganizingMaps(int size, int totalWeights,
			int maxEpochs, double initialLearningRate, Matrix *matrix,
			int totalSamples);
		SelfOrganizingMaps(int size, int totalWeights,
			int maxEpochs, double initialLearningRate, vector<DataPackage* > dataSet,
			int totalSamples);
		SelfOrganizingMaps(int size, int totalWeights,
			int maxEpochs, double initialLearningRate, vector<DataChunck* > dataSet,
			int totalSamples, int idUser);
		~SelfOrganizingMaps();
		int getIterations();
		int getEpochs();
		int getSize();
		int getTotalWeights();
		Matrix* getMatrix();

		// Setters
		void setSize(int size){_size = size;}
		void setEpochs(int epochs){_epochs = epochs;}
		void setMaxEpochs(int maxEpochs){_maxEpochs = maxEpochs;}
		void setInitialLearningRate(double initialLearningRate){
			_initialLearningRate = initialLearningRate;
		}
		void setTotalWeights(int totalWeights){_totalWeigths =  totalWeights;}
		void setNeuron(Neuron *neuron);

		// Main functionality of the algorithm
		void train(vector<double> inputVector);
		void trainPlainCode(vector<double> inputVector);
		void trainSegmentedFunctions(vector<double> inputVector);
		double getCurrenLearningRate();

		// OpenGL needed functions
		void displayUsingWeigths();
		void displayUsingNeuronColor();
		void reset();

		// Testing cases
		void evaluateIndependentVector(vector<double> inputVector);
		void evaluateIndependentRGBDataSet(vector<RGB *> inputDataset,
			int sigmaMultiplier);
		void evaluateIndependentDataChuckDataSet(vector<DataChunck *> inputDataset,
			int sigmaMultiplier, int iterations, int chunckSize);
		void evaluateIndependentDataChuckDataSet(vector<DataChunck *> inputDataset,
			int sigmaMultiplier, int iterations, int chunckSize, double red,
			double green, double blue);

		// Import trained matrix
		void setWeightVector(vector<double> weightVector, int x, int y);

	private:
		int _iterations;
		int _maxEpochs;
		int _size;
		int _totalWeigths;
		int _totalSamples;
		int _epochs;
		double _initialLearningRate;
		double _initialNeighbourhoodRadius;
		double _radiusTimeConstant;
		double _learningRateTimeConstant;
		Matrix *_matrix;
		map<Neuron *, Neuron *> _bmuTestCases;

		// Methods
		// Main functionality util  methods
		double getNeighbourhoodRadius();
		double getInfluence(double distanceToBMU, double radius);
		Neuron* getBMU(vector<double> inputVector);
		void updateMatrixWeigths(Neuron *bmu, vector<double> inputVector);
};

class DataSet{
	public:
		static vector<DataPackage *> createDataSetPackageFormat(string user, int type);
		static vector<DataChunck *> createDataSetDataChunckFormat(int idUser, int type,
			int chunckTimeSize, int chunckTimeInterval);
};

#endif