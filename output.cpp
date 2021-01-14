/**
 * @file        output.cpp
 *
 * @project     colDataUtil
 * @version     0.2
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       Perform calculations and output results.
 *
 */

#include "output.h"

/*
 * Perform all the selected operations on all the selected columns and print the
 * results to the terminal.
 */
void Output::printer(CmdArgs::Args* argsP) {
    vector<int> timesteps{IntV::getOneP(0)->getData()};

    size_t rowBgn, rowEnd;
    tuple<size_t, size_t> rowRange{argsP->getRowP()->getRowRange()};
    tie(rowBgn, rowEnd) = rowRange;

    string outputStr = "rows " + to_string(rowBgn) + " to " + to_string(rowEnd);

    // << std::setw(25) << colName
    // << std::setw(25) << outputStr

    // Print heading
    cout<< " Calculation results for rows "
            << to_string(rowBgn) << " to " << to_string(rowEnd) << '\n'
            << string(60, '=') << '\n';

    // Print calculations
    cout.setf(ios_base::scientific);
    cout.precision(numeric_limits<double>::max_digits10);

    // printer(dVP->getColName(), outputStr, CalcFnc::mapCalcToStr<int>.at(calc),
    //         calc(column, rowBgn, rowEnd));

    for (const int colNo : argsP->getColumnP()->getDoubleColSet()) {
        DoubleV* dVP{DoubleV::getOnePFromCol(colNo)};
        // Print subheading
        cout<< "\n " << dVP->getColName() << ":\n" << string(30, '-') << '\n';
        for (const CmdArgs::CalcId id : argsP->getCalcP()->getCalcIdSet()) {
            calcType calc{mapCalcIdToCalc<int>.at(id)};
            // Print calculations
            cout<< ' ' << left << setw(22)
                << CalcFnc::mapCalcToStr<int>.at(calc)
                << " = " << calc(colNo, rowBgn, rowEnd) << endl;
        }
    }
}

/*
 * Perform all the selected operations on all the selected columns and print the
 * results to the terminal.
 */
void Output::filer(CmdArgs::Args* argsP) {
    vector<int> timesteps{IntV::getOneP(0)->getData()};

    size_t rowBgn, rowEnd;
    tuple<size_t, size_t> rowRange{argsP->getRowP()->getRowRange()};
    tie(rowBgn, rowEnd) = rowRange;

    string outputStr = "rows " + to_string(rowBgn) + " to " + to_string(rowEnd);

    // << std::setw(25) << colName
    // << std::setw(25) << outputStr

    // Print heading
    cout<< " Calculation results for rows "
            << to_string(rowBgn) << " to " << to_string(rowEnd) << '\n'
            << string(60, '=') << '\n';

    // Print calculations
    cout.setf(ios_base::scientific);
    cout.precision(numeric_limits<double>::max_digits10);

    // printer(dVP->getColName(), outputStr, CalcFnc::mapCalcToStr<int>.at(calc),
    //         calc(column, rowBgn, rowEnd));

    for (const int colNo : argsP->getColumnP()->getDoubleColSet()) {
        DoubleV* dVP{DoubleV::getOnePFromCol(colNo)};
        // Print subheading
        cout<< "\n " << dVP->getColName() << ":\n" << string(30, '-') << '\n';
        for (const CmdArgs::CalcId id : argsP->getCalcP()->getCalcIdSet()) {
            calcType calc{mapCalcIdToCalc<int>.at(id)};
            // Print calculations
            cout<< ' ' << left << setw(22)
                << CalcFnc::mapCalcToStr<int>.at(calc)
                << " = " << calc(colNo, rowBgn, rowEnd) << endl;
        }
    }
}

void Output::output(CmdArgs::Args* argsP) {
    ColData::printColNames(argsP->getDataColTotal(), argsP->getDataDlmType());

    if (argsP->getPrintDataP()) {
        ColData::printData(argsP->getPrintDataP()->getDelimiter());
    }

    if (argsP->getCalcP()) {
        if (!argsP->getFileOutP()) {
            printer(argsP);
        }
        else {
            for (const string& fileOut : argsP->getFileOutP()->getFileLocSet()){
                filer(argsP);
                cout<< "The output has been written to the file \""
                    << fileOut << "\"." << std::endl;
            }
        }
    }
}
