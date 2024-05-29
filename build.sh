# Clear terminal
clear;

# Make Project
cmake -B ./out -S .;
cd ./out;
trash ./csvData;
mkdir ./csvData;
make;
./kalman-filter-proj;
echo "Finished number crunching! Onto the plots!"

## Plotting Round 1
#mv ../lorenzplot.gnu .;
#gnuplot lorenzplot.gnu;
#mv lorenzplot.gnu ../

## Plotting Round 2
#gnuplot multiplePlots.gnu;