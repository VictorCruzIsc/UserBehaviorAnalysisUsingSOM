all: train_module

som: main.cpp src/functionality/Neuron.cpp src/functionality/Matrix.cpp src/functionality/Utils.cpp src/functionality/SelfOrganizingMaps.cpp src/functionality/RGB.cpp src/functionality/DataSet.cpp src/functionality/DataPackage.cpp src/functionality/DataChunck.cpp src/functionality/StadisticsResults.cpp src/results/Results.cpp src/results/SamplesResult.cpp src/results/Experiment.cpp src/results/NumericAnalysis.cpp src/results/PercentualAnalysis.cpp src/results/ExperimentAverageAnalysis.cpp src/results/UserGraph.cpp
	g++ -std=c++11 main.cpp src/functionality/Neuron.cpp src/functionality/Matrix.cpp src/functionality/Utils.cpp src/functionality/SelfOrganizingMaps.cpp src/functionality/RGB.cpp src/functionality/DataSet.cpp src/functionality/DataPackage.cpp src/functionality/DataChunck.cpp src/functionality/StadisticsResults.cpp src/results/Results.cpp src/results/SamplesResult.cpp src/results/Experiment.cpp  src/results/NumericAnalysis.cpp src/results/PercentualAnalysis.cpp src/results/ExperimentAverageAnalysis.cpp src/results/UserGraph.cpp -o som -lstdc++ -lglut -lGL -lGLU -lm

som_mac: main.cpp src/functionality/Neuron.cpp src/functionality/Matrix.cpp src/functionality/Utils.cpp src/functionality/SelfOrganizingMaps.cpp src/functionality/RGB.cpp src/functionality/DataSet.cpp src/functionality/DataPackage.cpp src/functionality/DataChunck.cpp src/functionality/StadisticsResults.cpp src/results/Results.cpp src/results/SamplesResult.cpp src/results/Experiment.cpp src/results/NumericAnalysis.cpp src/results/PercentualAnalysis.cpp src/results/ExperimentAverageAnalysis.cpp src/results/UserGraph.cpp
	g++ -std=c++11 main.cpp src/functionality/Neuron.cpp src/functionality/Matrix.cpp src/functionality/Utils.cpp src/functionality/SelfOrganizingMaps.cpp src/functionality/RGB.cpp src/functionality/DataSet.cpp src/functionality/DataPackage.cpp src/functionality/DataChunck.cpp src/functionality/StadisticsResults.cpp src/results/Results.cpp src/results/SamplesResult.cpp src/results/Experiment.cpp  src/results/NumericAnalysis.cpp src/results/PercentualAnalysis.cpp src/results/ExperimentAverageAnalysis.cpp src/results/UserGraph.cpp -o som -framework OpenGL -framework GLUT -w


dataset_module: mainDataSetModule.cpp src/functionality/Neuron.cpp src/functionality/Matrix.cpp src/functionality/Utils.cpp src/functionality/SelfOrganizingMaps.cpp src/functionality/RGB.cpp src/functionality/DataSet.cpp src/functionality/DataPackage.cpp src/functionality/DataChunck.cpp src/functionality/StadisticsResults.cpp src/results/Results.cpp src/results/SamplesResult.cpp src/results/Experiment.cpp src/results/NumericAnalysis.cpp src/results/PercentualAnalysis.cpp src/results/ExperimentAverageAnalysis.cpp src/results/UserGraph.cpp
	g++ -std=c++11 mainDataSetModule.cpp src/functionality/Neuron.cpp src/functionality/Matrix.cpp src/functionality/Utils.cpp src/functionality/SelfOrganizingMaps.cpp src/functionality/RGB.cpp src/functionality/DataSet.cpp src/functionality/DataPackage.cpp src/functionality/DataChunck.cpp src/functionality/StadisticsResults.cpp src/results/Results.cpp src/results/SamplesResult.cpp src/results/Experiment.cpp  src/results/NumericAnalysis.cpp src/results/PercentualAnalysis.cpp src/results/ExperimentAverageAnalysis.cpp src/results/UserGraph.cpp -o dataset_module -framework OpenGL -framework GLUT -w

train_module: mainTrainModule.cpp src/functionality/Neuron.cpp src/functionality/Matrix.cpp src/functionality/Utils.cpp src/functionality/SelfOrganizingMaps.cpp src/functionality/RGB.cpp src/functionality/DataSet.cpp src/functionality/DataPackage.cpp src/functionality/DataChunck.cpp src/functionality/StadisticsResults.cpp src/results/Results.cpp src/results/SamplesResult.cpp src/results/Experiment.cpp src/results/NumericAnalysis.cpp src/results/PercentualAnalysis.cpp src/results/ExperimentAverageAnalysis.cpp src/results/UserGraph.cpp
	g++ -std=c++11 -g mainTrainModule.cpp src/functionality/Neuron.cpp src/functionality/Matrix.cpp src/functionality/Utils.cpp src/functionality/SelfOrganizingMaps.cpp src/functionality/RGB.cpp src/functionality/DataSet.cpp src/functionality/DataPackage.cpp src/functionality/DataChunck.cpp src/functionality/StadisticsResults.cpp src/results/Results.cpp src/results/SamplesResult.cpp src/results/Experiment.cpp  src/results/NumericAnalysis.cpp src/results/PercentualAnalysis.cpp src/results/ExperimentAverageAnalysis.cpp src/results/UserGraph.cpp -o train_module -framework OpenGL -framework GLUT -w