#include "lorenz_ensemble.hpp"

using namespace LorenzManager;

template <typename VMut>
LorenzEnsemble<VMut>::LorenzEnsemble(int numModels)
{
    LorenzSolver::params<double> paramSetMain;

    paramSetMain.timeResolution = 0.01;
    paramSetMain.initX = 1.0;
    paramSetMain.initY = 1.0;
    paramSetMain.initZ = 1.0;

    paramSetMain.beta = 2.66;
    paramSetMain.sigma = 10.0;
    paramSetMain.rho = 22.0;

    // Variations in Rho are distributed from the following variable for Ensemble
    VMut mean = 0;
    VMut stddev = 0.03;

    // Random number generation from: https://stackoverflow.com/questions/18950396/how-to-create-random-gaussian-vector-in-c
    // Could've pulled in boost, but seems like a waste to pull in another dependency
    std::random_device device_random_;
    std::default_random_engine generator_(device_random_());
    std::normal_distribution<VMut> distrib(mean, stddev);

    // If VMut is actually an integer, we don't want standard deviation to be 0, so we just make it 1.
    if (stddev == 0)
    {
        stddev = 1;
    }

    std::normal_distribution<double> dist(0, 0.5);
    VMut delRho = 0.0;
    std::string filePath;
    for (int count = 0; count < numModels; count++)
    {
        // Set precision for naming
        std::stringstream stream;
        stream << std::fixed << std::setprecision(6) << delRho;

        // Solve Model
        LorenzSolver::Model<VMut> models(paramSetMain);
        models.solveSystem();

        filePath = "./csvData/test_" + std::to_string(count) + "del_" + stream.str() + ".csv";
        filepaths.push_back(filePath);
        models.outputToCSV(filePath);
        outputScripts(false, false, 10000);

        // Adjust next model
        delRho = distrib(generator_);
        paramSetMain.rho = 28.0 + delRho;
        models.data.setZero();
    }
}

template <typename VMut>
void LorenzEnsemble<VMut>::outputScripts(bool showCenterOfMass, bool showAnimationNoSave, int numTimeSteps) {
    std::ofstream fname("./multiplePlots.gnu");
    if (fname.is_open()) {
        fname << "#File Setup" << std::endl;
        fname << "set term qt persist" << std::endl;
        fname << "set datafile separator \",\"" << std::endl;
        fname << "set title \"Lorenz Model Output\"" << std::endl;
        fname << "set xlabel \"x\"" << std::endl;
        fname << "set xlabel \"y\"" << std::endl;
        fname << "set xlabel \"z\"" << std::endl;
        
        fname << std::endl << "#Set up ranges" << std::endl;
        fname << "set xrange [-30:30]" << std::endl;
        fname << "set yrange [-30:30]" << std::endl;
        fname << "set zrange [0:50]" << std::endl;

        fname << "# filename and n=number of lines of your data" << std::endl;
        fname << "set term qt persist" << std::endl << std::endl;
    } else {
        std::cout << "File opening failed." << std::endl;
        assert(1==0);
    }
    
    if (!showAnimationNoSave) {
        fname << "set term gif animate delay 10" << std::endl;
        fname << "set output 'data_ensemble.gif'" << std::endl << std::endl;
    }
    fname << "set term pngcairo size 960, 540" << std::endl;

    int count = 0;
    for (auto fileName: filepaths) {
        if (!fileName.empty()) {
            if (count == 0) {
                fname << "n = system(sprintf('cat %s | wc -l', '" + fileName + "'))" << std::endl;
            }
            fname << "filedata" + std::to_string(count) + " = '" + fileName + "'" << std::endl;
            count++;
        }
    }

    std::string hexChoice = "0123456789ABCDEF";

    for (int i = 0; i < count; i++) {
        // Choosing a random color
        std::string color = "";
        int randChoice = 0;
        for (int i = 0; i < 6; i++) {
            randChoice = std::rand() % 16;
            color += hexChoice[randChoice];
        }

        fname << "set linetype " + std::to_string(i + 1) + " lw 2 lc rgb '#" + color + "' pt 1" << std::endl;
    }
    // Setting up plotting loop
    /*
    splot filedata u 1:2:3 every ::1::j w l lt 1 title "Lorenz Model" , \
          filedata u 1:2:3 every ::j::j w p lt 2 notitle, \
          filedata u 4:5:6 every ::j::j w p lt 3 title "Center of Mass"
    }
    */
    fname << "do for [j=1:n:5] {" << std::endl;
    fname << "\tset title 'time '.j" << std::endl;

    // Line one is different
    fname << "\tsplot filedata" + std::to_string(0) + " u 1:2:3 every ::1::j w l lt " + std::to_string(0 + 1) 
                + " title \"Plot 0\", \\" << std::endl;
    // fname << "filedata" + std::to_string(0) _ " u 1:2:3 every ::j::j w p lt " so no trailing point
    for (int i = 1; i < count - 1; i++) {
        fname << "\t\tfiledata" + std::to_string(i) + " u 1:2:3 every ::1::j w l lt " + std::to_string(i + 1) 
                + " title \"Plot " + std::to_string(i) + "\", \\" << std::endl;
    }
    fname << "\t\tfiledata" + std::to_string(i) + " u 1:2:3 every ::1::j w l lt " + std::to_string(i + 1) 
                + " title \"Plot " + std::to_string(i) + "\"" << std::endl;


    fname << "}" << std::endl;
    fname.close();
}