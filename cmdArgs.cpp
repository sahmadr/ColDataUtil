/**
 * @file        cmdArgs.cpp
 *
 * @project     ColDataUtil
 * @version     0.4
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
  m_delimiterP{nullptr}, m_fileInP{nullptr}, m_calcP{nullptr},
  m_columnP{nullptr}, m_rowP{nullptr}, m_timestepP{nullptr}, m_cycleP{nullptr},
  m_fileOutP{nullptr}, m_printDataP{nullptr}, m_fileDataP{nullptr},
  m_versionP{nullptr} {
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
                    case Option::calculation:
                        if (!m_calcP) {
                            m_calcP = new Calc(s_c, m_argc, m_argv);
                        }
                        else {
                            m_calcP->init(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::column:
                        if (!m_columnP) {
                            m_columnP = new Column(s_c, m_argc, m_argv);
                        }
                        else { m_columnP->init(s_c, m_argc, m_argv);}
                        break;
                    case Option::row:
                        if (!m_rowP) {
                            m_rowP = new Row(s_c, m_argc, m_argv);
                        }
                        else if (!std::get<1>(m_rowP->getDefStatus())) {
                            m_rowP->setRowEnd(s_c, m_argc, m_argv);
                        }
                        else {
                            throw invalid_argument(errorRowsAlreadySpecified);
                        }
                        break;
                    case Option::cycle:
                        if (!m_cycleP) {
                            m_cycleP = new Cycle(s_c, m_argc, m_argv);
                        }
                        else {
                            m_cycleP->init(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::timestep:
                        if (!m_timestepP) {
                            m_timestepP = new Timestep(s_c, m_argc, m_argv);
                        }
                        else if (!std::get<1>(m_timestepP->getDefStatus())) {
                            m_timestepP->setTimestepEnd(s_c, m_argc, m_argv);
                        }
                        else {
                            throw invalid_argument(
                                errorTimestepsAlreadySpecified);
                        }
                        break;
                    case Option::fileOut:
                        if (!m_fileOutP) {
                            m_fileOutP = new FileOut(s_c, m_argc, m_argv);
                        }
                        else {
                            m_fileOutP->init(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::printData:
                        if (!m_printDataP) {
                            m_printDataP = new PrintData(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::fileData:
                        if (!m_fileDataP) {
                            m_fileDataP = new FileData(s_c, m_argc, m_argv);
                        }
                        break;
                    case Option::help:
                        break;
                    case Option::version:
                        if (!m_versionP) {
                            m_versionP = new Version();
                        }
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
    // Before loading file ---------------------------------------------------//
    if (m_versionP) { return; }
    if (!m_delimiterP) { m_delimiterP = new Delimiter(); }
    if (!m_fileInP) { throw invalid_argument(errorFileInMissing); }
    if (!m_calcP && (m_columnP || m_timestepP || m_rowP || m_fileOutP)) {
        m_calcP = new Calc();
    }
    if (!m_columnP) { m_columnP = new Column(); }
    // if (m_rowP && m_timestepP) { throw logic_error(errorRowTimestepConflict); }
    if (!m_rowP) { m_rowP = new Row(); }
    if (!m_timestepP) { m_timestepP = new Timestep(); }

    // Load file and save the returned parameters ----------------------------//
    int dataColTotal;
    size_t dataRowTotal;
    Delimitation dataDlmType;
    tuple<bool, size_t, size_t> dataTimestepRange;

    tie(dataDlmType, dataColTotal, dataRowTotal, dataTimestepRange)
        = ColData::loadData(m_fileInP->getFileLocation(),
                            m_delimiterP->getDelimiter());

    m_fileInP->importDataDlmType(dataDlmType);
    m_columnP->importDataColTotal(dataColTotal);
    m_rowP->importDataRowTotal(dataRowTotal);
    m_timestepP->importDataTimestepRange(dataTimestepRange);

    // After loading file ----------------------------------------------------//
    // Mandatory argument members
    if (m_calcP) { m_calcP->process(); }
    m_columnP->process();
    m_rowP->process();
    m_timestepP->process();
    resolveRowVsTimestep();

    // Optional argument members
    if (m_fileOutP) { m_fileOutP->process(m_fileInP->getFileLocation()); }
    // m_printDataP: No processing needed.
    if (m_fileDataP) { m_fileDataP->process(m_fileInP->getFileLocation()); }
}

int Args::setCount(int newCount)                { return(s_c = newCount); }
int Args::getCount()                            { return s_c; }
int Args::getArgC() const                       { return m_argc; }
const vector<string>& Args::getArgV() const     { return m_argv; }
const string Args::getProgramName() const       { return m_programName; }

const Delimiter* Args::getDelimiterP() const    { return m_delimiterP; }
const FileIn* Args::getFileInP() const          { return m_fileInP; }
const Calc* Args::getCalcP() const              { return m_calcP; }
const Column* Args::getColumnP() const          { return m_columnP; }
const Row* Args::getRowP() const                { return m_rowP; }
const Timestep* Args::getTimestepP() const      { return m_timestepP; }
const FileOut* Args::getFileOutP() const        { return m_fileOutP; }
const PrintData* Args::getPrintDataP() const    { return m_printDataP; }
const FileData* Args::getFileDataP() const      { return m_fileDataP; }
const Version* Args::getVersionP() const        { return m_versionP; }

void Args::resolveRowVsTimestep() {
    bool rBgnDef, rEndDef, tBgnDef, tEndDef;
    tie(rBgnDef, rEndDef) = m_rowP->getDefStatus();
    tie(tBgnDef, tEndDef) = m_timestepP->getDefStatus();

    if ((rBgnDef && tBgnDef) || (rEndDef && tEndDef)) {
        throw logic_error(errorRowTimestepConflict);
    }

    if (!rBgnDef && !tBgnDef) {
        m_timestepP->setTimestepBgn(
            std::get<1>(m_timestepP->getDataTimestepRange()));
    }
    else if (tBgnDef) {
        m_rowP->setRowBgn(
            IntV::getOneP(0)->getRow(std::get<0>(m_timestepP->getRange())));
    }
    else {
        if (m_timestepP->isTimestepConsistent()) {
            m_timestepP->setTimestepBgn(
                IntV::getOneP(0)->getData()[std::get<0>(m_rowP->getRange())]);
        }
    }

    if (!rEndDef && !tEndDef) {
        m_rowP->setRowEnd(m_rowP->getDataRowTotal() - 1);
        m_timestepP->setTimestepEnd(
            std::get<2>(m_timestepP->getDataTimestepRange()));
    }
    else if (tEndDef) {
        m_rowP->setRowEnd(
            IntV::getOneP(0)->getRow(std::get<1>(m_timestepP->getRange())));
    }
    else {
        if (m_timestepP->isTimestepConsistent()) {
            m_timestepP->setTimestepEnd(
                IntV::getOneP(0)->getData()[std::get<1>(m_rowP->getRange())]);
        }
    }
}

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

void FileIn::importDataDlmType(Delimitation dataDlmType) {
    m_dataDlmType = dataDlmType;
}

const string& FileIn::getFileLocation() const   { return m_fileLocation; }
Delimitation FileIn::getDataDlmType() const     { return m_dataDlmType; }

//----------------------------------------------------------------------------//
//****************************** CmdArgs::Calc *******************************//
//----------------------------------------------------------------------------//

