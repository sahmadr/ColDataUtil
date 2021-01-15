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
 * Print the names of all the vector columns; first integers and then doubles.
 */
void Output::printInputDataInfo(int dataColTotal, Delimitation dataDlmType) {
    cout<< left << '\n' << string(30, '=') << "\n "
        << "Input data information\n" << string(30, '=') << "\n\n"
        << setw(20) << " Total columns:" << dataColTotal << '\n'
        << setw(20) << " Total rows:" << DoubleV::getOneP(0)->getData().size()
        << '\n' << setw(20) << " Data delimitation:"
        << ((dataDlmType == Delimitation::delimited) ? "delimiter" :
            ((dataDlmType == Delimitation::spacedAndDelimited) ?
                "whitespace and delimiter" : "whitespace")) << endl
        << "\n Integer columns:\n" << string(30, '-') << "\n";
    for (IntV* iVP : IntV::getSetP()) {
        cout<< setw(3) << right << iVP->getColNo() << ". "
            << left << iVP->getColName() << '\n';
    }
    cout<< "\n Double columns:\n"
        << string(30, '-') << "\n";
    for (DoubleV* dVP : DoubleV::getSetP()) {
        cout<< setw(3) << right << dVP->getColNo() << ". "
            << left << dVP->getColName() << '\n';
    }
    cout<< '\n' << string(55, '=') << '\n';
}

/*
 * Perform the output operations based on user input.
 */
void Output::output(CmdArgs::Args* argsP) {
    printInputDataInfo(argsP->getDataColTotal(), argsP->getDataDlmType());

    if (argsP->getPrintDataP()) {
        ColData::printData(argsP->getPrintDataP()->getDelimiter());
    }

    if (argsP->getCalcP()) {
        if (!argsP->getFileOutP()) {
            printer(argsP->getRowP()->getRowRange(),
                    argsP->getColumnP()->getDoubleColSet(),
                    argsP->getCalcP()->getCalcIdSet());
        }
        else {
            for (const string& fileOut : argsP->getFileOutP()->getFileLocSet()){
                filer(fileOut,
                      argsP->getRowP()->getRowRange(),
                      argsP->getColumnP()->getDoubleColSet(),
                      argsP->getCalcP()->getCalcIdSet());
                cout<< "\nThe output has been written to \"" << fileOut << "\""
                    << endl;
            }
        }
    }
}

/*
 * Perform all the selected operations on all the selected columns and print the
 * results to the terminal.
 */
void Output::printer(
        const tuple<size_t, size_t> rowRange,
        const vector<int>& doubleColSet,
        const vector<CmdArgs::CalcId>& calcIdSet) {
    vector<int> timesteps{IntV::getOneP(0)->getData()};
    size_t rowBgn, rowEnd;
    tie(rowBgn, rowEnd) = rowRange;

    // Set cout properties
    cout.setf(ios_base::scientific);
    cout.precision(numeric_limits<double>::max_digits10);

    // Print heading
    cout<< " Calculation results for "
        << "rows " << to_string(rowBgn) << " to " << to_string(rowEnd) << '\n'
        << string(55, '=') << '\n';

    // Print calculations
    for (const int colNo : doubleColSet) {

        // Print subheading
        cout<< "\n " << DoubleV::getOnePFromCol(colNo)->getColName() << '\n'
            << string(55, '-') << '\n';

        // Print calculation results
        for (const CmdArgs::CalcId id : calcIdSet) {
            calcType calc{mapCalcIdToCalc<int>.at(id)};
            cout<< ' ' << left << setw(22)
                << CalcFnc::mapCalcToStr<int>.at(calc)
                << " = " << calc(colNo, rowBgn, rowEnd) << '\n';
        }
    }
    cout << endl;
}

/*
 * Perform all the selected operations on all the selected columns and print the
 * results to the terminal.
 */
void Output::filer(const string& fileOutStr,
        const tuple<size_t, size_t> rowRange,
        const vector<int>& doubleColSet,
        const vector<CmdArgs::CalcId>& calcIdSet) {
    vector<int> timesteps{IntV::getOneP(0)->getData()};
    size_t rowBgn, rowEnd;
    tie(rowBgn, rowEnd) = rowRange;

    // Open file and set properties
    ofstream fOut;
    fOut.open(fileOutStr, ios_base::app);
    if(!fOut) { throw runtime_error(errorOutputFile); }
    fOut.setf(ios_base::scientific);
    fOut.precision(numeric_limits<double>::max_digits10);

    // File heading
    fOut<< "\nCalculation results for "
        << "rows " << to_string(rowBgn) << " to " << to_string(rowEnd)
        << '\n' << string(70, '`') << '\n';

    // File subheadings
    fOut << "Calculations\\Columns,";
    for (const int colNo : doubleColSet) {
        fOut<< DoubleV::getOnePFromCol(colNo)->getColName() << ',';
    }

    // File calculation results
    for (const CmdArgs::CalcId id : calcIdSet) {
        calcType calc{mapCalcIdToCalc<int>.at(id)};
        fOut << '\n' << CalcFnc::mapCalcToStr<int>.at(calc) << ',';
        for (const int colNo : doubleColSet) {
            fOut << calc(colNo, rowBgn, rowEnd) << ',';
        }
    }
    fOut << '\n';
    fOut.close();
}