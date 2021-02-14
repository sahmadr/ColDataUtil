/**
 * @version     ColDataUtil 1.0
 * @author      Syed Ahmad Raza (git@ahmads.org)
 * @copyright   GPLv3+: GNU Public License version 3 or later
 *
 * @file        output.cpp
 * @brief       Perform calculations and output results.
 */

#include "output.h"

//----------------------------------------------------------------------------//
//*************************** Main output function ***************************//
//----------------------------------------------------------------------------//
/*
 * Perform the output operations based on user input.
 */
void Output::output(CmdArgs::Args* argsP) {
    if (argsP->getHelpP()) {
        ifstream hF(argsP->getHelpP()->getHelpFileName());
        if (hF.is_open()) { cout << hF.rdbuf() << endl; }
        return;
    }
    if (argsP->getVersionP()) {
        cout << argsP->getVersionP()->getMsg();
        return;
    }
    printInputDataInfo(
        argsP->getFileInP()->getFileLocation(),
        argsP->getColumnP()->getDataColTotal(),
        argsP->getRowP()->getDataRowTotal(),
        argsP->getFileInP()->getDataDlmType(),
        argsP->getTimestepP()->getDataTimestepIVP(),
        argsP->getColumnP()->getDataDoubleVSetP()
    );

    // cout.setf(ios_base::scientific);
    cout.precision(numeric_limits<double>::max_digits10);

    if (argsP->getCycleP() || argsP->getCalcP()) {
        if (!argsP->getFileOutP()) {
            printer(
                argsP->getRowP()->getRange(),
                argsP->getTimestepP()->isTimestepConsistent(),
                argsP->getTimestepP()->getRange(),
                argsP->getColumnP()->getDataDoubleColSet(),
                argsP->getCalcP()->getCalcIdSet(),
                argsP->getCycleP(),
                argsP->getCalcP()
            );
        }
        else {
            for (const string& fileOut : argsP->getFileOutP()->getFileLocSet()){
                filer(
                    fileOut, argsP->getFileInP()->getFileLocation(),
                    argsP->getRowP()->getRange(),
                    argsP->getTimestepP()->isTimestepConsistent(),
                    argsP->getTimestepP()->getRange(),
                    argsP->getColumnP()->getDataDoubleColSet(),
                    argsP->getCalcP()->getCalcIdSet(),
                    argsP->getCycleP(),
                    argsP->getCalcP()
                );
                cout<< "\nThe output has been written to \"" << fileOut << "\""
                    << endl;
            }
        }
    }
    if (argsP->getFourierP()) {
        fourierFiler(argsP->getFourierP());
    }
    if (argsP->getPrintDataP()) {
        dataPrinter(
            argsP->getPrintDataP()->getDelimiter(),
            argsP->getRowP()->getDataRowTotal(),
            argsP->getTimestepP()->getDataTimestepIVP(),
            argsP->getColumnP()->getDataDoubleVSetP()
        );
    }
    if (argsP->getFileDataP()) {
        dataFiler(
            argsP->getFileDataP()->getFileDataName(),
            argsP->getFileDataP()->getDelimiter(),
            argsP->getRowP()->getDataRowTotal(),
            argsP->getTimestepP()->getDataTimestepIVP(),
            argsP->getColumnP()->getDataDoubleVSetP()
        );
        cout<< "\nThe output has been written to \""
            << argsP->getFileDataP()->getFileDataName() << "\"" << endl;
    }
    cout<<endl;
}

//----------------------------------------------------------------------------//
//******************** Printing information on loaded data *******************//
//----------------------------------------------------------------------------//
/*
 * Print the names of all the vector columns; first integers and then doubles.
 */