Calc::Calc(int c, int argC, const vector<string>& argV) {
    init(c, argC, argV);
}
void Calc::init(int c, int argC, const vector<string>& argV) {
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
    init(c, argC, argV);
}
void Column::init(int c, int argC, const vector<string>& argV) {
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
void Column::importDataColTotal(int dataColTotal) {
    m_dataColTotal = dataColTotal;
}
void Column::process() {
    // If no number or name is entered after option flag, select all columns
    if (m_intInputColSet.empty() && m_strInputColSet.empty()) {
        for (ColData::DoubleV* dVP : ColData::DoubleV::getSetP()) {
            m_dataDoubleColSet.push_back(dVP->getColNo());
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
            if (colExists) { m_dataDoubleColSet.push_back(dVP->getColNo()); }
        }
        // If the none of the inputs match any column in the given file
        if (m_dataDoubleColSet.empty()) {
            throw invalid_argument(errorColNamesInvalid);
        }
    }
}
const vector<int>& Column::getIntInputColSet() const {
    return m_intInputColSet;
}
const vector<string>& Column::getStrInputColSet() const {
    return m_strInputColSet;
}
int Column::getDataColTotal() const {
    return m_dataColTotal;
}
const vector<int>& Column::getDataDoubleColSet() const {
    return m_dataDoubleColSet;
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
            m_rowBgnDefined = m_rowEndDefined = true;
            Args::setCount(c+2);
        }
        else {
            if (stoi(argV[c+1]) < 0) {
                throw invalid_argument(errorRowRangeInvalid);
            }
            m_rowBgn = stoi(argV[Args::setCount(++c)]);
            m_rowBgnDefined = true;
        }
    }
}
void Row::setRowEnd(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        if (stoi(argV[c+1]) <= 0) {
            throw invalid_argument(errorRowRangeInvalid);
        }
        m_rowEnd = stoi(argV[Args::setCount(++c)]);
        m_rowEndDefined = true;
    }
}
void Row::setRowBgn(size_t val) { m_rowBgn = val; }
void Row::setRowEnd(size_t val) { m_rowEnd = val; }
void Row::importDataRowTotal(size_t dataRowTotal) {
    m_dataRowTotal = dataRowTotal;
}
void Row::process() {
    if (m_rowBgnDefined) {
        if (m_rowBgn >= (m_dataRowTotal - 1)) {
            throw invalid_argument(errorRowRangeInvalid);
        }
    }
    if (m_rowEndDefined) {
        if (m_rowEnd > (m_dataRowTotal - 1)) {
            throw invalid_argument(errorRowRangeInvalid);
        }
    }
    if (m_rowBgnDefined && m_rowEndDefined && m_rowBgn >= m_rowEnd) {
        throw invalid_argument(errorRowRangeInvalid);
    }
}
size_t Row::getDataRowTotal() const { return m_dataRowTotal; }
tuple<size_t, size_t> Row::getRange() const { return {m_rowBgn, m_rowEnd}; }
tuple<bool, bool> Row::getDefStatus() const {
    return {m_rowBgnDefined, m_rowEndDefined};
}

