Thesis project using Self Organizing Maps to classify user in the network

Used technologies:
c++
OpenGL
python

If needed to use MAC OS, only enable or disable the MAC flag on main.cpp and Classes.h files.
and on makefile use target som_mac to compile with required framework.

To run for getting results this is the command once the binary is created:
./som 1 2 200 300 700 1 trainedMatrixes/user4_1_11E.txt 4 trainedMatrixes/user5_1_11E.txt 5 trainedMatrixes/user6_1_11E.txt 6 trainedMatrixes/user8_1_11E.txt 8

NOTE:
To get correct results, you need to follow this:
cd scripts/
./unzipFiles.sh # This will create the deataset in the correct dataset folder, if this files are not created, you will get an error of Exeption

Once you have this info you can enjoy
