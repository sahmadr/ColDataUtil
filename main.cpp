/**
 * @file        main.cpp
 *
 * @project     colDataUtil
 * @version     0.1
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 * @date        2020-11-19
 *
 * @brief       A utility program for loading, writing and performing
 *              calculations  on column data in comma or space delimited files.
 *              This program has been written primarily for timestep column
 *              data obtained from solvers used widely in computational fluid
 *              dynamics, it can be used for any data file having the correct
 *              formatting.
 */

#include "cmdArgs.h"
#include "columnData.h"

inline const string fileUniqueName = "TESTFILEOUT3";
inline const string filePath = "../data/"; //{"../data/" + fileName + "/"};
inline const string fileExt = ".csv";
inline const string file = filePath+fileUniqueName+fileExt;

// int main() {
int main(int argc, char* argv[]) {
    try {
        CmdArgs::Args input(argc, argv);
        input.process();
        input.output();

        // std::cout << "\nTotal columns = " << totalColumns;
        // std::string s(" ");
        // std::cout << s+" is space= "
        //     << std::any_of(s.begin(), s.end(), isspace);
/*
        cout << "TEST";
        ColData::outputValue(ColData::findMean, 2);
        ColData::outputValue(ColData::findQuadraticMean, 2);
        ColData::outputValue(ColData::findCubicMean, 2);
        ColData::outputValue(ColData::findMean, 2, 2, 8);
        ColData::outputValue(ColData::findQuadraticMean, 2, 2, 8);
        ColData::outputValue(ColData::findCubicMean, 2, 2, 8);
        ColData::outputValue(ColData::findMean, 2, 3);
        ColData::outputValue(ColData::findQuadraticMean, 2, 3);
        ColData::outputValue(ColData::findCubicMean, 2, 3);
        ColData::outputValue(ColData::findMean, 2, 5);
        ColData::outputValue(ColData::findQuadraticMean, 2, 5);
        ColData::outputValue(ColData::findCubicMean, 2, 5);
        ColData::outputValue(ColData::findMean, 3, 3);
        ColData::outputValue(ColData::findQuadraticMean, 3, 3);
        ColData::outputValue(ColData::findMax, "fy", 3, 9);
        ColData::outputValue(ColData::findMin, "fy", 3, 9);
        ColData::outputValue(ColData::findAbsMax, "fy", 3, 9);
        ColData::outputValue(ColData::findAbsMin, "fy", 3, 9);
 */
    }
    catch (const std::invalid_argument& exception) {
        std::cerr
            << "\n\nERROR! Invalid argument: " << exception.what()
            << generalErrorMessage << std::endl;
        return 1;
    }
    catch (const std::logic_error& exception) {
        std::cerr
            << "Logic error: " << exception.what()
            << generalErrorMessage << std::endl;
        return 1;
    }
    catch (const std::runtime_error& exception) {
        std::cerr
            << "\n\nERROR! Runtime error: " << exception.what()
            << generalErrorMessage << std::endl;
        return 1;
    }
    return 0;
}
