/**
 * @file        output.cpp
 *
 * @project     colDataUtil
 * @version     0.3
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       Perform calculations and output results.
 *
 */

#include "output.h"

//----------------------------------------------------------------------------//
//*************************** Main output function ***************************//
//----------------------------------------------------------------------------//
/*
 * Perform the output operations based on user input.
 */
void Output::output(CmdArgs::Args* argsP) {
    printInputDataInfo(argsP->getFileInP()->getFileLocation(),
                       argsP->getFileInP()->getDataColTotal(),
                       argsP->getFileInP()->getDataRowTotal(),
                       argsP->getFileInP()->getDataDlmType());

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
    if (argsP->getPrintDataP()) {
        dataPrinter(argsP->getPrintDataP()->getDelimiter(),
                    argsP->getFileInP()->getDataRowTotal());
    }
    if (argsP->getFileDataP()) {
        dataFiler(argsP->getFileDataP()->getFileDataName(),
                  argsP->getFileDataP()->getDelimiter(),
                  argsP->getFileInP()->getDataRowTotal());
        cout<< "\nThe output has been written to \""
            << argsP->getFileDataP()->getFileDataName() << "\"" << endl;
    }
}

//----------------------------------------------------------------------------//
//******************** Printing information on loaded data *******************//
//----------------------------------------------------------------------------//
/*
 * Print the names of all the vector columns; first integers and then doubles.
 */
void Output::printInputDataInfo(const string& fileInName,
        const int dataColTotal, const size_t dataRowTotal,
        const Delimitation dataDlmType) {
    cout<< left << '\n' << string(55, '=') << "\n "
        << "Input file: " << fileInName << '\n' << string(55, '=') << "\n\n"
        << setw(20) << " Total columns:" << dataColTotal << '\n'
        << setw(20) << " Total rows:" << dataRowTotal
        << '\n' << setw(20) << " Data delimitation:"
        << ((dataDlmType == Delimitation::delimited) ? "delimiter" :
            ((dataDlmType == Delimitation::spacedAndDelimited) ?
                "whitespace and delimiter" : "whitespace")) << '\n' << endl;

    if (!IntV::getSetP().empty()) {
        cout<< " Timestep column:\n" << string(30, '-') << '\n';
        for (IntV* iVP : IntV::getSetP()) {
            cout<< setw(3) << right << iVP->getColNo() << ". "
                << left << iVP->getColName() << '\n';
        }
        cout << '\n';
    }
    else {
        cout<< " No timestep column was found.\n\n";
    }
    cout<< " Data columns:\n"
        << string(30, '-') << "\n";
    for (DoubleV* dVP : DoubleV::getSetP()) {
        cout<< setw(3) << right << dVP->getColNo() << ". "
            << left << dVP->getColName() << '\n';
    }
    cout<< '\n' << string(55, '=') << '\n';
}

//----------------------------------------------------------------------------//
//***************** Printing and filing calculation results ******************//
//----------------------------------------------------------------------------//
/*
 * Perform all the selected operations on all the selected columns and print the
 * results to the terminal.
 */
void Output::printer(
        const tuple<size_t, size_t> rowRange,
        const vector<int>& doubleColSet,
        const vector<CmdArgs::CalcId>& calcIdSet) {
    // vector<int> timesteps{IntV::getOneP(0)->getData()};
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
    // vector<int> timesteps{IntV::getOneP(0)->getData()};
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

//----------------------------------------------------------------------------//
//********************* Printing and filing loaded data **********************//
//----------------------------------------------------------------------------//
/*
 * Print the data of all the vector columns; first integers and then doubles.
 */
void Output::dataPrinter(const string& dlm, const size_t dataRowTotal) {
    vector<IntV*>    iVSetP{IntV::getSetP()};
    vector<DoubleV*> dVSetP{DoubleV::getSetP()};

    cout.setf(ios_base::scientific);
    cout.precision(numeric_limits<double>::max_digits10);

    // Print the header line
    cout << '\n';
    for (IntV* iVP : iVSetP)    { cout << iVP->getColName() << dlm;}
    for (DoubleV* dVP : dVSetP) { cout << dVP->getColName() << dlm;}
    cout << "\n\n";

    // Print the data
    for (size_t row=0; row<dataRowTotal; ++row) {
        for (IntV* iVP : iVSetP)    { cout << iVP->getData()[row] << dlm;}
        for (DoubleV* dVP : dVSetP) { cout << dVP->getData()[row] << dlm;}
        cout << '\n';
    }
    cout << flush;
}

/*
 * File the data of all the vector columns; first integers and then doubles.
 */
void Output::dataFiler(const string& fileName, const string& dlm,
        const size_t dataRowTotal) {
    ofstream fOut{fileName};
    fOut.setf(ios_base::scientific);
    fOut.precision(numeric_limits<double>::max_digits10);

    vector<IntV*>    iVSetP{IntV::getSetP()};
    vector<DoubleV*> dVSetP{DoubleV::getSetP()};

    // File the header line
    for (IntV* iVP : iVSetP)    { fOut << iVP->getColName() << dlm; }
    for (DoubleV* dVP : dVSetP) { fOut << dVP->getColName() << dlm; }
    fOut << '\n';

    // File the data
    for (size_t row=0; row<dataRowTotal; ++row) {
        for (IntV* iVP : iVSetP) {
            fOut << iVP->getData()[row] << dlm;
        }
        for (DoubleV* dVP : dVSetP) {
            fOut << dVP->getData()[row] << dlm;
        }
        fOut << '\n';
    }
    fOut << flush;
    fOut.close();
}

//----------------------------------------------------------------------------//
//******************** Printing information on loaded data *******************//
//----------------------------------------------------------------------------//
/*
 * Print the total timesteps available in the file, assuming that
 * the first column of the loaded data is an integer representation of the
 * number of timesteps.
 */
/*
void ColData::printAvailableTimestepRange() {
    vector<int> timesteps{IntV::getOneP(0)->getData()};
    cout << timesteps.size() << '\n';
    cout    << "\nThe timestep range available in the source file is from "
            << timesteps.at(0) << " to ";
            // << timesteps.at(timesteps.size()-1) << " timesteps." << endl;
}
*/