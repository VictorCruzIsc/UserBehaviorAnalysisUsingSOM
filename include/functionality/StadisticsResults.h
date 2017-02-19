#ifndef STADISTIC_RESULTS_H
#define STADISTIC_RESULTS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

using namespace std;

class StadisticsResults{
	public:
		StadisticsResults(int initial, int final);
		string info();
		int getInitialUserAssigned();
		int getFinalUserAssigned();
		int getValue();
		void addToValue(int amount);

	private:
		int _initialUserAssigned;
		int _finalUserAssigned;
		int _value;
};
#endif