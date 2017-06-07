#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "include/functionality/Classes.h"

#define CHUNCKTIMESIZE 			5	// Given in minutes
#define CHUNCKTIMEINTERVAL 		10	// Given in seconds

// DataSet variables
vector<DataChunck *>			_buildDataChunckSet;
vector<DataChunck *>			_trainDataChunckSet;
vector<DataChunck *>			_evaluateDataChunckSet;

// Algorithm methods
void createUserDataSets(int idUser, int chunckTimeSize, int chunckTimeInterval, int version);

/*
* Initialize build, train and evaluate dataSets for a specific user
*/
void createUserDataSets(int idUser, int chunckTimeSize, int chunckTimeInterval, int version){
	cout << "Start building Build DataSet..." << endl;
	_buildDataChunckSet = DataSet::createDataSetDataChunckFormat(idUser, Utils::BUILD, chunckTimeSize, chunckTimeInterval, version);
	cout << "Finish building Build DataSet" << endl;

	cout << "Start building Train DataSet..." << endl;
	_trainDataChunckSet = DataSet::createDataSetDataChunckFormat(idUser, Utils::TRAIN, chunckTimeSize, chunckTimeInterval, version);
	cout << "Finish building Train DataSet" << endl;

	cout << "Start building Evaluation DataSet..." << endl;
	_evaluateDataChunckSet = DataSet::createDataSetDataChunckFormat(idUser, Utils::EVALUATE, chunckTimeSize, chunckTimeInterval, version);
	cout << "Finish building Evaluation DataSet" << endl;
}

int main(int argc, char **argv){
	for(int i=1; i<argc; i++){
		int user = atoi(argv[i]);
		cout << "Creating datasets for user " << user << endl;
		createUserDataSets(user, CHUNCKTIMESIZE, CHUNCKTIMEINTERVAL, 2);

		cout << "Exporting datasets for user " << user << endl;
		Utils::exportDataChuckDataSet(Utils::BUILD, user, _buildDataChunckSet);
		Utils::exportDataChuckDataSet(Utils::TRAIN, user, _trainDataChunckSet);
		Utils::exportDataChuckDataSet(Utils::EVALUATE, user, _evaluateDataChunckSet);

		_buildDataChunckSet.clear();
		_trainDataChunckSet.clear();
		_evaluateDataChunckSet.clear();
	}

	return 0;
}