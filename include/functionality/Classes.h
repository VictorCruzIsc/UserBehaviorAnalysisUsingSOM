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

#include <algorithm>
#include <sys/types.h>
#include <dirent.h>
#include <iterator>
#include <regex>
#include <math.h>  

#include "Matrix.h"
#include "DataPackage.h"
#include "DataChunck.h"
#include "StadisticsResults.h"
#include "../results/SamplesResult.h"

//#define DEBUG
//#define DEBUG_DATCHUNCK
//#define DEBUG_STATICS

#define MAC

#ifdef MAC
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <OpenGL/glext.h>
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

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
		static void exportDataChuckDataSet(int type, int userId, vector<DataChunck *> &dataSet);
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
		static string buildFileNameDataSet(int type, int userId);
		static double getVariance(vector<double> samples, double average);
};

class SelfOrganizingMaps{
	public:
		// Create SOM completely from scratch
		SelfOrganizingMaps(int size, int totalWeights, int maxEpochs,
			double learningRate, int totalSamples, bool initialize);
		// Create SOM from a Matrix already trained
		SelfOrganizingMaps(int size, int totalWeights,
			int maxEpochs, double initialLearningRate, Matrix *matrix,
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
			int sigmaMultiplier, int chunckSize, double red,
			double green, double blue, int evaluatedIdUser);

		// Import trained matrix
		void setWeightVector(vector<double> weightVector, int x, int y);

		// Stadistics
		SamplesResult* getMatrixStadistics(int samples, int totalUsersEvaluated, int sigma,
			vector<int> &userIds);
		void resetMatrixStadistics();

		// Variables
		int _incorrect;
		int _totalPositiveColitions;
		int _totalNegativeColitions;
		int _correct;
		map<int, StadisticsResults *> _errorStadisticsResults;
		map<int, StadisticsResults *> _correctStadisticsResults;

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
		void setAnStadisticResult(int initial, int final, bool correct);
};

class DataSet{
	public:
		static vector<DataPackage *> createDataSetPackageFormat(string user, int type, int version);
		static vector<DataChunck *> createDataSetDataChunckFormat(int idUser, int type,
			int chunckTimeSize, int chunckTimeInterval, int version);
		static vector<DataChunck *> createDataSetDataChunckFormatFromFile(int idUser, int type);
};

#endif