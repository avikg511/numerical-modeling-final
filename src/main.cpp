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

// Previously used to test Lorenz Model Outputs
void testLorenzModel()
{
    LorenzSolver::params<double> params;

    // Lorenz Parameters
    params.timeResolution = 0.01;
    params.beta = 2.66;
    params.sigma = 10.0;
    params.rho = 28.0;

    // Model Initial Conditions
    params.initX = 1.0;
    params.initY = 1.0;
    params.initZ = 1.0;

    LorenzSolver::Model<const double, double> singleModel(params);
    singleModel.solveSystem();
    singleModel.outputToCSV("test.csv");
}

int main()
{
    testLorenzModel();
    return 0;
}