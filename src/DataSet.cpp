#include "../include/Classes.h"

vector<DataPackage *> DataSet::createDataSetPackageFormat(string user,
			int type, int initial, int final){
	fstream infile;
	string fileNamePrefix = "flows-";
	string filesPath = "networkTrafficCaptures";
	string file;
	string line;
	vector<string> packageElements;
	vector<DataPackage *> packageDataSet;

#ifdef DEBUG
	int counter = 0;
#endif

	switch(type){
		case 0: // Build
			filesPath += "BuildDataSet/";
			break;
		case 1: // Train
			filesPath += "TrainDataSet/";
			break;
		case 2: // Evaluation
			filesPath += "EvaluationDataSet/";
			break;
	}

	filesPath += user + "/";

#ifdef DEBUG
	cout << "FilesPath: " << filesPath << endl;
#endif

	for(int i=initial; i<=final; i++){
		file += filesPath + fileNamePrefix + to_string(i) + ".csv";

#ifdef DEBUG
		cout << file << endl;
#endif

		infile.open(file);

		//while(getline(infile, line) && counter < 100000){
		while(getline(infile, line)){
			replace(line.begin(), line.end(), ',', ' ');

#ifdef DEBUG
			cout << " DataSet: " << counter << " " << line << endl;
#endif

			stringstream ssin(line);

			packageElements.resize(12);
        	
        	for(int i=0; i<12; i++){
				ssin >> packageElements[i];
			}

#ifdef DEBUG
			for(int i=0; i<12; i++){
				cout << packageElements[i] << endl;
			}
#endif

			DataPackage *dataPackage = new DataPackage(packageElements);
			if(!dataPackage->hasError())
				packageDataSet.push_back(dataPackage);
			packageElements.clear();
#ifdef DEBUG
			counter++;
#endif
		}

		infile.close();
		
		file = "";
	}

#ifdef DEBUG
		cout << "No more files to read" << endl;
		cout << packageDataSet.size() << endl;
		for(int i=0; i<packageDataSet.size(); i++){
			cout << i << ". " << packageDataSet[i]->getDeviceMAC() << "- "<< endl;
		}
#endif

	return packageDataSet;
}

vector<DataChunck *> DataSet::createDataSetDataChunckFormat(string user,
			int type, int initial, int final, int chunckTimeSize, int chunckTimeInterval){
	vector<DataPackage*> rawPackages = DataSet::createDataSetPackageFormat(user,
		type, initial, final);

	//cout << "CH1 totalRawPackages: " << rawPackages.size() << endl;

	vector<DataChunck*> dataChunckDataSet;

	int runningIndex = 0;
	int index = runningIndex;
	int rawPackagesDataSetSize = rawPackages.size();
	time_t runningTimeStamp;
	time_t intervalTimeStamp;
	time_t initialTimeStamp;
	time_t finalTimeStamp;


	while(index < rawPackagesDataSetSize){
		initialTimeStamp = rawPackages[runningIndex]->getOriginTimeStamp();
		finalTimeStamp = initialTimeStamp + (60 * chunckTimeSize); // chunckTimeSize is in minutes

#ifdef DEBUG_DATCHUNCK
		struct tm initial, final, differenceFormated;
		char initialBuffer[80], finalBuffer[80];
		initial = *localtime(&initialTimeStamp);
		final = *localtime(&finalTimeStamp);
		strftime(initialBuffer, sizeof(initialBuffer), "%H:%M:%S", &initial);
		strftime(finalBuffer, sizeof(finalBuffer), "%H:%M:%S", &final);

		cout << "A " << initialBuffer << " " << finalBuffer << endl;
#endif

		vector<DataPackage *> dataChunckComposition;
		dataChunckComposition.push_back(rawPackages[runningIndex]);

		runningIndex++;

		while(runningIndex < rawPackagesDataSetSize){
			runningTimeStamp = rawPackages[runningIndex]->getOriginTimeStamp();
			if(runningTimeStamp < finalTimeStamp)
				dataChunckComposition.push_back(rawPackages[runningIndex]);
			else
				break;

			runningIndex++;
		}

		DataChunck *dataChunck = new DataChunck(dataChunckComposition);
		dataChunckDataSet.push_back(dataChunck);

		// Setting new interval
		runningIndex = index;
		initialTimeStamp = rawPackages[runningIndex]->getOriginTimeStamp();
		intervalTimeStamp = initialTimeStamp + chunckTimeInterval; // chunckTimeInterval is in seconds

#ifdef DEBUG_DATCHUNCK
		initial = *localtime(&initialTimeStamp);
		final = *localtime(&intervalTimeStamp);
		strftime(initialBuffer, sizeof(initialBuffer), "%H:%M:%S", &initial);
		strftime(finalBuffer, sizeof(finalBuffer), "%H:%M:%S", &final);

		cout << "B " << initialBuffer << " " << finalBuffer << endl;
#endif

		while(runningIndex < rawPackagesDataSetSize){
			runningTimeStamp = rawPackages[runningIndex]->getOriginTimeStamp();
			if(runningTimeStamp < intervalTimeStamp){
				runningIndex++;
			}else{
				break;
			}
		}

		index = runningIndex;
	}
#ifdef DEBUG_DATCHUNCK
	cout << "CH Final:" << dataChunckDataSet.size() << endl;
#endif

	return dataChunckDataSet;
}