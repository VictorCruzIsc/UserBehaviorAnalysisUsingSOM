#ifndef DATACHUNCK_H
#define DATACHUNCK_H

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <string>
#include <ctime>
#include <sstream>
#include <math.h>
#include <cfloat>
#include <algorithm>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

#include "DataPackage.h"

#define TCP 6
#define UDP 17

using namespace std;

class DataChunck{
	public:
		DataChunck();
		DataChunck(double proportionTCPUD, double bytesToInternalIP,
			double bytesThroughWebMetric,
			vector<DataPackage*> &dataChunckComposition);
		DataChunck(vector<DataPackage*> &dataPackageVector);
		~DataChunck();

		double getProportionTCPUDPMetric(){
			return _proportionTCPUDPMetric;
		}
		double getBytesToInternalIPMetric(){
			return _bytesToInternalIPMetric;
		}
		double getBytesThroughWebMetric(){
			return _bytesThroughWebMetric;
		}

		double getTotalBytes(){
			return _totalBytes;
		}

		double getTotalBytesThroughTCP(){
			return _totalBytesThroughTCP;
		}

		double getTotalBytesThroughUDP(){
			return _totalBytesThroughUDP;
		}

		double getTotalBytesThroughInternalIp(){
			return _totalBytesThroughInternalIP;
		}

		double getTotalBytesWebTraffic(){
			return _totalBytesWebTraffic;
		}

		time_t getDataChunckStart(){
			return _dataChunckStart;
		}

		time_t getDataChunckFinish(){
			return _dataChunckFinish;
		}

		bool hasOnlyMetrics(){
			return _onlyMetrics;
		}

		vector<DataPackage*> getDataChunckComposition(){
			return _dataChunckComposition;
		}

		void setProportionTCPUDPMetric(double value){
			_proportionTCPUDPMetric = value;
		}
		void setBytesToInternalIPMetric(double value){
			_bytesToInternalIPMetric = value;
		}
		void setBytesThroughWebMetric(double value){
			_bytesThroughWebMetric = value;
		}

		void setTotalBytes(double value){
			_totalBytes = value;
		}

		void setTotalBytesThroughTCP(double value){
			_totalBytesThroughTCP = value;
		}

		void setTotalBytesThroughUDP(double value){
			_totalBytesThroughUDP = value;
		}

		void setTotalBytesThroughInternalIp(double value){
			_totalBytesThroughInternalIP = value;
		}

		void setTotalBytesWebTraffic(double value){
			_totalBytesWebTraffic = value;
		}

		void setDataChunckStart(time_t value){
			_dataChunckStart = value;
		}

		void setDataChunckFinish(time_t value){
			_dataChunckFinish= value;
		}

		void setDataChunckComposition(
			vector<DataPackage*> &dataPackageVector){
			_dataChunckComposition = dataPackageVector;
		}

		void setOnlyMetrics(bool value){
			_onlyMetrics = value;
		}

		vector<double> dataChunckToVector();

		void info();

		string exportDataChunck();

	private:
		// Original Values
		double _totalBytes;
		double _totalBytesThroughTCP;
		double _totalBytesThroughUDP;
		double _totalBytesThroughInternalIP; //148.201.X.X
		double _totalBytesWebTraffic; // Analyzed ports: 80, 40, 3128

		// Metrics
		double _proportionTCPUDPMetric;
		double _bytesToInternalIPMetric;
		double _bytesThroughWebMetric;

		// DataChunck composition
		vector<DataPackage*> _dataChunckComposition;

		time_t _dataChunckStart;
		time_t _dataChunckFinish;

		bool _onlyMetrics;
};
#endif