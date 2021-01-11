/**
 * @file        main.cpp
 *
 * @project     colDataUtil
 * @version     0.2
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       A utility program for loading, writing and performing
 *              calculations  on column data in comma or space delimited files.
 *              This program has been written primarily for timestep column
 *              data obtained from solvers used widely in computational fluid
 *              dynamics, it can be used for any data file having the correct
 *              formatting.
 */

#include "cmdArgs.h"
#include "errorMsgs.h"

int main(int argc, char* argv[]) {
    try {
        CmdArgs::Args input(argc, argv);
        input.process();
        input.output();
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
