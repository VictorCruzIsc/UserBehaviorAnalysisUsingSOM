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
			if(!dataPackage->getError())
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