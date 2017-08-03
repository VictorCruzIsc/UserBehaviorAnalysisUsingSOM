./evaluate_module 100 200 800 150 3 R/r_trained_som/trained_lattice_4_100_25.txt 4 R/r_trained_som/trained_lattice_5_100_25.txt 5 R/r_trained_som/trained_lattice_6_100_25.txt 6 R/r_trained_som/trained_lattice_9_100_25.txt 9


ps | grep ./evaluate_module

kill -9 <psid>

data=$(ps | grep ./evaluate)

pidModule=$(echo $data | awk '{print $1;}')

echo $pidModule

echo $data | tr -s " " "\012"




ps | grep "./evaluate" > data.txt
data=$(tail -1 data.txt)
echo $data
pidModule=$(echo $data | awk '{print $1;}')
echo $pidModule
kill -9 $(echo $pidModule)