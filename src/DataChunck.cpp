#include "../include/DataChunck.h"

DataChunck::DataChunck(){
	_proportionTCPUDPMetric = 0;
	_bytesToInternalIPMetric = 0;
	_bytesThroughWebMetric = 0;
	_totalBytes = 0;
	_totalBytesThroughTCP = 0;
	_totalBytesThroughUDP = 0;
	_totalBytesThroughInternalIP = 0;
	_totalBytesWebTraffic = 0;
	_dataChunckStart = 0;
	_dataChunckFinish = 0;
	_onlyMetrics = true;
}

DataChunck::DataChunck(double proportionTCPUD, double bytesToInternalIP,
			double bytesThroughWeb,
			vector<DataPackage*> &dataChunckComposition){
	_totalBytes = 0;
	_totalBytesThroughTCP = 0;
	_totalBytesThroughUDP = 0;
	_totalBytesThroughInternalIP = 0;
	_totalBytesWebTraffic = 0;
	_proportionTCPUDPMetric = proportionTCPUD;
	_bytesToInternalIPMetric = bytesToInternalIP;
	_bytesThroughWebMetric = bytesThroughWeb;
	_dataChunckComposition = dataChunckComposition;
	_dataChunckStart = 0;
	_dataChunckFinish = 0;
	_onlyMetrics = true;
}

DataChunck::DataChunck(vector<DataPackage*> &dataPackageVector){
	_proportionTCPUDPMetric = 0;
	_bytesToInternalIPMetric = 0;
	_bytesThroughWebMetric = 0;
	_totalBytes = 0;
	_totalBytesThroughTCP = 0;
	_totalBytesThroughUDP = 0;
	_totalBytesThroughInternalIP = 0;
	_totalBytesWebTraffic = 0;
	_dataChunckStart = 0;
	_dataChunckFinish = 0;
	_onlyMetrics = false;


	int totalRawPackages = dataPackageVector.size();
	int startIndex = 0;
	
	while(dataPackageVector[startIndex]->hasError()){
		startIndex++;
	}

	_dataChunckStart = dataPackageVector[startIndex]->getOriginTimeStamp();

	if(totalRawPackages > 0){
		for(int i=startIndex; i<totalRawPackages; i++){
			DataPackage *currentDataPackage = dataPackageVector[i];
			
			//Damaged Package
			if(currentDataPackage->hasError()){
				continue;
			}

			int packageBytes = currentDataPackage->getTransmitedBytes();
			int farPort = currentDataPackage->getRemotePort();
			
			_totalBytes += packageBytes;

			// TCP/UP relation metric
			if(currentDataPackage->getProtocol() == TCP){
				_totalBytesThroughTCP += packageBytes;
				// Web traffic metric
				if(farPort == 80 || farPort == 443 || farPort == 3128){
					_totalBytesWebTraffic += packageBytes;
				}
			}else if(currentDataPackage->getProtocol() == UDP){
				_totalBytesThroughUDP += packageBytes;
			}

			// ip internal metric
			if(currentDataPackage->isInternalIp()){
				_totalBytesThroughInternalIP += packageBytes;
			}
		}

		if(_totalBytesThroughUDP > 0){
			_proportionTCPUDPMetric = _totalBytesThroughTCP/_totalBytesThroughUDP;
		}

		if(_totalBytes >0){
			_bytesToInternalIPMetric = _totalBytesThroughInternalIP/_totalBytes;
			_bytesThroughWebMetric = _totalBytesWebTraffic/_totalBytes;
			_dataChunckComposition = dataPackageVector;
		}
	}

	_dataChunckFinish = dataPackageVector[totalRawPackages - 1]->getOriginTimeStamp();
}

vector<double> DataChunck::dataChunckToVector(){
	vector<double> dataChunckVector;
	dataChunckVector.resize(3);
	dataChunckVector[0] = _proportionTCPUDPMetric;
	dataChunckVector[1] = _bytesToInternalIPMetric;
	dataChunckVector[2] = _bytesThroughWebMetric;
	return dataChunckVector;
}

void DataChunck::info(){
	string dataChunckInfo = "";
	dataChunckInfo += "Chunck start: " + to_string(_dataChunckStart);
	dataChunckInfo += " Chunck finish: " + to_string(_dataChunckFinish);
	dataChunckInfo += " TCP/UDP metric: " + to_string(_proportionTCPUDPMetric);
	dataChunckInfo += " Internal Ip metric: " + to_string(_bytesToInternalIPMetric);
	dataChunckInfo += " Web metric: " + to_string(_bytesThroughWebMetric);
	dataChunckInfo += " Total Bytes: " + to_string(_totalBytes);
	dataChunckInfo += " Total Bytes(TCP): " + to_string(_totalBytesThroughTCP);
	dataChunckInfo += " Total Bytes(UDP): " + to_string(_totalBytesThroughUDP);
	dataChunckInfo += " Total Bytes internal: " + to_string(_totalBytesThroughInternalIP);
	dataChunckInfo += " Total Bytes web: " + to_string(_totalBytesWebTraffic);
	if(_onlyMetrics) dataChunckInfo += "Only metrics: TRUE";
	else dataChunckInfo += "Only metrics: FALSE";
	cout << dataChunckInfo << endl;
}