//----------------------------------------------------------------------------//
//**************************** CmdArgs::Timestep *****************************//
//----------------------------------------------------------------------------//

Timestep::Timestep(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        if (c+2 < argC && argV[c+2][0] != '-') {
            if (stoi(argV[c+1]) < 0 || stoi(argV[c+2]) <= 0){
                throw invalid_argument(errorTimestepRangeInvalid);
            }
            m_timestepBgn = stoi(argV[c+1]),
            m_timestepEnd = stoi(argV[c+2]);
            m_timestepBgnDefined = m_timestepEndDefined = true;
            Args::setCount(c+2);
        }
        else {
            if (stoi(argV[c+1]) < 0) {
                throw invalid_argument(errorTimestepRangeInvalid);
            }
            m_timestepBgn = stoi(argV[Args::setCount(++c)]);
            m_timestepBgnDefined = true;
        }
    }
}
void Timestep::setTimestepEnd(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        if (stoi(argV[c+1]) <= 0) {
            throw invalid_argument(errorTimestepRangeInvalid);
        }
        m_timestepEnd = stoi(argV[Args::setCount(++c)]);
        m_timestepEndDefined = true;
    }
}
void Timestep::setTimestepBgn(size_t val) { m_timestepBgn = val; }
void Timestep::setTimestepEnd(size_t val) { m_timestepEnd = val; }
void Timestep::importDataTimestepRange(
        tuple<bool, size_t, size_t> dataTimestepRange) {
    m_dataTimestepRange = dataTimestepRange;
}
void Timestep::process() {
    if (std::get<0>(m_dataTimestepRange)) {// is input timestep range consistent
        if (m_timestepBgnDefined) {
            if (m_timestepBgn < std::get<1>(m_dataTimestepRange)) {
                throw invalid_argument(errorTimeStepTooSmall);
            }
            if (m_timestepBgn >= std::get<2>(m_dataTimestepRange)) {
                throw invalid_argument(errorTimeStepTooLarge);
            }
        }
        // else {
        //     m_timestepBgn = std::get<1>(m_dataTimestepRange);
        // }
        if (m_timestepEndDefined) {
            if (m_timestepEnd <= std::get<1>(m_dataTimestepRange)) {
                throw invalid_argument(errorTimeStepTooSmall);
            }
            if (m_timestepEnd > std::get<2>(m_dataTimestepRange)) {
                throw invalid_argument(errorTimeStepTooLarge);
            }
        }
        // else {
        //     m_timestepEnd = std::get<2>(m_dataTimestepRange);
        // }
        if (m_timestepBgnDefined && m_timestepEndDefined
            && m_timestepBgn >= m_timestepEnd) {
            throw invalid_argument(errorTimestepRangeInvalid);
        }
        m_timestepConsistent = true;
    }
    else {
        if (m_timestepBgnDefined || m_timestepEndDefined) {
            throw logic_error(errorDataTimestepInconsistent);
        }
    }
}
tuple<bool, size_t, size_t> Timestep::getDataTimestepRange() const {
    return m_dataTimestepRange;
}
tuple<size_t, size_t> Timestep::getRange() const {
    return {m_timestepBgn, m_timestepEnd};
}
tuple<bool, bool> Timestep::getDefStatus() const {
    return {m_timestepBgnDefined, m_timestepEndDefined};
}
bool Timestep::isTimestepConsistent() const { return m_timestepConsistent; }

