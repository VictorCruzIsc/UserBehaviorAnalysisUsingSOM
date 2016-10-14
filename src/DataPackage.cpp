#include "../include/DataPackage.h"

DataPackage::DataPackage(){
	_way = false;
	_localIp = 0;
	_destinationIp = 0;
	_protocol = "";
	_localPort = 0;
	_remotePort = 0;
	_transmitedBytes = 0;
	_originTimeStamp = 0;
	_deviceOSIp = "";
	_deviceMAC = "";
	_error = false;
}

DataPackage::DataPackage(bool way, int localIp, int destinationIp, string protocol,
			int localPort, int remotePort, int transmitedBytes,
			long long int originTimeStamp, string deviceOSIp, string deviceMAC){
	_way = way;
	_localIp = localIp;
	_destinationIp = destinationIp;
	_protocol = protocol;
	_localPort = localPort;
	_remotePort = remotePort;
	_transmitedBytes = transmitedBytes;
	_originTimeStamp = originTimeStamp;
	_deviceOSIp = deviceOSIp;
	_deviceMAC = deviceMAC;
	_error = false;
}

DataPackage::DataPackage(vector<string> packageElements){
	if(packageElements.size() != 12){
		_error = true;
		cout << "ERROR size = " << packageElements.size() << endl;
		return;
	}

#ifdef DEBUG
	cout << "Values: " << endl;
	for(int i=0; i<12; i++){
		cout << i << " " << packageElements[i] << endl;
	}
#endif

	// Check problem with ip
	string ipError = "FF02";
	size_t found = packageElements[3].find(ipError);
	if(found != string::npos){
    	_error = true;
    	return;
	}

	_way = wayFromString(packageElements[2]);
	_localIp = ipFromString(packageElements[3]);
	_destinationIp = ipFromString(packageElements[4]);
	_protocol = packageElements[5];
	_localPort = stoi(packageElements[6]);
	_remotePort = stoi(packageElements[7]);
	_transmitedBytes = stoi(packageElements[11]);
	_originTimeStamp = stoll (packageElements[8]);
	_deviceOSIp = packageElements[9];
	_deviceMAC = packageElements[10];
	_components = packageElements;
	_error =  false;
}

bool DataPackage::wayFromString(string value){
	if(value.compare("OUT") == 0){
		return false;
	}
	return true;
}

int DataPackage::ipFromString(string value){
	// 189.230.4.163
	// To calculate the decimal address from a dotted string, perform the following calculation.
	// (first octet * 256³) + (second octet * 256²) + (third octet * 256) + (fourth octet)
	// =	(first octet * 16777216) + (second octet * 65536) + (third octet * 256) + (fourth octet)
	// =	(189 * 16777216) + (230 * 65536) + (4 * 256) + (163)
	// = 3185968291
	int ip = 0;
	int multiplier[4];
	string octet;

	multiplier[0] = 16777216;
	multiplier[1] = 65536;
	multiplier[2] = 256;
	multiplier[3] = 1;

	replace(value.begin(), value.end(), '.', ' ');

	stringstream ssin(value);
	
	for(int i=0; i<4; i++){
		ssin >> octet;
		ip += stoi(octet) * multiplier[i];
	}
	return ip;
}

string DataPackage::info(){
	string representation  = "";
	if(_components.size() == 12)
		for(int i=0; i<12; i++){
			representation += _components[i] + " ";
		}
	return representation;
}