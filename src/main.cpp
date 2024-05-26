//
//  main.cpp
//  hw4xcode
//
//  Created by Avik Ghosh on 5/23/24.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "lorenz_solver.hpp"
#include "lorenz_solver.cpp" // Included to avoid templating issues

int main(int argc, const char * argv[]) {
    LorenzSolver::params<const double, double> params {
        (const double) 0.01,        // timeResolution
        (double) 3.0,               // lowerBoundaryTemp
        (double) 0.001,             // thermalConductivity
        (double) 0.001,             // fluidVel
        (double) 1.0,               // initX
        (double) 1.0,               // initY
        (double) 1.0,               // initZ
    };

    LorenzSolver::Model<const double, double> x(params);
    x.solveSystem();
    x.outputToCSV("test.csv");
    return 0;
}