//----------------------------------------------------------------------------//
//****************************** CmdArgs::Cycle ******************************//
//----------------------------------------------------------------------------//

Cycle::Cycle(int c, int argC, const vector<string>& argV) {
    init(c, argC, argV);
}
void Cycle::init(int c, int argC, const vector<string>& argV) {
    while (c+1 < argC && argV[c+1][0] != '-') {
        Args::setCount(++c);
        // std::unordered_map<string, CycleId>::const_iterator
        //     mapIt{mapStrToCycle.find(argV[c])};
        // if (mapIt != mapStrToCycle.end()) {
        //     if (std::find(m_calcIdSet.begin(), m_calcIdSet.end(),
        //             mapIt->second) == m_calcIdSet.end()) {
        //         m_calcIdSet.push_back(mapIt->second);
        //     }
        // }
        // else { throw invalid_argument(errorCycleNameInvalid); }
    }
}
void Cycle::process() {
    // if (m_calcIdSet.empty()) {
    //     m_calcIdSet.insert(m_calcIdSet.end(), {CycleId::findMin,
    //         CycleId::findMax, CycleId::findAbsMin, CycleId::findAbsMax,
    //         CycleId::findMean, CycleId::findQuadraticMean,
    //         CycleId::findCubicMean});
    // }
}
CycleInit Cycle::getCycleIdSet() const { return m_cycleInit; }

//----------------------------------------------------------------------------//
//***************************** CmdArgs::FileOut *****************************//
//----------------------------------------------------------------------------//

FileOut::FileOut(int c, int argC, const vector<string>& argV) {
    init(c, argC, argV);
}
void FileOut::init(int c, int argC, const vector<string>& argV) {
    while (c+1 < argC && argV[c+1][0] != '-') {
        Args::setCount(++c);
        m_fileOutLocSet.push_back(argV[c]+".csv");
    }
}
void FileOut::process(const string& fileInName) {
    if (m_fileOutLocSet.empty()) {
        string fileOutName;
        size_t pos;
        if ((pos = fileInName.find_last_of('.')) != string::npos) {
            fileOutName = fileInName.substr(0, pos) + "_calc.csv";
        }
        else {
            fileOutName = fileInName + "_calc.csv";
        }
        m_fileOutLocSet.push_back(fileOutName);
    }
}
const vector<string> FileOut::getFileLocSet() const {
    return m_fileOutLocSet;
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
//**************************** CmdArgs::FileData *****************************//
//----------------------------------------------------------------------------//

FileData::FileData(int c, int argC, const vector<string>& argV) {
    if (c+1 < argC && argV[c+1][0] != '-') {
        m_delimiter = argV[Args::setCount(++c)];
    }
}
void FileData::process(const string& fileInName) {
    size_t pos;
    if ((pos = fileInName.find_last_of('.')) != string::npos) {
        m_fileDataName = fileInName.substr(0, pos) + "_data.dat";
    }
    else {
        m_fileDataName = fileInName + "_data.dat";
    }
}
const string& FileData::getFileDataName() const { return m_fileDataName; }
const string& FileData::getDelimiter() const    { return m_delimiter; }

//----------------------------------------------------------------------------//
//***************************** CmdArgs::Version *****************************//
//----------------------------------------------------------------------------//

Version::Version() :
  m_msg{m_program + ' ' + m_version + '\n'
        + m_copyright + ' ' + m_author + " (" + m_email + ")\n\n"} {}

const string& Version::getMsg() const { return m_msg; }

//----------------------------------------------------------------------------//