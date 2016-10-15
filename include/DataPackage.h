#ifndef DATAPACKAGE_H
#define DATAPACKAGE_H

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <limits>
#include <math.h>
#include <cfloat>
#include <algorithm>
#include <map>

using namespace std;

//#define DEBUG

class DataPackage{
	public:
		DataPackage();
		DataPackage(bool way, int localIp, int destinationIp, int protocol,
			int localPort, int remotePort, int transmitedBytes,
			long long int originTimeStamp, string deviceOSIp, string deviceMAC);
		DataPackage(vector<string> packageElements);
		~DataPackage();
		
		// getters
		bool getWay(){ return _way; }
		int getLocalIp(){ return _localIp; }
		int getDestinationIp(){ return _destinationIp; }
		int getProtocol(){ return _protocol; }
		int getLocalPort(){ return _localPort; }
		int getRemotePort(){ return _remotePort; }
		int getTransmitedBytes(){ return _transmitedBytes; }
		long long int getOriginTimeStamp(){ return _originTimeStamp; }
		string getDeviceOSIp(){ return _deviceOSIp; }
		string getDeviceMAC(){ return _deviceMAC; }
		bool getError(){return _error;}

		// setters
		void setWay(bool way){ _way = way; }
		void setLocalIp(int localIp){ _localIp = localIp; }
		void setdestinationIp(int destinationIp){
			_destinationIp = destinationIp;
		}
		void setProtocol(int protocol){ _protocol = protocol; }
		void setLocalPort(int localPort){ _localPort = localPort; }
		void setRemotePort(int remotePort){ _remotePort = remotePort; }
		void setTransmitedBytes(int transmitedBytes){
			_transmitedBytes = transmitedBytes;
		}
		void setOriginTimestamp(long long int originTimeStamp){
			_originTimeStamp = originTimeStamp;
		}
		void setDeviceOSIp(string deviceOSIp){ _deviceOSIp = deviceOSIp; }
		void setDeviceMAC(string deviceMAC){ _deviceMAC = deviceMAC; }

		vector<double> dataPackageToVector();
		string info();

	private:
		bool wayFromString(string value);
		int ipFromString(string value);
		int encodeProtocolName(string value);

		bool _way;
		bool _error;
		int _localIp;
		int _destinationIp;
		int _localPort;
		int _remotePort;
		int _transmitedBytes;
		int _protocol;
		long long int _originTimeStamp;
		string _deviceOSIp;
		string _deviceMAC;
		vector<string> _components;
};
#endif