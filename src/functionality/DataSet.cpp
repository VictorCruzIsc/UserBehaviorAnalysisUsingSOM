#include "../../include/functionality/Classes.h"

vector<DataPackage *> DataSet::createDataSetPackageFormat(string user, int type, int version){
	fstream infile;
	string line;
	vector<string> packageElements;
	vector<DataPackage *> packageDataSet;
	string fileNamePrefix = "flows-";
	string filesPath = "networkTrafficCaptures";

	int elementsInFile = ((version == 1) ? 12 : 10);

	// Get all names of files in the directory
	string regexPattern = "(flows)(.*)(.csv)";
	DIR *directory;
	struct dirent *file;
	regex validFile(regexPattern);

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
			string filePath = filesPath + "/" + fileName; // Complete route to the file

			if(regex_match(fileName, validFile)){ // Means it has format flows.*.csv
				infile.open(filePath);
				if(infile.is_open()){

					#ifdef DEBUG
						cout << "File:" << fileName << " is OPENED" << endl;
					#endif

					while(getline(infile, line)){
						replace(line.begin(), line.end(), ',', ' ');
						stringstream ssin(line);
						packageElements.resize(elementsInFile);

						for(int i=0; i<elementsInFile; i++){
							ssin >> packageElements[i];
						}

						DataPackage *dataPackage = new DataPackage(packageElements);
						if(!dataPackage->hasError()){
							packageDataSet.push_back(dataPackage);
						}else{
							#ifdef DEBUG
								cout << "Error DataPackage not added" << endl;
							#endif
						}

						packageElements.clear();
					}

					infile.close();

				}else{
					cout << "File: " << fileName << " is CLOSED ";
					cout << "Path: " << filePath << endl;
				}
			}
		}
	}

	#ifdef DEBUG
		cout << "No more files to read" << endl;
		cout << "packageDataSet size: " << packageDataSet.size() << endl;
	#endif

	return packageDataSet;
}

vector<DataChunck *> DataSet::createDataSetDataChunckFormat(int idUser, int type,
	int chunckTimeSize, int chunckTimeInterval, int version){

	string userNetworkTraffic =  "user" + to_string(idUser);

	vector<DataPackage*> rawPackages = DataSet::createDataSetPackageFormat(userNetworkTraffic,
		type, version);

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

vector<DataChunck *> DataSet::createDataSetDataChunckFormatFromFile(int idUser, int type){
	vector<DataPackage*> dataChunckComposition;
	vector<string> packageElements;
	vector<DataChunck*> dataChunckDataSet;

	int totalReadLines = 0;
	string fileName =  to_string(type) + "." + to_string(idUser) + ".txt";
	string filePath = "chunks/" + fileName;
	string::size_type sz;

	//cout << "FilePath: " << filePath << endl;

	fstream infile;
	string line;
	packageElements.resize(3);

	infile.open(filePath);

	if(infile.is_open()){
		//cout << "File is open" << endl;
		while(getline(infile, line)){
			totalReadLines++;
			stringstream ssin(line);

			//cout << line << endl;

			if(totalReadLines != 1){
				for(int i=0; i<3; i++){
					ssin >> packageElements[i];
				}

				DataChunck *chunck =  new DataChunck(stod(packageElements[0]),
					stod(packageElements[1]), stod(packageElements[2]), dataChunckComposition);

				//chunck->info();

				dataChunckDataSet.push_back(chunck);

				packageElements.clear();
			}else{
				cout << "Total elements in file: " + line << endl;
			}
		}
		infile.close();
	}else{
		cout << "File: " << fileName << " is CLOSED ";
		cout << "Path: " << filePath << endl;
	}

	cout << "Total elements processed: " << dataChunckDataSet.size() << endl;
	return dataChunckDataSet;
}