/**
 * @file        output.cpp
 *
 * @project     ColDataUtil
 * @version     0.4
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
    if (argsP->getVersionP()) {
        cout << argsP->getVersionP()->getMsg();
        return;
    }
    printInputDataInfo(argsP->getFileInP()->getFileLocation(),
                       argsP->getColumnP()->getDataColTotal(),
                       argsP->getRowP()->getDataRowTotal(),
                       argsP->getFileInP()->getDataDlmType(),
                       argsP->getTimestepP()->getDataTimestepRange());

    if (argsP->getCalcP()) {
        if (!argsP->getFileOutP()) {
            printer(argsP->getRowP()->getRange(),
                    argsP->getTimestepP()->isTimestepConsistent(),
                    argsP->getTimestepP()->getRange(),
                    argsP->getColumnP()->getDataDoubleColSet(),
                    argsP->getCalcP()->getCalcIdSet());
        }
        else {
            for (const string& fileOut : argsP->getFileOutP()->getFileLocSet()){
                filer(fileOut, argsP->getFileInP()->getFileLocation(),
                      argsP->getRowP()->getRange(),
                      argsP->getTimestepP()->isTimestepConsistent(),
                      argsP->getTimestepP()->getRange(),
                      argsP->getColumnP()->getDataDoubleColSet(),
                      argsP->getCalcP()->getCalcIdSet());
                cout<< "\nThe output has been written to \"" << fileOut << "\""
                    << endl;
            }
        }
    }
    if (argsP->getPrintDataP()) {
        dataPrinter(argsP->getPrintDataP()->getDelimiter(),
                    argsP->getRowP()->getDataRowTotal());
    }
    if (argsP->getFileDataP()) {
        dataFiler(argsP->getFileDataP()->getFileDataName(),
                  argsP->getFileDataP()->getDelimiter(),
                  argsP->getRowP()->getDataRowTotal());
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
        const Delimitation dataDlmType,
        const tuple<bool, size_t, size_t> dataTimestepRange) {
    cout<< left << '\n' << string(55, '=') << "\n "
        << "Input file: " << fileInName << '\n' << string(55, '=') << "\n\n"
        << setw(20) << " Total columns:" << dataColTotal << '\n'
        << setw(20) << " Total rows:" << dataRowTotal
        << '\n' << setw(20) << " Data delimitation:"
        << ((dataDlmType == Delimitation::delimited) ? "delimiter" :
            ((dataDlmType == Delimitation::spacedAndDelimited) ?
                "whitespace and delimiter" : "whitespace")) << '\n' << endl;

    if (std::get<0>(dataTimestepRange)) {
        cout<< " Timestep column:\n" << string(30, '-') << '\n'
            << setw(3) << right << IntV::getOneP(0)->getColNo() << ". "
            << left << IntV::getOneP(0)->getColName() << '\n'
            << "\n Available timestep range is from "
            << std::get<1>(dataTimestepRange) << " to "
            << std::get<2>(dataTimestepRange) << ".\n\n";
    }
    else {
        cout<< " No consistent timestep column was found.\n\n";
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
        const bool timestepConsistent, const tuple<size_t,size_t> timestepRange,
        const vector<int>& doubleColSet,
        const vector<CmdArgs::CalcId>& calcIdSet) {
    size_t rBgn, rEnd, tBgn, tEnd;
    tie(rBgn, rEnd) = rowRange;
    tie(tBgn, tEnd) = timestepRange;

    // Set cout properties
    cout.setf(ios_base::scientific);
    cout.precision(numeric_limits<double>::max_digits10);

    // Print heading
    cout<< " Calculation results for:\n"
        << " Rows      => " << to_string(rBgn) << " to " << to_string(rEnd)
        <<'\n';
    if (timestepConsistent) {
        cout << " Timesteps => " << to_string(tBgn) << " to " << to_string(tEnd)
        <<'\n';
    }
    cout<< string(55, '=') << '\n';

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
                << " = " << calc(colNo, rBgn, rEnd) << '\n';
        }
    }
    cout << endl;
}

/*
 * Perform all the selected operations on all the selected columns and print the
 * results to the terminal.
 */
void Output::filer(const string& fileOutName,  const string& fileInName,
        const tuple<size_t, size_t> rowRange,
        const bool timestepConsistent, const tuple<size_t,size_t> timestepRange,
        const vector<int>& doubleColSet,
        const vector<CmdArgs::CalcId>& calcIdSet) {
    size_t rBgn, rEnd, tBgn, tEnd;
    tie(rBgn, rEnd) = rowRange;
    tie(tBgn, tEnd) = timestepRange;

    // Open file and set properties
    ofstream fOut;
    fOut.open(fileOutName, ios_base::app);
    if(!fOut) { throw runtime_error(errorOutputFile); }
    fOut.setf(ios_base::scientific);
    fOut.precision(numeric_limits<double>::max_digits10);

    // File heading
    fOut<< "\nInput file: " << fileInName
        << "\nCalculation results for:\n"
        << " Rows      => " << to_string(rBgn) << " to " << to_string(rEnd)
        <<'\n';
    if (timestepConsistent) {
        fOut << " Timesteps => " << to_string(tBgn) << " to " << to_string(tEnd)
        <<'\n';
    }
    fOut<< string(70, '`') << '\n';

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
            fOut << calc(colNo, rBgn, rEnd) << ',';
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
