//
//  lorenz_solver.hpp
//  hw4xcode
//
//  Created by Avik Ghosh on 5/23/24.
//

#ifndef lorenz_solver_hpp
#define lorenz_solver_hpp

#include <stdio.h>
#include <iostream>
#include <Eigen/Core>
#include <fstream>

// Simple struct to add in all the physical parameters instead of just adding a dozen magic numbers to a constructor
// Math from the link: https://hyemingu.github.io/assets/pdf/Project_Paper___Applied_Math_Modeling.pdf

namespace LorenzSolver
{
    template <typename VMut>
    struct params
    {
        VMut timeResolution;
        VMut rho, sigma, beta;
        VMut initX, initY, initZ;
    };

    template <typename VMut>
    class Model
    {
    public:
        Model(params<VMut> modelParams);
        void editParams();
        void solveSystem();
        void outputToCSV(std::string fname);
        Eigen::Matrix<VMut, Eigen::Dynamic, Eigen::Dynamic> data;
        VMut beta, rho, sigma;
        VMut dt;

    private:
        void lorenzDerivatives(Eigen::RowVector3<VMut> &prevRow, Eigen::RowVector3<VMut> &derivsRet);
        void RK4(Eigen::RowVector3<VMut> &prevRow, Eigen::RowVector3<VMut> &derivs, Eigen::RowVector3<VMut> &scratch);
    };

}
#endif /* lorenz_solver_hpp */
