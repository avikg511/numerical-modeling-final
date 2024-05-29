//
//  ensemble_model_controller.hpp
//  numerical-modeling-final
//
//  Created by Avik Ghosh on 5/23/24.
//

#ifndef lorenz_ensemble_hpp
#define lorenz_ensemble_hpp

#include <stdio.h>
#include <iostream>
#include <Eigen/Core>
#include <fstream>
#include "lorenz_solver.hpp"
#include <sstream>
#include <string>
#include <random>

/*
    Class manages multiple sets of Lorenz Models, each with slight variations in say parameters, initial conditions,
        etc. etc. Also contains methods to compare individual instances on RMS error w.r.t timestep for states/Center of
        mass, etc.

    Also, the Model class has no understanding of the physics, it just understands the numbers passed in. This manager calculates those parameters.
*/

namespace LorenzManager
{
    template <typename VMut>
    struct physicalParams
    {
        VMut timeResolution;
        VMut lowerBoundaryTemp;
        VMut thermalConductivity;
        VMut fluidVel;
        VMut initX;
        VMut initY;
        VMut initZ;
    };

    template <typename VMut>
    class LorenzEnsemble
    {
    public:
        LorenzEnsemble(int numModels);
        LorenzEnsemble(physicalParams<VMut> params1, physicalParams<VMut> params2);
        // std::vector<Eigen::Matrix<VMut, Eigen::Dynamic, Eigen::Dynamic> &> models;
        void errorTimeseries(Eigen::Matrix<VMut, Eigen::Dynamic, 2> &errorSeries, LorenzSolver::Model<VMut> &model1,
                             LorenzSolver::Model<VMut> &model2);
        void compareModels(LorenzSolver::Model<VMut> &model1, LorenzSolver::Model<VMut> &model2);
        void outputScripts(bool showCenterOfMass, bool showAnimationNoSave, int numTimeSteps);

        std::vector<std::string> filepaths;
    };
};
#endif /* lorenz_ensemble_hpp */
