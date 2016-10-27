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
#include <stdio.h>
#include <time.h>

using namespace std;

//#define DEBUG
#define FIRST_OCTET_MULTIPLIER		16777216
#define SECOND_OCTET_MULTIPLIER		65536
#define THIRD_OCTET_MULTIPLIER		256
#define FOURTH_OCTET_MULTIPLIER		1
#define INTERNAL_IP_VALUE			148 * FIRST_OCTET_MULTIPLIER + 201 * SECOND_OCTET_MULTIPLIER

class DataPackage{
	public:
		DataPackage();
		DataPackage(bool way, int localIp, int destinationIp, int protocol,
			int localPort, int remotePort, int transmitedBytes,
			time_t originTimeStamp, string deviceOSIp, string deviceMAC,
			bool internalIp);
		DataPackage(vector<string> packageElements);
		~DataPackage();
		
		// getters
		bool getWay(){ return _way; }
		long int getLocalIp(){ return _localIp; }
		long int getDestinationIp(){ return _destinationIp; }
		int getProtocol(){ return _protocol; }
		int getLocalPort(){ return _localPort; }
		int getRemotePort(){ return _remotePort; }
		int getTransmitedBytes(){ return _transmitedBytes; }
		time_t getOriginTimeStamp(){ return _originTimeStamp; }
		string getDeviceOSIp(){ return _deviceOSIp; }
		string getDeviceMAC(){ return _deviceMAC; }
		bool hasError(){ return _error; }
		bool isInternalIp(){ return _internalIp; }

		// setters
		void setWay(bool way){ _way = way; }
		void setLocalIp(long int localIp){ _localIp = localIp; }
		void setdestinationIp(long int destinationIp){
			_destinationIp = destinationIp;
		}
		void setProtocol(int protocol){ _protocol = protocol; }
		void setLocalPort(int localPort){ _localPort = localPort; }
		void setRemotePort(int remotePort){ _remotePort = remotePort; }
		void setTransmitedBytes(int transmitedBytes){
			_transmitedBytes = transmitedBytes;
		}
		void setOriginTimestamp(time_t originTimeStamp){
			_originTimeStamp = originTimeStamp;
		}
		void setDeviceOSIp(string deviceOSIp){ _deviceOSIp = deviceOSIp; }
		void setDeviceMAC(string deviceMAC){ _deviceMAC = deviceMAC; }

		vector<double> dataPackageToVector();
		string info();

	private:
		bool wayFromString(string value);
		long int ipFromString(string value);
		int encodeProtocolName(string value);

		bool _way;
		bool _error;
		bool _internalIp;
		long int _localIp;
		long int _destinationIp;
		int _localPort;
		int _remotePort;
		int _transmitedBytes;
		int _protocol;
		time_t _originTimeStamp;
		string _deviceOSIp;
		string _deviceMAC;
		vector<string> _components;

		static const long int FIRSTOCTET;
		static const long int SECONDOCTET;
		static const long int THIRDOCTET;
		static const long int FOURTHOCTET;
		static const long int INTERNALIPFIRSTOCTET;
		static const long int INTERNALIPSECONDOCTET;
		static const long int INTERNALIPMASK;
};
#endif