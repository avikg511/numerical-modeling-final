//
//  models.cpp
//  finalxcode
//
//  Created by Avik Ghosh on 5/25/24.
//

#include "lorenz_solver.hpp"

using namespace LorenzSolver;

template <typename UConst, typename VMut>
Model<UConst, VMut>::Model(LorenzSolver::params<UConst, VMut> modelParams) {
    // Calculate params properly later, now just random values
    this->prandtlNum = 10.0;
    this->rayleighNum = 28.0;
    this->domainAspectRatio = 2.66;

    this->dt = modelParams.timeResolution;

    data.resize(int(1000 / dt), 6);

    // Initial conditions:
    this->data(0, 0) = modelParams.initX;
    this->data(0, 1) = modelParams.initY;
    this->data(0, 2) = modelParams.initZ;
    
    // Center of Mass
    this->data(0, 3) = modelParams.initX;
    this->data(0, 4) = modelParams.initY;
    this->data(0, 5) = modelParams.initZ;
}

template <typename UConst, typename VMut>
void Model<UConst, VMut>::solveSystem() {
    /*
        The 3 equations to solve are:
            \frac{dx}{dt} = -\sigma x + \sigma y \\
            \frac{dy}{dt} = \rho x  - y - xz \\
            \frac{dz}{dt} = -\beta z + xy \\

            where \sigma = this->prandtlNum,
                  \rho   = this->rayleighNum,
                  \beta  = this->domainAspectRatio

        The variables/derivation is from this writeup: https://hyemingu.github.io/assets/pdf/Project_Paper___Applied_Math_Modeling.pdf

        For the ODEs themselves, I'm using RK4.
    */
    
    Eigen::RowVector3<VMut> scratch;
    Eigen::RowVector3<VMut> derivs;
    Eigen::RowVector3<VMut> prevRow;
    
    for (int count = 1; count < data.rows(); count++) {
//         Model output
        prevRow = ((this->data).leftCols(3)).row(count - 1);
        RK4(prevRow, derivs, scratch);
        (this->data).leftCols(3).row(count) = prevRow + derivs;
        
//        Center of Masses
        (this->data)(count, 3) = (this->data).col(0).sum() / (count + 1);
        (this->data)(count, 4) = (this->data).col(1).sum() / (count + 1);
        (this->data)(count, 5) = (this->data).col(2).sum() / (count + 1);
   }
}

template <typename UConst, typename VMut>
void Model<UConst, VMut>::RK4(Eigen::RowVector3<VMut>& prevRow, Eigen::RowVector3<VMut>& derivs, Eigen::RowVector3<VMut>& scratch) {
    /*
     Let r \in {x, y, z}. Let's also consider f(rVal) = \frac{dr}{dt} @ r = rVal. (Evaluate derivative at rVal).

     rMidPointEstA = currentRVal + \frac{dt}{2} * f(rVal)
     rMidPointEstB = currentRVal + \frac{dt}{2} * f(rMidPointEstA)

     rNextValEst   = currentRVal + dt * f(rMidPointEstB)
     rNextVal      = currentRVal + \frac{dt}{6} * (f(currentRVal) + 2 * f(rMidPointEstA) + 2 * f(rMidPointEstB) + f(rNextValEst))

     We would do this for x, y, and z. Note that instead of using x, y, and z as explicit variables, we will instead use the data values at the
         the matrix position. Also, this x, y, z are independent of the actual physics. The code was written to solve convection from the physical
         parameters, but this will work for any lorenz system.
     */
    Eigen::RowVector3<VMut> initDerivs;
    lorenzDerivatives(prevRow, initDerivs);
    
    Eigen::RowVector3<VMut> testADerivs;
    scratch = prevRow + initDerivs * 0.5 * this->dt;
    lorenzDerivatives(scratch, testADerivs);
    
    Eigen::RowVector3<VMut> testBDerivs;
    scratch = prevRow + testADerivs * 0.5 * this->dt;
    lorenzDerivatives(scratch, testBDerivs);
    
    Eigen::RowVector3<VMut> testFinalDerivs;
    scratch = prevRow + testBDerivs * this->dt;
    lorenzDerivatives(scratch, testFinalDerivs);
    
    // Store data in final location, provided by method call
    derivs = this->dt * (initDerivs + 2 * testADerivs + 2 * testBDerivs + testFinalDerivs) / 6.0;
}

template <typename UConst, typename VMut>
void Model<UConst, VMut>::lorenzDerivatives(Eigen::RowVector3<VMut>& prevRow, Eigen::RowVector3<VMut>& derivsRet) {
    VMut x = prevRow[0];
    VMut y = prevRow[1];
    VMut z = prevRow[2];
    
    derivsRet[0] = (y - x) * this->prandtlNum;
    derivsRet[1] = x * (this->rayleighNum - z) - y;
    derivsRet[2] = x * y - this->domainAspectRatio * z;
}

template <typename UConst, typename VMut>
void Model<UConst, VMut>::outputToCSV(std::string fname) {
    const static Eigen::IOFormat CSVFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", "\n");

    std::ofstream filepath(fname.c_str());
    filepath << this->data.format(CSVFormat);
    filepath.close();
}
