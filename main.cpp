/**
 * @file        main.cpp
 *
 * @project     ColDataUtil
 * @version     0.4
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       A utility program for loading, writing and performing
 *              calculations on column data in comma or space delimited files.
 *              This program has been written primarily for timestep column
 *              data obtained from solvers used widely in computational fluid
 *              dynamics, it can be used for any data file having the correct
 *              formatting. The calculations can only be performed for the
 *              non-integer columns.
 */

#include "namespaces.h"
#include "cmdArgs.h"
#include "output.h"
#include "errorMsgs.h"

int main(int argc, char* argv[]) {
    try {
        CmdArgs::Args input(argc, argv);
        input.process();
        Output::output(&input);
        // input.output();
    }
    catch (const invalid_argument& exception) {
        std::cerr
            << "\n\nERROR! Invalid argument: " << exception.what()
            << generalErrorMessage << std::endl;
        return 1;
    }
    catch (const logic_error& exception) {
        std::cerr
            << "Logic error: " << exception.what()
            << generalErrorMessage << std::endl;
        return 1;
    }
    catch (const runtime_error& exception) {
        std::cerr
            << "\n\nERROR! Runtime error: " << exception.what()
            << generalErrorMessage << std::endl;
        return 1;
    }
    return 0;
}
