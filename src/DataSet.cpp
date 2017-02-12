#include "../include/Classes.h"

vector<DataPackage *> DataSet::createDataSetPackageFormat(string user,
			int type){
	fstream infile;
	string fileNamePrefix = "flows-";
	string filesPath = "networkTrafficCaptures";
	string line;
	vector<string> packageElements;
	vector<DataPackage *> packageDataSet;

	// Get all names of files in the directory
	string regexPattern = "(flows)(.*)(.csv)";
	DIR *directory;
	struct dirent *file;
	regex validFile(regexPattern);

	#ifdef DEBUG
		int counter = 0;
	#endif

	switch(type){
		case Utils::BUILD: // Build
			filesPath += "BuildDataSet/";
			break;
		case Utils::TRAIN: // Train
			filesPath += "TrainDataSet/";
			break;
		case Utils::EVALUATE: // Evaluation
			filesPath += "EvaluationDataSet/";
			break;
	}

	filesPath += user;

	#ifdef DEBUG
		cout << "FilesPath: " << filesPath << endl;
	#endif

	// Refactor:
	// For each element under FilesPath get the data
	if(directory = opendir(filesPath.c_str())){ // Opening Directory
		while(file = readdir(directory)){ // There is  file
			string fileName = string(file->d_name);
			filesPath += "/" + fileName; // Complete route to the file
			if(regex_match(fileName, validFile)){ // Means it has format flows.*.csv
				#ifdef DEBUG
					cout << "Ento en la regex: " << fileName << endl;
				#endif
				infile.open(filesPath);
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
					if(!dataPackage->hasError()){
						packageDataSet.push_back(dataPackage);
					}

					packageElements.clear();

					#ifdef DEBUG
						counter++;
					#endif
				}

				infile.close();
			}
		}
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

vector<DataChunck *> DataSet::createDataSetDataChunckFormat(int idUser, int type,
	int chunckTimeSize, int chunckTimeInterval){

	string userNetworkTraffic =  "user" + to_string(idUser);

	vector<DataPackage*> rawPackages = DataSet::createDataSetPackageFormat(userNetworkTraffic,
		type);

	// cout << "CH1 totalRawPackages: " << rawPackages.size() << endl;

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