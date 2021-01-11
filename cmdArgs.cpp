/**
 * @file        cmdArgs.cpp
 *
 * @project     colDataUtil
 * @version     0.2
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       Read command line input options.
 *
 */

#include "namespaces.h"
#include "cmdArgs.h"
#include "mappings.h"
#include "errorMsgs.h"

using namespace CmdArgs;

//----------------------------------------------------------------------------//
//****************************** CmdArgs::Args *******************************//
//----------------------------------------------------------------------------//

Args::Args(int argc, char* argv[]) :
m_argc{argc}, m_argv{argv, argv+argc}, m_programName{argv[0]},
m_fileInP{nullptr}, m_calcP{nullptr}, m_columnP{nullptr}, m_rowP{nullptr},
m_timestepP{nullptr}, m_fileOutP{nullptr}, m_printDataP{nullptr} {
    if (argc<=1) { throw std::logic_error(errorNoArguments); }
    for (s_c=1; s_c<m_argc; ++s_c) {
        if (m_argv[s_c][0] == '-') {
            std::unordered_map<string, Option>::const_iterator
                mapIt{mapStrToOption.find(m_argv[s_c])};
            if (mapIt != mapStrToOption.end()) {
                switch (mapIt->second) {
                    case Option::fileIn:
                        if (!m_fileInP) {
                            m_fileInP = new FileIn(s_c, m_argc, m_argv);
                        }
                        else { throw invalid_argument(errorFileInNamedAlready);}
                        break;
                    case Option::function:
                        cout << "Found key = function" << std::endl;
                        if (!m_calcP) {
                            m_calcP = new Calc(s_c, m_argc, m_argv);
                        }
                        else {
                            m_calcP->setCalcIdSet(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::column:
                        cout << "Found key = column" << std::endl;
                        if (!m_columnP) {
                            m_columnP = new Column(s_c, m_argc, m_argv);
                        }
                        else { m_columnP->setColInputSets(s_c, m_argc, m_argv);}
                        break;
                    case Option::row:
                        cout << "Found key = row" << std::endl;
                        if (!m_rowP) {
                        }
                        break;
                    case Option::timestep:
                        cout << "Found key = timestep" << std::endl;
                        if (!m_timestepP) {
                        }
                        break;
                    case Option::fileOut:
                        cout << "Found key = fileOut" << std::endl;
                        if (!m_fileOutP) {
                            m_fileOutP = new FileOut(s_c, m_argc, m_argv);
                        }
                        else {
                            m_fileOutP->setFileLocationSet(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::printData:
                        cout << "Found key = printData" << std::endl;
                        if (!m_printDataP) {
                            m_printDataP = new PrintData(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::help:
                        cout << "Found key = help" << std::endl;
                        break;
                    case Option::version:
                        cout << "Found key = version" << std::endl;
                        break;
                }
            }
            else { throw invalid_argument(errorInvalidOption); }
        }
        else if (s_c == 1) {
            if (!m_fileInP) { m_fileInP = new FileIn(m_argc, m_argv); }
            else { throw invalid_argument(errorFileInNamedAlready); }
        }
        else { throw invalid_argument(errorInvalidValue); }
    }
}
void Args::process() {
    if (!m_fileInP) { throw invalid_argument(errorFileInMissing); }
    m_fileInP->process();

    if (!m_columnP) { m_columnP = new Column(); }
    m_columnP->process();

    if (!m_calcP && (m_columnP || m_timestepP || m_rowP || m_fileOutP)) {
        m_calcP = new Calc();
    }
    m_calcP->process();

    if (m_printDataP) { m_printDataP->process(); }

    if (m_fileOutP) { m_fileOutP->process(m_fileInP->getFileLocation()); }
}
void Args::output() {
    ColData::printColNames();

    if (m_printDataP) { ColData::printData(m_printDataP->getDelimiter()); }

    if (m_calcP) {
        if (!m_fileOutP) {
            for (const int colNo : m_columnP->getColSet()) {
                for (const CalcId id : m_calcP->getCalcIdSet()) {
                    ColData::outputValue(
                        mapCalcIdToCalc<int>.at(id), colNo);
                }
            }
        }
        else {
            for (const string& fileOut : m_fileOutP->getFileLocationSet()) {
                for (const int colNo : m_columnP->getColSet()) {
                    for (const CalcId id : m_calcP->getCalcIdSet()) {
                        ColData::outputValue(fileOut,
                            mapCalcIdToCalc<int>.at(id), colNo);
                    }
                }
                cout<< "The output has been written to the file \""
                    << fileOut << "\"." << std::endl;
            }
        }
    }
}
int Args::setCount(int newCount)            { return(s_c = newCount); }
int Args::getCount()                        { return s_c; }
int Args::getArgC() const                   { return m_argc; }
const vector<string>& Args::getArgV() const { return m_argv; }
const string Args::getProgramName() const   { return m_programName; }

//----------------------------------------------------------------------------//
//***************************** CmdArgs::FileIn ******************************//
//----------------------------------------------------------------------------//

FileIn::FileIn(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        if (c+2 < argC && argV[c+2][0] != '-') {
            m_fileLocation = argV[c+1],
            m_delimiter = argV[c+2];
            Args::setCount(c+2);
        }
        else { m_fileLocation = argV[Args::setCount(++c)]; }
    }
    else { throw invalid_argument(errorFileInNameMissing); }
}
FileIn::FileIn(int argC, const vector<string>& argV) {
    if (argC > 2 && argV[2][0] != '-') {
        m_fileLocation = argV[1];
        m_delimiter = argV[2];
        Args::setCount(2);
    }
    else { m_fileLocation = argV[Args::setCount(1)]; }
}
void FileIn::process() { ColData::loadData(m_fileLocation, m_delimiter); }
const string& FileIn::getFileLocation() const { return m_fileLocation; }
const string& FileIn::getDelimiter() const { return m_delimiter; }

//----------------------------------------------------------------------------//
//***************************** CmdArgs::Calc *****************************//
//----------------------------------------------------------------------------//

Calc::Calc(int c, int argC, const vector<string>& argV) {
    setCalcIdSet(c, argC, argV);
}
void Calc::setCalcIdSet(int c, int argC, const vector<string>& argV) {
    while (c+1 < argC && argV[c+1][0] != '-') {
        Args::setCount(++c);
        std::unordered_map<string, CalcId>::const_iterator
            mapIt{mapStrToCalc.find(argV[c])};
        if (mapIt != mapStrToCalc.end()) {
            if (std::find(m_calcIdSet.begin(), m_calcIdSet.end(),
                    mapIt->second) == m_calcIdSet.end()) {
                m_calcIdSet.push_back(mapIt->second);
            }
        }
        else { throw invalid_argument(errorCalcNameInvalid); }
    }
}
void Calc::process() {
    if (m_calcIdSet.empty()) {
        m_calcIdSet.insert(m_calcIdSet.end(), {CalcId::findMin,
            CalcId::findMax, CalcId::findAbsMin, CalcId::findAbsMax,
            CalcId::findMean, CalcId::findQuadraticMean,
            CalcId::findCubicMean});
    }
}
const vector<CalcId>& Calc::getCalcIdSet() const {
    return m_calcIdSet;
}

//----------------------------------------------------------------------------//
//***************************** CmdArgs::Column ******************************//
//----------------------------------------------------------------------------//

Column::Column(int c, int argC, const vector<string>& argV) {
    setColInputSets(c, argC, argV);
}
void Column::setColInputSets(int c, int argC, const vector<string>& argV) {
    while (c+1 < argC && argV[c+1][0] != '-') {
        Args::setCount(++c);
        bool inputIsInteger{true};
        const string& inputStr{argV[c]};
        for (size_t i=0; i<inputStr.length(); ++i) {
            if (!isdigit(inputStr[i])) { inputIsInteger = false; }
        }
        if (inputIsInteger) { m_intInputColSet.push_back(std::stoi(inputStr)); }
        else { m_strInputColSet.push_back(inputStr); }
    }
}
void Column::process() {
    if (m_intInputColSet.empty() && m_strInputColSet.empty()) {
        for (ColData::DoubleV* dVP : ColData::DoubleV::getSetP()) {
            m_colSet.push_back(dVP->getColNo());
        }
    }
    else {
        // Check that it is NOT an integer column
        for (ColData::IntV* iVP : ColData::IntV::getSetP()) {
            bool colExists{0};
            for (int intInput : m_intInputColSet) {
                colExists = intInput == iVP->getColNo();
            }
            if (!colExists) {
                for (string& strInput : m_strInputColSet) {
                    colExists = strInput == iVP->getColName();
                }
            }
            if (colExists) { throw invalid_argument(errorColIsInt); }
        }
        // Match the input to the respective column in the given file
        for (ColData::DoubleV* dVP : ColData::DoubleV::getSetP()) {
            bool colExists{0};
            for (int intInput : m_intInputColSet) {
                colExists = intInput == dVP->getColNo();
            }
            if (!colExists) {
                for (string& strInput : m_strInputColSet) {
                    colExists = strInput == dVP->getColName();
                }
            }
            if (colExists) { m_colSet.push_back(dVP->getColNo()); }
        }
        // If the none of the inputs match any column in the given file
        if (m_colSet.empty()) {
            throw invalid_argument(errorColNamesInvalid);
        }
    }
}
const vector<int>& Column::getColSet() const {
    return m_colSet;
}
const vector<int>& Column::getIntInputColSet() const {
    return m_intInputColSet;
}
const vector<string>& Column::getStrInputColSet() const {
    return m_strInputColSet;
}


//----------------------------------------------------------------------------//
//***************************** CmdArgs::FileOut *****************************//
//----------------------------------------------------------------------------//

FileOut::FileOut(int c, int argC, const vector<string>& argV) {
    setFileLocationSet(c, argC, argV);
}
void FileOut::setFileLocationSet(int c, int argC, const vector<string>& argV) {
    while (c+1 < argC && argV[c+1][0] != '-') {
        Args::setCount(++c);
        m_fileLocationSet.push_back(argV[c]);
    }
}
void FileOut::process(const string& fileInName) {
    if (m_fileLocationSet.empty()) {
        string fileOutName {fileInName + "_calc.csv"};
        m_fileLocationSet.push_back(fileOutName);
    }
}
const vector<string> FileOut::getFileLocationSet() const {
    return m_fileLocationSet;
}

//----------------------------------------------------------------------------//
//**************************** CmdArgs::PrintData ****************************//
//----------------------------------------------------------------------------//

PrintData::PrintData(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        m_delimiter = argV[Args::setCount(++c)];
    }
}
void PrintData::process() {
    // if (m_delimiter.length() > delimLenLimit) {
    //     std::cerr << warnDelimiterTooLong << endl;
    // }
}
const string& PrintData::getDelimiter() const { return m_delimiter; }

//----------------------------------------------------------------------------//
//******************************* CmdArgs::Row *******************************//
//----------------------------------------------------------------------------//

Row::Row(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        if (c+2 < argC && argV[c+2][0] != '-') {
            m_rowBgn = std::stoi(argV[c+1]),
            m_rowEnd = std::stoi(argV[c+2]);
            Args::setCount(c+2);
        }
        else { m_rowBgn = stoi(argV[Args::setCount(++c)]); }
    }
}

//----------------------------------------------------------------------------//
//**************************** CmdArgs::Timestep *****************************//
//----------------------------------------------------------------------------//
/*
 * Test the validity of the input timestep begin and end values, assuming that
 * the first column of the loaded data is an integer representation of the
 * number of timesteps.
 */
/*
void testInputTimestep(const size_t timestepBgn,
        const size_t timestepEnd) {
    vector<int> timesteps{IntV::getOneP(0)->getData()};
    if (timestepBgn < static_cast<size_t>(timesteps[0])) {
        throw runtime_error(errorTimeStepTooSmall);
    }
    if (timestepEnd > timesteps.size()) {
        throw runtime_error(errorTimeStepTooLarge);
    }
} */