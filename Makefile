all: som

som: main.cpp src/Neuron.cpp src/Matrix.cpp src/Utils.cpp src/SelfOrganizingMaps.cpp src/RGB.cpp
	g++ -std=c++11 main.cpp src/Neuron.cpp src/Matrix.cpp src/Utils.cpp src/SelfOrganizingMaps.cpp src/RGB.cpp -o som -lstdc++ -lglut -lGL -lGLU -lm