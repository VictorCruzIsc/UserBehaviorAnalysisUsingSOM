all: som2

som2: main.cpp src/Neuron.cpp src/Matrix.cpp src/Utils.cpp src/SelfOrganizingMaps.cpp src/RGB.cpp src/DataSet.cpp src/DataPackage.cpp src/DataChunck.cpp src/StadisticsResults.cpp
	g++ -std=c++11 main.cpp src/Neuron.cpp src/Matrix.cpp src/Utils.cpp src/SelfOrganizingMaps.cpp src/RGB.cpp src/DataSet.cpp src/DataPackage.cpp src/DataChunck.cpp src/StadisticsResults.cpp -o som2 -lstdc++ -lglut -lGL -lGLU -lm