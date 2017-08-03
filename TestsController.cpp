#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

int main(){
	string currentExecution = "./scripts/setup.sh";
	string killExecution = "./scripts/kill.sh";

	vector<string> runningOptions;
	runningOptions.push_back("4 5 9");
	runningOptions.push_back("4 5 10");
	runningOptions.push_back("4 9 10");
	runningOptions.push_back("5 9 10");

	int totalRunningOptions = runningOptions.size();

	cout << "MAIN THREAD START" << endl;

	for(int i=50; i<160; i+=25){
		for(int currentRunningOption=0; currentRunningOption<totalRunningOptions;
			currentRunningOption++){
			currentExecution += " " + to_string(i)
								+ " " + runningOptions[currentRunningOption];

			system(currentExecution.c_str());
			system(killExecution.c_str());

			currentExecution = "./scripts/setup.sh";
		}
	}

	cout << "MAIN THREAD FINISH" << endl;
}