#include "../../include/functionality/StadisticsResults.h"

StadisticsResults::StadisticsResults(int initial, int final){
	_initialUserAssigned = initial;
	_finalUserAssigned = final;
	_value = 1;
}

int StadisticsResults::getInitialUserAssigned(){
	return _initialUserAssigned;
}

int StadisticsResults::getFinalUserAssigned(){
	return _finalUserAssigned;
}

int StadisticsResults::getValue(){
	return _value;
}

void StadisticsResults::addToValue(int amount){
	_value += amount;
}

string StadisticsResults::info(){
	string info = to_string(_initialUserAssigned);
	info += " " + to_string(_finalUserAssigned);
	info += " " + to_string(_value);
	return info;
}