void Output::printInputDataInfo(const string& fileInName,
        const int dataColTotal, const size_t dataRowTotal,
        const Delimitation dataDlmType, const ColData::IntV* dataTimestepIVP,
        const vector<ColData::DoubleV*> dataDoubleVSetP) {
    cout<< left << '\n' << string(55, '=') << "\n "
        << "Input file: " << fileInName << '\n' << string(55, '=') << "\n\n"
        << setw(20) << " Total columns:" << dataColTotal << '\n'
        << setw(20) << " Total rows:" << dataRowTotal
        << '\n' << setw(20) << " Data delimitation:"
        << ((dataDlmType == Delimitation::delimited) ? "delimiter" :
            ((dataDlmType == Delimitation::spacedAndDelimited) ?
                "whitespace and delimiter" : "whitespace")) << '\n' << endl;

    if (dataTimestepIVP && get<0>(dataTimestepIVP->getTimestepRange())) {
        cout<< " Timestep column:\n" << string(30, '-') << '\n'
            << setw(3) << right << dataTimestepIVP->getColNo() << ". "
            << left << dataTimestepIVP->getColName() << '\n'
            << "\n Available timestep range is from "
            << get<1>(dataTimestepIVP->getTimestepRange()) << " to "
            << get<2>(dataTimestepIVP->getTimestepRange()) << ".\n\n";
    }
    else {
        cout<< " No consistent timestep column was found.\n\n";
    }
    cout<< " Data columns:\n"
        << string(30, '-') << "\n";
    for (DoubleV* dVP : dataDoubleVSetP) {
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
        const bool timestepConsistent,
        const tuple<size_t,size_t> timestepRange,
        const vector<int>& doubleColSet,
        const vector<CmdArgs::CalcId>& calcIdSet,
        const CmdArgs::Cycle* cycleP, const CmdArgs::Calc* calcP) {
    size_t rBgn, rEnd, tBgn, tEnd;
    tie(rBgn, rEnd) = rowRange;
    tie(tBgn, tEnd) = timestepRange;

    // Print heading
    cout<< " Calculation results\n";
    if (timestepConsistent) {
        cout<< "\n Timesteps         => "
            << to_string(tBgn) << " to " << to_string(tEnd);
    }
    cout<< "\n Rows              => "
        << to_string(rBgn) << " to " << to_string(rEnd);
    if (cycleP) {
        DoubleV* cycleColDVP{DoubleV::getOnePFromCol(cycleP->getColNo())};

        cout<< "\n Column for cycles => "<< cycleColDVP->getColName()
            << "\n Center for cycles => "  << cycleP->getCenter()
            << "\n Number of cycles  => "  << cycleP->getCycleCount();
    }
    cout<< '\n' << string(55, '=') << endl;

    if (calcP) {
        if (cycleP && cycleP->getCycleCount()==0) {
            throw logic_error(errorCycleInvalidForCalc);
        }
        for (const int colNo : doubleColSet) {
            // Print subheadings
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
        cout<< '\n' << string(55, '=') << endl;
    }
}

/*
 * Perform all the selected operations on all the selected columns and print the
 * results to the terminal.
 */
void Output::filer(const string& fileOutName,  const string& fileInName,
        const tuple<size_t, size_t> rowRange,
        const bool timestepConsistent,
        const tuple<size_t,size_t> timestepRange,
        const vector<int>& doubleColSet,
        const vector<CmdArgs::CalcId>& calcIdSet,
        const CmdArgs::Cycle* cycleP, const CmdArgs::Calc* calcP) {
    size_t rBgn, rEnd, tBgn, tEnd;
    tie(rBgn, rEnd) = rowRange;
    tie(tBgn, tEnd) = timestepRange;

    // Open file and set properties
    ofstream fOut;
    fOut.open(fileOutName, ios_base::app);
    if(!fOut) { throw runtime_error(errorOutputFile); }

    // File heading
    fOut<< "\nInput file: " << fileInName
        << "\nCalculation results";
    if (timestepConsistent) {
        fOut<< "\nTimesteps         => "
            << to_string(tBgn) << " to " << to_string(tEnd);
    }
    fOut<< "\nRows              => "
        << to_string(rBgn) << " to " << to_string(rEnd);
    if (cycleP) {
        DoubleV* cycleColDVP{DoubleV::getOnePFromCol(cycleP->getColNo())};

        fOut<< "\nColumn for cycles => "<< cycleColDVP->getColName()
            << "\nCenter for cycles => "  << cycleP->getCenter()
            << "\nNumber of cycles  => "  << cycleP->getCycleCount();
    }
    fOut<< '\n' << string(70, '`') << '\n';

    if (calcP) {
        if (cycleP && cycleP->getCycleCount()==0) {
            throw logic_error(errorCycleInvalidForCalc);
        }
        fOut.setf(ios_base::scientific);
        fOut.precision(numeric_limits<double>::max_digits10);

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
    }
    fOut << '\n';
    fOut.close();
}

//----------------------------------------------------------------------------//
//******************* Filing Fast Fourier Transform results ******************//
//----------------------------------------------------------------------------//
/*
 * File the results of Fast Fourier Transform.
 */
#include <complex>
#include "fftw3.h"

void Output::fourierFiler(const CmdArgs::Fourier* fourierP) {
    const size_t
        rowBgn{get<0>(fourierP->getRowRange())},
        rowEnd{get<1>(fourierP->getRowRange())},
        sampleFreq{rowEnd - rowBgn + 1},        // Fs
        signalLen{sampleFreq},                  // L
        outputLen{(signalLen/2)+1};
    DoubleV* fourierColDVP{DoubleV::getOnePFromCol(fourierP->getColNo())};
    vector<double> fourierColData{fourierColDVP->getData()};
    vector<std::complex<double>> fftData;
    vector<double> fftMag;
    fftData.reserve(signalLen);
    fftMag.reserve(outputLen);

    fftw_plan plan{
        fftw_plan_dft_1d(
            signalLen,
            reinterpret_cast<fftw_complex*>(&fftData.data()[0]),
            reinterpret_cast<fftw_complex*>(&fftData.data()[0]),
            FFTW_FORWARD,
            FFTW_ESTIMATE
        )
    };

    for (size_t r=0; r<signalLen; ++r) {
        using namespace std::complex_literals;
        fftData.emplace_back(0i);
        reinterpret_cast<double(&)[2]>(fftData[r])[0]
            = fourierColData[rowBgn+r];
    }

    fftw_execute(plan);

    double signalLenInv{1.0/signalLen};
    double outputLenInv{1.0/outputLen};
    // Output the data
    if (fourierP->getFileFourierName() != "no.csv") {
        ofstream fOut{fourierP->getFileFourierName()};
        fOut.precision(numeric_limits<double>::max_digits10);
        fOut<< "Frequency"  << ','
            << "Magnitude"  << ','
            << "Phase"      << ','
            << '\n';
        // double outputLenInv{1.0/(signalLen/2)};  // Matches with Teclplot
        for (size_t r=0; r<outputLen; ++r) {
            fftMag.emplace_back(2.*std::abs(fftData[r])*signalLenInv);
            fOut<< static_cast<double>(r)*outputLenInv*100. << ','
                << fftMag[r] << ','
                << std::arg(fftData[r]) << ','
                << '\n';
        }
        fOut.close();
    }
    else {
        for (size_t r=0; r<outputLen; ++r) {
            fftMag.emplace_back(2.*std::abs(fftData[r])*signalLenInv);
        }
    }

    // Print partial results
    cout<< '\n' << " FFT partial results (sorted by magnitude)"
        << '\n' << string(55, '=') << "\n\n "
        << setw(30) << "Frequency"
        << setw(30) << "Magnitude"
        // << setw(30) << "Phase"
        << "\n\n ";
    for (int n=0; n<fftValuesToPrint; ++n) {
        vector<double>::iterator maxValueIt{
            std::max_element(fftMag.begin(), fftMag.end())
        };
        std::ptrdiff_t maxIndex{std::distance(fftMag.begin(), maxValueIt)};

        cout<< setw(30) << static_cast<double>(maxIndex)*outputLenInv*100.
            << setw(30) << *maxValueIt
            // << setw(30) << std::arg(fftData[maxIndex])
            << "\n ";
        fftMag[maxIndex] = 0.0;
    }
    cout<< '\n' << string(55, '=') << '\n';
    if (fourierP->getFileFourierName() != "no.csv") {
        cout<< "\nThe output has been written to \""
            << fourierP->getFileFourierName() << "\"" << endl;
    }

    fftw_destroy_plan(plan);
}


//----------------------------------------------------------------------------//
//********************* Printing and filing loaded data **********************//
//----------------------------------------------------------------------------//
/*
 * Print the data of all the vector columns; first integers and then doubles.
 */
void Output::dataPrinter(const string& dlm, const size_t dataRowTotal,
        const ColData::IntV* dataTimestepIVP,
        const vector<ColData::DoubleV*> dataDoubleVSetP) {
    cout.setf(ios_base::scientific);
    cout.precision(numeric_limits<double>::max_digits10);

    // Print the header line
    cout << '\n';
    cout << dataTimestepIVP->getColName() << dlm;
    for (DoubleV* dVP : dataDoubleVSetP) { cout << dVP->getColName() << dlm;}
    cout << "\n\n";

    // Print the data
    for (size_t row=0; row<dataRowTotal; ++row) {
        cout << dataTimestepIVP->getData()[row] << dlm;
        for (DoubleV* dVP : dataDoubleVSetP) { cout << dVP->getData()[row] << dlm;}
        cout << '\n';
    }
    cout << flush;
}

/*
 * File the data of all the vector columns; first integers and then doubles.
 */
void Output::dataFiler(const string& fileName, const string& dlm,
        const size_t dataRowTotal, const ColData::IntV* dataTimestepIVP,
        const vector<ColData::DoubleV*> dataDoubleVSetP) {
    ofstream fOut{fileName};
    fOut.setf(ios_base::scientific);
    fOut.precision(numeric_limits<double>::max_digits10);

    // File the header line
    fOut << dataTimestepIVP->getColName() << dlm;
    for (DoubleV* dVP : dataDoubleVSetP) { fOut << dVP->getColName() << dlm; }
    fOut << '\n';

    // File the data
    for (size_t row=0; row<dataRowTotal; ++row) {
        fOut << dataTimestepIVP->getData()[row] << dlm;
        for (DoubleV* dVP : dataDoubleVSetP) {
            fOut << dVP->getData()[row] << dlm;
        }
        fOut << '\n';
    }
    fOut << flush;
    fOut.close();
}
