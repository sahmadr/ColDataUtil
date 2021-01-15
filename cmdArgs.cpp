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
  m_dataDlmType{Delimitation::undefined}, m_dataColTotal{0},
  m_delimiterP{nullptr}, m_fileInP{nullptr}, m_calcP{nullptr}, m_columnP{nullptr},
  m_rowP{nullptr}, m_timestepP{nullptr}, m_fileOutP{nullptr},
  m_printDataP{nullptr} {
    if (argc<=1) { throw logic_error(errorNoArguments); }
    for (s_c=1; s_c<m_argc; ++s_c) {
        if (m_argv[s_c][0] == '-') {
            unordered_map<string, Option>::const_iterator
                mapIt{mapStrToOption.find(m_argv[s_c])};
            if (mapIt != mapStrToOption.end()) {
                switch (mapIt->second) {
                    case Option::delimiter:
                        if (!m_delimiterP) {
                            m_delimiterP = new Delimiter(s_c, m_argv);
                        }
                        else {throw invalid_argument(errorDlmSpecifiedAlready);}
                        break;
                    case Option::fileIn:
                        if (!m_fileInP) {
                            m_fileInP = new FileIn(s_c, m_argc, m_argv);
                        }
                        else { throw invalid_argument(errorFileInNamedAlready);}
                        break;
                    case Option::function:
                        cout << "Found key = function" << endl;
                        if (!m_calcP) {
                            m_calcP = new Calc(s_c, m_argc, m_argv);
                        }
                        else {
                            m_calcP->setCalcIdSet(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::column:
                        cout << "Found key = column" << endl;
                        if (!m_columnP) {
                            m_columnP = new Column(s_c, m_argc, m_argv);
                        }
                        else { m_columnP->setColInputSets(s_c, m_argc, m_argv);}
                        break;
                    case Option::row:
                        cout << "Found key = row" << endl;
                        if (!m_rowP) {
                            m_rowP = new Row(s_c, m_argc, m_argv);
                        }
                        else if (std::get<1>(m_rowP->getRowRange()) == 0) {
                            m_rowP->setRowEnd(s_c, m_argc, m_argv);
                        }
                        else {
                            throw invalid_argument(errorRowsAlreadySpecified);
                        }
                        break;
                    case Option::timestep:
                        cout << "Found key = timestep" << endl;
                        if (!m_timestepP) {
                            m_timestepP = new Timestep(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::fileOut:
                        cout << "Found key = fileOut" << endl;
                        if (!m_fileOutP) {
                            m_fileOutP = new FileOut(s_c, m_argc, m_argv);
                        }
                        else {
                            m_fileOutP->setFileLocSet(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::printData:
                        cout << "Found key = printData" << endl;
                        if (!m_printDataP) {
                            m_printDataP = new PrintData(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::help:
                        cout << "Found key = help" << endl;
                        break;
                    case Option::version:
                        cout << "Found key = version" << endl;
                        break;
                }
            }
            else { throw invalid_argument(errorInvalidOption); }
        }
        else if (s_c == 1) {
            if (!m_fileInP) { m_fileInP = new FileIn(m_argv); }
            else { throw invalid_argument(errorFileInNamedAlready); }
        }
        else { throw invalid_argument(errorInvalidValue); }
    }
}

/*
 * Check all the argument types, mainly ensuring that no necessary arguments are
 * left empty and that the specified arguments are valid, especially columns and
 * rows. Also, read the input file, create the column vectors and populate them.
 * All the called methods belong to Args class of CmdArgs namespace, which in
 * turn may or may not call the ColData namespace methods.
 */
void Args::process() {
    // Mandatory argument members (types)
    if (!m_delimiterP) { m_delimiterP = new Delimiter(); }

    if (!m_fileInP) { throw invalid_argument(errorFileInMissing); }
    tie(m_dataColTotal, m_dataDlmType) =
        m_fileInP->process(m_delimiterP->getDelimiter());

    if (!m_calcP && (m_columnP || m_timestepP || m_rowP || m_fileOutP)) {
        m_calcP = new Calc();
        m_calcP->process();
    }

    if (!m_columnP) { m_columnP = new Column(); }
    m_columnP->process();

    if (m_timestepP && m_rowP) {
        throw logic_error(errorRowTimestepConflict);
    }
    else if (m_timestepP) {
        //m_timestepP->process();
    }
    else {
        if (!m_rowP) {
            m_rowP = new Row();
        }
        m_rowP->process();
    }

    // Optional argument members (types)
    if (m_fileOutP) { m_fileOutP->process(m_fileInP->getFileLocation()); }
    // No processing needed for m_printDataP
}

int Args::setCount(int newCount)                { return(s_c = newCount); }
int Args::getCount()                            { return s_c; }
int Args::getArgC() const                       { return m_argc; }
const vector<string>& Args::getArgV() const     { return m_argv; }
const string Args::getProgramName() const       { return m_programName; }
Delimitation Args::getDataDlmType() const       { return m_dataDlmType; }
int Args::getDataColTotal() const               { return m_dataColTotal; }

const Delimiter* Args::getDelimiterP() const    { return m_delimiterP; }
const FileIn* Args::getFileInP() const          { return m_fileInP; }
const Calc* Args::getCalcP() const              { return m_calcP; }
const Column* Args::getColumnP() const          { return m_columnP; }
const Row* Args::getRowP() const                { return m_rowP; }
const Timestep* Args::getTimestepP() const      { return m_timestepP; }
const FileOut* Args::getFileOutP() const        { return m_fileOutP; }
const PrintData* Args::getPrintDataP() const    { return m_printDataP; }


//----------------------------------------------------------------------------//
//**************************** CmdArgs::Delimiter ****************************//
//----------------------------------------------------------------------------//

Delimiter::Delimiter(int c, const vector<string>& argV) {
    m_delimiter = argV[Args::setCount(++c)];
}
const string& Delimiter::getDelimiter() const { return m_delimiter; }

//----------------------------------------------------------------------------//
//***************************** CmdArgs::FileIn ******************************//
//----------------------------------------------------------------------------//

FileIn::FileIn(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        m_fileLocation = argV[Args::setCount(++c)];
    }
    else { throw invalid_argument(errorFileInNameMissing); }
}
FileIn::FileIn(const vector<string>& argV) {
    m_fileLocation = argV[Args::setCount(1)];
}
tuple<int, Delimitation> FileIn::process(const string& dlm) {
    return ColData::loadData(m_fileLocation, dlm);
}
const string& FileIn::getFileLocation() const { return m_fileLocation; }

//----------------------------------------------------------------------------//
//****************************** CmdArgs::Calc *******************************//
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
        if (inputIsInteger) { m_intInputColSet.push_back(stoi(inputStr)); }
        else { m_strInputColSet.push_back(inputStr); }
    }
}
void Column::process() {
    // If no number or name is entered after option flag, select all columns
    if (m_intInputColSet.empty() && m_strInputColSet.empty()) {
        for (ColData::DoubleV* dVP : ColData::DoubleV::getSetP()) {
            m_doubleColSet.push_back(dVP->getColNo());
        }
    }
    else {
        // Check that it is NOT an integer column
        for (ColData::IntV* iVP : ColData::IntV::getSetP()) {
            bool colExists{0};
            for (int intInput : m_intInputColSet) {
                colExists = (intInput == iVP->getColNo());
            }
            if (!colExists) {
                for (string& strInput : m_strInputColSet) {
                    colExists = (strInput == iVP->getColName());
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
            if (colExists) { m_doubleColSet.push_back(dVP->getColNo()); }
        }
        // If the none of the inputs match any column in the given file
        if (m_doubleColSet.empty()) {
            throw invalid_argument(errorColNamesInvalid);
        }
    }
}
const vector<int>& Column::getDoubleColSet() const {
    return m_doubleColSet;
}
const vector<int>& Column::getIntInputColSet() const {
    return m_intInputColSet;
}
const vector<string>& Column::getStrInputColSet() const {
    return m_strInputColSet;
}

//----------------------------------------------------------------------------//
//******************************* CmdArgs::Row *******************************//
//----------------------------------------------------------------------------//

Row::Row(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        if (c+2 < argC && argV[c+2][0] != '-') {
            if (stoi(argV[c+1]) < 0 || stoi(argV[c+2]) <= 0){
                throw invalid_argument(errorRowRangeInvalid);
            }
            m_rowBgn = stoi(argV[c+1]),
            m_rowEnd = stoi(argV[c+2]);
            Args::setCount(c+2);
        }
        else {
            if (stoi(argV[c+1]) < 0) {
                throw invalid_argument(errorRowRangeInvalid);
            }
            m_rowBgn = stoi(argV[Args::setCount(++c)]);
        }
    }
}
void Row::setRowEnd(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        if (stoi(argV[c+1]) <= 0) {
            throw invalid_argument(errorRowRangeInvalid);
        }
        m_rowEnd = stoi(argV[Args::setCount(++c)]);
    }
}
void Row::process() {
    if (m_rowEnd == 0) {
        m_rowEnd = ColData::DoubleV::getOneP(0)->getData().size() - 1;
    }
    else if (m_rowEnd > (ColData::DoubleV::getOneP(0)->getData().size() - 1)
            || (m_rowEnd <= m_rowBgn)) {
        throw invalid_argument(errorRowRangeInvalid);
    }
}
const tuple<size_t, size_t> Row::getRowRange() const {
    return {m_rowBgn, m_rowEnd};
}

//----------------------------------------------------------------------------//
//**************************** CmdArgs::Timestep *****************************//
//----------------------------------------------------------------------------//

Timestep::Timestep(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        if (c+2 < argC && argV[c+2][0] != '-') {
            if (stoi(argV[c+1]) < 0 || stoi(argV[c+2]) <= 0){
                throw invalid_argument(errorRowRangeInvalid);
            }
            m_timestepBgn = stoi(argV[c+1]),
            m_timestepEnd = stoi(argV[c+2]);
            Args::setCount(c+2);
        }
        else { m_timestepBgn = stoi(argV[Args::setCount(++c)]); }
    }
}
void Timestep::process() {
    if (m_timestepEnd == 0) {
        // m_timestepEnd = ColData::DoubleV::getOneP(0)->getData().size();
    }
    else if (m_timestepEnd > ColData::DoubleV::getOneP(0)->getData().size()
            || (m_timestepEnd <= m_timestepBgn)) {
        throw invalid_argument(errorRowRangeInvalid);
    }
    cout << "timestepBgn = " << m_timestepBgn << endl;
    cout << "timestepEnd = " << m_timestepEnd << endl;
}

//----------------------------------------------------------------------------//
//***************************** CmdArgs::FileOut *****************************//
//----------------------------------------------------------------------------//

FileOut::FileOut(int c, int argC, const vector<string>& argV) {
    setFileLocSet(c, argC, argV);
}
void FileOut::setFileLocSet(int c, int argC, const vector<string>& argV) {
    while (c+1 < argC && argV[c+1][0] != '-') {
        Args::setCount(++c);
        m_fileLocSet.push_back(argV[c]);
    }
}
void FileOut::process(const string& fileInName) {
    if (m_fileLocSet.empty()) {
        string fileOutName {fileInName + "_calc.csv"};
        m_fileLocSet.push_back(fileOutName);
    }
}
const vector<string> FileOut::getFileLocSet() const {
    return m_fileLocSet;
}

//----------------------------------------------------------------------------//
//**************************** CmdArgs::PrintData ****************************//
//----------------------------------------------------------------------------//

PrintData::PrintData(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        m_delimiter = argV[Args::setCount(++c)];
    }
}
const string& PrintData::getDelimiter() const { return m_delimiter; }

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


//----------------------------------------------------------------------------//
/*
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
*/