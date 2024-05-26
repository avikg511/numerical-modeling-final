# Clear terminal
clear;

# Make Project
cmake -B ./out -S .;
cd ./out;
make;
./kalman-filter-proj;

## Plotting
mv ../lorenzplot.gnu .;
gnuplot lorenzplot.gnu;
mv lorenzplot.gnu ../
