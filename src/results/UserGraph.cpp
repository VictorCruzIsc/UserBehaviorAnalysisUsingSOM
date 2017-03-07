#include "../../include/results/UserGraph.h"

void UserGraph::info(){
	int totalNeededGraphs = _userResults.size();
	cout << "User " << _belongingUser <<  " " << totalNeededGraphs << endl;
	for(int i=0; i<totalNeededGraphs; i++){
		_userResults[i]->infoToExport();
	}
}