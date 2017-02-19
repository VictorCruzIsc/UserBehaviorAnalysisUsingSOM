#include "../../include/functionality/DataPackage.h"

const long int DataPackage::FIRSTOCTET	= 			16777216;
const long int DataPackage::SECONDOCTET	= 			65536;
const long int DataPackage::THIRDOCTET	= 			256;
const long int DataPackage::FOURTHOCTET	= 			1;
const long int DataPackage::INTERNALIPFIRSTOCTET =	148 * DataPackage::FIRSTOCTET;
const long int DataPackage::INTERNALIPSECONDOCTET =	201 * DataPackage::SECONDOCTET;
const long int DataPackage::INTERNALIPMASK =		DataPackage::INTERNALIPFIRSTOCTET + DataPackage::INTERNALIPSECONDOCTET;



DataPackage::DataPackage(){
	_way = false;
	_localIp = 0;
	_destinationIp = 0;
	_protocol = 0;
	_localPort = 0;
	_remotePort = 0;
	_transmitedBytes = 0;
	_originTimeStamp = 0;
	_deviceOSIp = "";
	_deviceMAC = "";
	_error = false;
	_internalIp = false;
}

DataPackage::DataPackage(bool way, int localIp, int destinationIp, int protocol,
			int localPort, int remotePort, int transmitedBytes,
			time_t originTimeStamp, string deviceOSIp, string deviceMAC, bool internalIp){
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
	_internalIp = internalIp;
}

DataPackage::DataPackage(vector<string> packageElements){
	if(packageElements.size() != 12){
		_error = true;
		cout << "ERROR size = " << packageElements.size() << endl;
		return;
	}

	_error =  false;
	_internalIp = false;

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
	_localIp = ipFromString(packageElements[3], true);
	_destinationIp = ipFromString(packageElements[4], false);
	_protocol = encodeProtocolName(packageElements[5]);
	_localPort = stoi(packageElements[6]);
	_remotePort = stoi(packageElements[7]);
	_transmitedBytes = stoi(packageElements[11]);
	_originTimeStamp = stoll (packageElements[8]);
	_deviceOSIp = packageElements[9];
	_deviceMAC = packageElements[10];
	_components = packageElements;
}

bool DataPackage::wayFromString(string value){
	if(value.compare("OUT") == 0){
		return false;
	}
	return true;
}

long int DataPackage::ipFromString(string value, bool isInternal){
	// 189.230.4.163
	// To calculate the decimal address from a dotted string, perform the following calculation.
	// (first octet * 256³) + (second octet * 256²) + (third octet * 256) + (fourth octet)
	// =	(first octet * 16777216) + (second octet * 65536) + (third octet * 256) + (fourth octet)
	// =	(189 * 16777216) + (230 * 65536) + (4 * 256) + (163)
	// = 3185968291

	long int ip = 0;
	string octet;

	replace(value.begin(), value.end(), '.', ' ');

	stringstream ssin(value);

	ssin >> octet;
	ip += stol(octet) * DataPackage::FIRSTOCTET;

	ssin >> octet;
	ip += stol(octet) * DataPackage::SECONDOCTET;

	if(isInternal){
		if(ip == DataPackage::INTERNALIPMASK){
			_internalIp = true;
		}
	}

	ssin >> octet;
	ip += stol(octet) * DataPackage::THIRDOCTET;

	ssin >> octet;
	ip += stol(octet) * DataPackage::FOURTHOCTET;

	return ip;
}

int DataPackage::encodeProtocolName(string value){
	map<string,int> protocols;
	protocols["TCP"] = 6;
	protocols["UDP"] = 17;

	auto search = protocols.find(value);
	if(search != protocols.end()) {
		return search->second;
	}
	return 0;
}

string DataPackage::info(){
	string representation  = "";
	if(_components.size() == 12)
		for(int i=0; i<12; i++){
			representation += _components[i] + " ";
		}
	return representation;
}

vector<double> DataPackage::dataPackageToVector(){
	vector<double> dataPackageVector;
	dataPackageVector.resize(3);
	dataPackageVector[0] = _destinationIp;
	dataPackageVector[1] = _protocol;
	dataPackageVector[2] = _remotePort;
	return dataPackageVector;
}