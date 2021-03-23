/**
 * @version     ColDataUtil 1.4
 * @author      Syed Ahmad Raza (git@ahmads.org)
 * @copyright   GPLv3+: GNU Public License version 3 or later
 *
 * @file        cmdArgs.cpp
 * @brief       Read command line input options.
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
  m_columnP{nullptr}, m_rowP{nullptr}, m_timestepP{nullptr},
  m_cycleP{nullptr}, m_fourierP{nullptr},
  m_fileOutP{nullptr}, m_printDataP{nullptr}, m_fileDataP{nullptr},
  m_helpP{nullptr}, m_versionP{nullptr} {
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
                        else if (!get<1>(m_rowP->getDefStatus())) {
                            m_rowP->setRowEnd(s_c, m_argc, m_argv);
                        }
                        else {
                            throw invalid_argument(errorRowsAlreadySpecified);
                        }
                        break;
                    case Option::timestep:
                        if (!m_timestepP) {
                            m_timestepP = new Timestep(s_c, m_argc, m_argv);
                        }
                        else if (!get<1>(m_timestepP->getDefStatus())) {
                            m_timestepP->setTimestepEnd(s_c, m_argc, m_argv);
                        }
                        else {
                            throw invalid_argument(
                                errorTimestepsAlreadySpecified);
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
                    case Option::fourier:
                        if (!m_fourierP) {
                            m_fourierP = new Fourier(s_c, m_argc, m_argv);
                        }
                        else {
                            m_fourierP->init(s_c, m_argc, m_argv);
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
                        if (!m_helpP) {
                            m_helpP = new Help();
                        }
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
    if (m_helpP) { return; }
    if (m_versionP) { return; }
    if (!m_delimiterP) { m_delimiterP = new Delimiter(); }
    if (!m_fileInP) { throw invalid_argument(errorFileInMissing); }
    if (!m_calcP && (m_columnP || m_timestepP || m_rowP || m_fileOutP)) {
        m_calcP = new Calc();
    }
    if (!m_columnP) { m_columnP = new Column(); }
    if (m_cycleP && (m_rowP || m_timestepP)) {
        throw logic_error(errorCycleRowTimestepConflict);
    }
    if (!m_rowP) { m_rowP = new Row(); }
    if (!m_timestepP) { m_timestepP = new Timestep(); }

    // Load file and save the returned parameters ----------------------------//
    tuple<Delimitation, int, size_t, IntV*, vector<DoubleV*>&> loadedFileData {
        ColData::loadData(m_fileInP->getFileLocation(),
        m_delimiterP->getDelimiter())
    };
    m_fileInP->importDataDlmType(get<0>(loadedFileData));
    m_columnP->importDataColTotal(get<1>(loadedFileData));
    m_columnP->importDataDouble(get<4>(loadedFileData));
    m_rowP->importDataRowTotal(get<2>(loadedFileData));
    m_timestepP->importDataTimestep(get<3>(loadedFileData));

    // After loading file ----------------------------------------------------//
    // Mandatory argument members
    if (m_cycleP) {
        m_cycleP->process(m_columnP->getDataDoubleVSetP());
        m_rowP->process(
            get<0>(m_cycleP->getRowDefStatus()),
            get<1>(m_cycleP->getRowDefStatus()),
            get<0>(m_cycleP->getRowDefRange()),
            get<1>(m_cycleP->getRowDefRange()),
            m_cycleP->getInitType()
        );
        m_timestepP->process(
            get<0>(m_cycleP->getTimestepDefStatus()),
            get<1>(m_cycleP->getTimestepDefStatus()),
            get<0>(m_cycleP->getTimestepDefRange()),
            get<1>(m_cycleP->getTimestepDefRange()),
            m_cycleP->getInitType(),
            m_cycleP->getTimeIncrement()
        );
    }
    else {
        m_rowP->process();
        m_timestepP->process();
    }
    resolveRowVsTimestep();
    if (m_cycleP) {
        DoubleV* cycleColDVP{
            DoubleV::getOnePFromCol(m_cycleP->getCycleColNo())
        };
        // Calculate cycle data and update
        if (m_cycleP->getInitType() == CmdArgs::CycleInit::full) {
            m_cycleP->setCalcCDataAndCycleInputCount(
                cycleColDVP->findCycles(
                    m_rowP->getRowBgn(), m_rowP->getRowEnd(),
                    m_cycleP->getCenter(), m_cycleP->getMinAmplitude(),
                    m_cycleP->getMinRowInterval()
                )
            );
        }
        else if (m_cycleP->getInitType() == CmdArgs::CycleInit::first) {
            m_cycleP->setCalcCDataAndCycleInputCount(
                cycleColDVP->findCyclesFirst(
                    m_rowP->getRowBgn(), m_rowP->getRowEnd(),
                    m_cycleP->getCenter(), m_cycleP->getMinAmplitude(),
                    m_cycleP->getMinRowInterval(), m_cycleP->getInputCount()
                )
            );
        }
        else if (m_cycleP->getInitType() == CmdArgs::CycleInit::last) {
            m_cycleP->setCalcCDataAndCycleInputCount(
                cycleColDVP->findCyclesLast(
                    m_rowP->getRowBgn(), m_rowP->getRowEnd(),
                    m_cycleP->getCenter(), m_cycleP->getMinAmplitude(),
                    m_cycleP->getMinRowInterval(), m_cycleP->getInputCount()
                )
            );
        }
        // Update rows and timesteps
        m_rowP->setRowBgn(m_cycleP->getCalcCycleData().rowInitial);
        m_rowP->setRowEnd(m_cycleP->getCalcCycleData().rowFinal);
        if (m_timestepP->isTimestepConsistent()) {
            m_timestepP->setTimestepBgnFromRow(
                m_cycleP->getCalcCycleData().rowInitial
            );
            m_timestepP->setTimestepEndFromRow(
                m_cycleP->getCalcCycleData().rowFinal
            );
        }
        // Calculate frequency
        if (m_cycleP->getSimTimeColNo()>=0) {
            m_cycleP->setFrequency(m_rowP->getRowBgn(), m_rowP->getRowEnd());
        }
        else if(m_cycleP->getTimeIncrement()>=0.0) {
            m_cycleP->setFrequency(m_timestepP->getRange());
        }
        if (m_cycleP->getFileName() == "auto") {
            m_cycleP->setAutoFileName(m_rowP->getRowBgn(), m_rowP->getRowEnd(),
                m_fileInP->getFileLocation());
        }
    }
    if (m_fourierP) {
        if (m_cycleP) {
            m_fourierP->process(
                m_columnP->getDataDoubleVSetP(),
                m_cycleP->getCycleColNo(),
                m_rowP->getRowBgn(), m_rowP->getRowEnd(),
                m_fileInP->getFileLocation()
            );
        }
        else {
            m_fourierP->process(
                m_columnP->getDataDoubleVSetP(), -1,
                m_rowP->getRowBgn(), m_rowP->getRowEnd(),
                m_fileInP->getFileLocation()
            );
        }
    }
    if (m_calcP) { m_calcP->process(); }
    m_columnP->process(m_timestepP->getDataTimestepIVP());

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
const Cycle* Args::getCycleP() const            { return m_cycleP; }
const Fourier* Args::getFourierP() const        { return m_fourierP; }
const FileOut* Args::getFileOutP() const        { return m_fileOutP; }
const PrintData* Args::getPrintDataP() const    { return m_printDataP; }
const FileData* Args::getFileDataP() const      { return m_fileDataP; }
const Help* Args::getHelpP() const              { return m_helpP; }
const Version* Args::getVersionP() const        { return m_versionP; }

void Args::resolveRowVsTimestep() {
    bool rBgnDef, rEndDef, tBgnDef, tEndDef;
    tie(rBgnDef, rEndDef) = m_rowP->getDefStatus();
    tie(tBgnDef, tEndDef) = m_timestepP->getDefStatus();

    if ((rBgnDef && tBgnDef) || (rEndDef && tEndDef)) {
        throw logic_error(errorRowTimestepConflict);
    }
    if (!rBgnDef && !tBgnDef) {
        m_timestepP->setTimestepBgnFromData();
    }
    else if (tBgnDef) {
        m_rowP->setRowBgn(
            m_timestepP->getDataTimestepIVP()->getRow(
                m_timestepP->getTimestepBgn())
        );
    }
    else {
        if (m_timestepP->isTimestepConsistent()) {
            m_timestepP->setTimestepBgnFromRow(m_rowP->getRowBgn());
        }
    }
    if (!rEndDef && !tEndDef) {
        m_rowP->setRowEndFromData();
        m_timestepP->setTimestepEndFromData();
    }
    else if (tEndDef) {
        m_rowP->setRowEnd(
            m_timestepP->getDataTimestepIVP()->getRow(
                m_timestepP->getTimestepEnd())
        );
    }
    else {
        if (m_timestepP->isTimestepConsistent()) {
            m_timestepP->setTimestepEndFromRow(m_rowP->getRowEnd());
        }
    }
    if (m_rowP->getRowBgn() > m_rowP->getRowEnd()) {
        throw invalid_argument(errorRowRangeInvalid);
    }
    if (m_timestepP->getTimestepBgn() > m_timestepP->getTimestepEnd()) {
            throw invalid_argument(errorTimestepRangeInvalid);
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
        unordered_map<string, CalcId>::const_iterator mapIt{
            mapStrToCalc.find(argV[c])
        };
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
        m_calcIdSet.insert(m_calcIdSet.end(), {
            CalcId::findMin,
            CalcId::findMax,
            // CalcId::findAbsMin,
            // CalcId::findAbsMax,
            CalcId::findMean,
            CalcId::findQuadraticMean,
            // CalcId::findCubicMean,
        });
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
void Column::importDataDouble(
            const vector<ColData::DoubleV*>& dataDoubleVSetP) {
    m_dataDoubleVSetP = dataDoubleVSetP;
}
void Column::process(const ColData::IntV* dataTimestepIVP) {
    // If no number or name is entered after option flag, select all columns
    if (m_intInputColSet.empty() && m_strInputColSet.empty()) {
        for (ColData::DoubleV* dVP : m_dataDoubleVSetP) {
            m_dataDoubleColSet.push_back(dVP->getColNo());
        }
    }
    else {
        // Check that it is NOT the timestep column
        if (dataTimestepIVP != nullptr) {
            for (int intInput : m_intInputColSet) {
                if (intInput == dataTimestepIVP->getColNo()) {
                    throw invalid_argument(errorColIsTimeStep);
                }
            }
            for (string& strInput : m_strInputColSet) {
                if (strInput == dataTimestepIVP->getColName()) {
                    throw invalid_argument(errorColIsTimeStep);
                }
            }
        }
        // Match the input to the respective column in the given file
        for (ColData::DoubleV* dVP : m_dataDoubleVSetP) {
            bool colExists{0};
            for (int intInput : m_intInputColSet) {
                if (intInput == dVP->getColNo()) {
                    colExists = true;
                }
            }
            if (!colExists) {
                for (string& strInput : m_strInputColSet) {
                    if (strInput == dVP->getColName()) {
                        colExists = true;
                    }
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
const vector<ColData::DoubleV*>& Column::getDataDoubleVSetP() const {
    return m_dataDoubleVSetP;
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
void Row::importDataRowTotal(size_t dataRowTotal) {
    m_dataRowTotal = dataRowTotal;
}
void Row::process(bool rowDefined1, bool rowDefined2, size_t row1, size_t row2,
        CycleInit cycleInitStatus){
    if (rowDefined1 && rowDefined2) {
        m_rowBgn = row1;
        m_rowEnd = row2;
        m_rowBgnDefined = m_rowEndDefined = true;
    }
    else if (rowDefined1 && !rowDefined2
            && cycleInitStatus != CycleInit::last) {
        m_rowBgn = row1;
        m_rowBgnDefined = true;
    }
    else if (rowDefined1 && !rowDefined2
            && cycleInitStatus == CycleInit::last) {
        m_rowEnd = row1;
        m_rowEndDefined = true;
    }
    Row::process();
}
void Row::process() {
    if (m_rowBgnDefined && m_rowEndDefined && m_rowBgn > m_rowEnd) {
        size_t temp = m_rowBgn;
        m_rowBgn = m_rowEnd;
        m_rowEnd = temp;
    }
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
}
void Row::setRowBgn(size_t val) { m_rowBgn = val; }
void Row::setRowEnd(size_t val) { m_rowEnd = val; }
void Row::setRowEndFromData() { m_rowEnd = m_dataRowTotal - 1; }
size_t Row::getDataRowTotal() const { return m_dataRowTotal; }
tuple<size_t, size_t> Row::getRange() const { return {m_rowBgn, m_rowEnd}; }
size_t Row::getRowBgn() const { return m_rowBgn; }
size_t Row::getRowEnd() const { return m_rowEnd; }
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
void Timestep::setTimestepBgnFromData() {
    m_timestepBgn = get<1>(m_dataTimestepRange);
}
void Timestep::setTimestepEndFromData() {
    m_timestepEnd = get<2>(m_dataTimestepRange);
}
void Timestep::setTimestepBgnFromRow(size_t val) {
    m_timestepBgn = m_dataTimestepIVP->getData()[val];
}
void Timestep::setTimestepEndFromRow(size_t val) {
    m_timestepEnd = m_dataTimestepIVP->getData()[val];
}
void Timestep::importDataTimestep(const ColData::IntV* dataTimestepIVP) {
    if (dataTimestepIVP) {
        m_dataTimestepIVP = dataTimestepIVP;
        m_dataTimestepRange = dataTimestepIVP->getTimestepRange();
    }
}
void Timestep::process(bool timestepDefined1, bool timestepDefined2,
        size_t timestep1, size_t timestep2, CycleInit cycleInitStatus,
        double timeIncrement) {
    if (timestepDefined1 && timestepDefined2) {
        m_timestepBgn = timestep1;
        m_timestepEnd = timestep2;
        m_timestepBgnDefined = m_timestepEndDefined = true;
    }
    else if (timestepDefined1 && !timestepDefined2
            && cycleInitStatus != CycleInit::last) {
        m_timestepBgn = timestep1;
        m_timestepBgnDefined = true;
    }
    else if (timestepDefined1 && !timestepDefined2
            && cycleInitStatus == CycleInit::last) {
        m_timestepEnd = timestep1;
        m_timestepEndDefined = true;
    }
    if (!get<0>(m_dataTimestepRange) && timeIncrement >= 0.0) {
        throw logic_error(errorDataTimestepInconsistent);
    }
    Timestep::process();
}
void Timestep::process() {
    if (get<0>(m_dataTimestepRange)) {// is input data timestep range consistent
        if (m_timestepBgnDefined && m_timestepEndDefined
                && m_timestepBgn > m_timestepEnd) {
            size_t temp = m_timestepBgn;
            m_timestepBgn = m_timestepEnd;
            m_timestepEnd = temp;
        }
        if (m_timestepBgnDefined) {
            if (m_timestepBgn < get<1>(m_dataTimestepRange)) {
                throw invalid_argument(errorTimeStepTooSmall);
            }
            if (m_timestepBgn >= get<2>(m_dataTimestepRange)) {
                throw invalid_argument(errorTimeStepTooLarge);
            }
        }
        if (m_timestepEndDefined) {
            if (m_timestepEnd <= get<1>(m_dataTimestepRange)) {
                throw invalid_argument(errorTimeStepTooSmall);
            }
            if (m_timestepEnd > get<2>(m_dataTimestepRange)) {
                throw invalid_argument(errorTimeStepTooLarge);
            }
        }
        m_timestepConsistent = true;
    }
    else {
        if (m_timestepBgnDefined || m_timestepEndDefined) {
            throw logic_error(errorDataTimestepInconsistent);
        }
    }
}
const ColData::IntV* Timestep::getDataTimestepIVP() const {
    return m_dataTimestepIVP;
}
tuple<bool, size_t, size_t> Timestep::getDataTimestepRange() const {
    return m_dataTimestepRange;
}
tuple<size_t, size_t> Timestep::getRange() const {
    return {m_timestepBgn, m_timestepEnd};
}
size_t Timestep::getTimestepBgn() const { return m_timestepBgn; }
size_t Timestep::getTimestepEnd() const { return m_timestepEnd; }
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
    while (c+1 < argC && argV[c+1][0] != '-' && m_argV.size() < m_maxArgs) {
        Args::setCount(++c);
        ++m_argC;
        m_argV.push_back(argV[c]);
    }
}
void Cycle::process(const vector<ColData::DoubleV*>& dataDoubleVSetP) {
    for (string cycleArg : m_argV) {
        if (all_of(cycleArg.begin(), cycleArg.end(), isdigit)) {
            if (m_cycleInputCount<0) {
                m_cycleInputCount = stoi(cycleArg);
                if (m_cycleInputCount < 0) {
                    throw invalid_argument(errorCycleInvalid);
                }
            }
            else {
                throw invalid_argument(errorCycleAlreadySpecified);
            }
        }
        else {
            size_t pos;
            unordered_map<string, CycleInit>::const_iterator mapIt{
                mapStrToCycleInit.find(cycleArg)
            };
            if (m_cycleInit == CycleInit::empty
                    && mapIt != mapStrToCycleInit.end()) {
                m_cycleInit = mapIt->second;
            }
            else if (m_fileName == ""
                    && ((pos=cycleArg.find("o=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                if (cycleArg.empty()) {
                    throw invalid_argument(errorOutputFileNameEmpty);
                }
                m_fileName = cycleArg + ".csv";
            }
            else if (m_fileName == "" && cycleArg == "o") {
                m_fileName = "auto";
            }
            else if (m_center == 0.0
                    && ((pos=cycleArg.find("m=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                string numStr{"0123456789Ee-+."};
                if (all_of(cycleArg.begin(), cycleArg.end(),
                        [&](char c) {
                            return (numStr.find(c) != string::npos);
                        })) {
                    m_center = stod(cycleArg);
                }
                else {
                    throw invalid_argument{errorCycleCenterInvalid};
                }
            }
            else if (m_minAmplitude == 0.0
                    && ((pos=cycleArg.find("a=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                string numStr{"0123456789Ee-+."};
                if (all_of(cycleArg.begin(), cycleArg.end(),
                        [&](char c) {
                            return (numStr.find(c) != string::npos);
                        })) {
                    m_minAmplitude = stod(cycleArg);
                }
                else {
                    throw invalid_argument{errorCycleMinAmplitudeInvalid};
                }
            }
            else if (m_minRowInterval == 0
                    && ((pos=cycleArg.find("n=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                if (stoi(cycleArg) < 0){
                    throw invalid_argument{errorCycleMinRowIntervalInvalid};
                }
                m_minRowInterval = stoi(cycleArg);
            }
            else if (m_timeIncrement < 0.0
                    && ((pos=cycleArg.find("dt=")) != string::npos)) {
                cycleArg.erase(0, pos+3);
                string numStr{"0123456789Ee-+."};
                if (all_of(cycleArg.begin(), cycleArg.end(),
                        [&](char c) {
                            return (numStr.find(c) != string::npos);
                        })) {
                    m_timeIncrement = stod(cycleArg);
                }
                else {
                    throw invalid_argument{errorTimeIncrementInvalid};
                }
            }
            else if (m_simTimeColNo<0
                    && (pos=cycleArg.find("st=")) != string::npos) {
                cycleArg.erase(0, pos+3);
                bool colExists{false};
                if (all_of(cycleArg.begin(), cycleArg.end(), isdigit)) {
                    m_simTimeColNo = stoi(cycleArg);
                    for (ColData::DoubleV* dVP : dataDoubleVSetP) {
                        if (m_simTimeColNo == dVP->getColNo()) {
                            colExists = true;
                            break;
                        }
                    }
                }
                else {
                    for (ColData::DoubleV* dVP : dataDoubleVSetP) {
                        if (cycleArg == dVP->getColName()) {
                            m_simTimeColNo = dVP->getColNo();
                            colExists = true;
                            break;
                        }
                    }
                }
                if (!colExists) {
                    throw invalid_argument(errorColAbsent);
                }
            }
            else if (!get<0>(m_rowDefined)
                     && ((pos=cycleArg.find("r=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                if (stoi(cycleArg) < 0){
                    throw invalid_argument(errorRowRangeInvalid);
                }
                get<0>(m_rowRange) = stoi(cycleArg);
                get<0>(m_rowDefined) = true;
            }
            else if (!get<1>(m_rowDefined)
                     && ((pos=cycleArg.find("r=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                if (stoi(cycleArg) <= 0){
                    throw invalid_argument(errorRowRangeInvalid);
                }
                get<1>(m_rowRange) = stoi(cycleArg);
                get<1>(m_rowDefined) = true;
            }
            else if (!get<0>(m_timestepDefined)
                     && ((pos=cycleArg.find("t=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                if (stoi(cycleArg) < 0) {
                    throw invalid_argument(errorTimestepRangeInvalid);
                }
                get<0>(m_timestepDefRange) = stoi(cycleArg);
                get<0>(m_timestepDefined) = true;
            }
            else if (!get<1>(m_timestepDefined)
                     && ((pos=cycleArg.find("t=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                if (stoi(cycleArg) <= 0) {
                    throw invalid_argument(errorTimestepRangeInvalid);
                }
                get<1>(m_timestepDefRange) = stoi(cycleArg);
                get<1>(m_timestepDefined) = true;
            }
            else if (m_cycleColNo<0) {
                if ((pos=cycleArg.find("c=")) != string::npos) {
                    cycleArg.erase(0, pos+2);
                    bool colExists{false};
                    if (all_of(cycleArg.begin(), cycleArg.end(), isdigit)) {
                        m_cycleColNo = stoi(cycleArg);
                        for (ColData::DoubleV* dVP : dataDoubleVSetP) {
                            if (m_cycleColNo == dVP->getColNo()) {
                                colExists = true;
                                break;
                            }
                        }
                    }
                    else {
                        for (ColData::DoubleV* dVP : dataDoubleVSetP) {
                            if (cycleArg == dVP->getColName()) {
                                m_cycleColNo = dVP->getColNo();
                                colExists = true;
                                break;
                            }
                        }
                    }
                    if (!colExists) {
                        throw invalid_argument(errorColAbsent);
                    }
                }
                else {
                    for (ColData::DoubleV* dVP : dataDoubleVSetP) {
                        if (cycleArg == dVP->getColName()) {
                            m_cycleColNo = dVP->getColNo();
                            break;
                        }
                    }
                    if (m_cycleColNo<0) {
                        throw invalid_argument(errorCycleColNameInvalid);
                    }
                }
            }
            else {
                throw invalid_argument(errorCycleArgumentInvalid);
            }
        }
    }
    // Error checking
    if (m_cycleColNo<0) {
        throw logic_error(errorCycleColNameMissing);
    }
    if (m_cycleInputCount>0 &&
            (get<1>(m_rowDefined) || get<1>(m_timestepDefined))) {
        throw logic_error(errorCycleTooManyArguments);
    }
    if (get<0>(m_rowDefined) && get<0>(m_timestepDefined)) {
        throw logic_error(errorCycleWithRowTimestepConflict);
    }
    if (m_cycleInputCount<0
            && (m_cycleInit == CycleInit::first
                || m_cycleInit == CycleInit::last)) {
        throw logic_error(errorCycleMissing);
    }
    // Automatic assignment of variables
    if (m_cycleInputCount<0 && m_cycleInit == CycleInit::empty) {
        m_cycleInit = CycleInit::full;
    }
    if (m_cycleInputCount>0 && m_cycleInit == CycleInit::empty) {
        m_cycleInit = CycleInit::last;
    }
    if (m_simTimeColNo >= 0 && m_timeIncrement >= 0.0) {
        throw logic_error(errorFrequencyTooManyArguments);
    }
}
void Cycle::setCalcCDataAndCycleInputCount(ColData::CycleData cData) {
    m_calcCycleData = cData;
    m_cycleInputCount = cData.cycleCount;
}
void Cycle::setFrequency(size_t rowBgn, size_t rowEnd) {
    vector<double> simTimeColData{
        DoubleV::getOnePFromCol(m_simTimeColNo)->getData()
    };
    m_frequency = m_cycleInputCount /
        (simTimeColData[rowEnd] - simTimeColData[rowBgn]);
}
void Cycle::setFrequency(tuple <size_t, size_t> timestepRange) {
    m_frequency = m_cycleInputCount /
        (m_timeIncrement*(get<1>(timestepRange) - get<0>(timestepRange)));
}
void Cycle::setAutoFileName(const size_t rowBgn, const size_t rowEnd,
        const string& fileInName) {
    string fileNameAffix{
        "_cycles_c" + to_string(m_cycleColNo) + "_r"
        + to_string(rowBgn) + "to"
        + to_string(rowEnd) + ".csv"
    };
    size_t pos;
    if ((pos = fileInName.find_last_of('.')) != string::npos
            && (fileInName.size() - pos) < 5) {
        m_fileName = fileInName.substr(0, pos) + fileNameAffix;
    }
    else {
        m_fileName = fileInName + fileNameAffix;
    }
}
int Cycle::getInputCount() const        { return m_cycleInputCount; }
CycleInit Cycle::getInitType() const    { return m_cycleInit; }
int Cycle::getCycleColNo() const        { return m_cycleColNo; }
int Cycle::getSimTimeColNo() const      { return m_simTimeColNo; }
double Cycle::getCenter() const         { return m_center; }
double Cycle::getTimeIncrement() const  { return m_timeIncrement; }
double Cycle::getFrequency() const      { return m_frequency; }
double Cycle::getMinAmplitude() const   { return m_minAmplitude; }
size_t Cycle::getMinRowInterval() const { return m_minRowInterval; }
const tuple<size_t, size_t> Cycle::getRowDefRange() const {
    return m_rowRange;
}
const tuple<size_t, size_t> Cycle::getTimestepDefRange() const {
    return m_timestepDefRange;
}
const tuple<bool, bool> Cycle::getRowDefStatus() const {
    return m_rowDefined;
}
const tuple<bool, bool> Cycle::getTimestepDefStatus() const {
    return m_timestepDefined;
}
const string& Cycle::getFileName() const { return m_fileName; }
const ColData::CycleData& Cycle::getCalcCycleData() const {
    return m_calcCycleData;
}

//----------------------------------------------------------------------------//
//***************************** CmdArgs::Fourier *****************************//
//----------------------------------------------------------------------------//

Fourier::Fourier(int c, int argC, const vector<string>& argV) {
    init(c, argC, argV);
}
void Fourier::init(int c, int argC, const vector<string>& argV) {
    while (c+1 < argC && argV[c+1][0] != '-' && m_argV.size() < 2) {
        Args::setCount(++c);
        ++m_argC;
        m_argV.push_back(argV[c]);
    }
}

void Fourier::process(const vector<ColData::DoubleV*>& dataDoubleVSetP,
        const int colNo, const size_t rowBgn, const size_t rowEnd,
        const string& fileInName) {
    if (colNo>=0) {
        m_colNo = colNo;
    }
    get<0>(m_rowRange) = rowBgn;
    get<1>(m_rowRange) = rowEnd;

    size_t pos;
    for (string fourierArg : m_argV) {
        if (m_fileName == ""
                && ((pos=fourierArg.find("o=")) != string::npos)) {
            fourierArg.erase(0, pos+2);
            if (fourierArg.empty()) {
                throw invalid_argument(errorOutputFileNameEmpty);
            }
            m_fileName = fourierArg + ".csv";
        }
        else if (m_fileName == "" && fourierArg == "o") {
            m_fileName = "auto";
        }
        else if (m_colNo<0 && ((pos=fourierArg.find("c=")) != string::npos)) {
            fourierArg.erase(0, pos+2);
            if (all_of(fourierArg.begin(), fourierArg.end(), isdigit)) {
                m_colNo = stoi(fourierArg);
                for (ColData::DoubleV* dVP : dataDoubleVSetP) {
                    if (m_colNo == dVP->getColNo()) {
                        break;
                    }
                }
            }
            else {
                for (ColData::DoubleV* dVP : dataDoubleVSetP) {
                    if (fourierArg == dVP->getColName()) {
                        m_colNo = dVP->getColNo();
                        break;
                    }
                }
            }
            if (m_colNo<0) {
                throw invalid_argument(errorColNoAbsent);
            }
        }
        else if (m_colNo<0) {
            for (ColData::DoubleV* dVP : dataDoubleVSetP) {
                if (fourierArg == dVP->getColName()) {
                    m_colNo = dVP->getColNo();
                    break;
                }
            }
            if (m_colNo<0) {
                throw invalid_argument(errorFourierColNameInvalid);
            }
        }
        else {
            throw invalid_argument(errorFourierArgumetInvalid);
        }
    }
    if (m_colNo<0) {
        throw logic_error(errorFourierColMissing);
    }
    if (m_fileName == "auto") {
        string fileNameAffix{
            "_fft_c" + to_string(m_colNo) + "_r" + to_string(rowBgn) + "to"
            + to_string(rowEnd) + ".csv"
        };
        if ((pos = fileInName.find_last_of('.')) != string::npos
                && (fileInName.size() - pos) < 5) {
            m_fileName = fileInName.substr(0, pos) + fileNameAffix;
        }
        else {
            m_fileName = fileInName + fileNameAffix;
        }
    }
}
int Fourier::getColNo() const { return m_colNo; }
const tuple<size_t, size_t> Fourier::getRowRange() const {
    return m_rowRange;
}
const string& Fourier::getFileName() const {
    return m_fileName;
}

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
        size_t pos;
        string fileOutName;
        string fileNameAffix{"_calc.csv"};

        if ((pos = fileInName.find_last_of('.')) != string::npos
                && (fileInName.size() - pos) < 5) {
            fileOutName = fileInName.substr(0, pos) + fileNameAffix;
        }
        else {
            fileOutName = fileInName + fileNameAffix;
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
    string fileOutName;
    string fileNameAffix{"_data.csv"};

    if ((pos = fileInName.find_last_of('.')) != string::npos
            && (fileInName.size() - pos) < 5) {
        fileOutName = fileInName.substr(0, pos) + fileNameAffix;
    }
    else {
        fileOutName = fileInName + fileNameAffix;
    }
}
const string& FileData::getFileName() const { return m_fileDataName; }
const string& FileData::getDelimiter() const    { return m_delimiter; }

//----------------------------------------------------------------------------//
//****************************** CmdArgs::Help *******************************//
//----------------------------------------------------------------------------//

const string& Help::getHelpFileName() const { return m_helpFileName; }

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//***************************** CmdArgs::Version *****************************//
//----------------------------------------------------------------------------//

Version::Version() :
  m_msg{m_program + ' ' + m_version + '\n'
        + m_copyright + ' ' + m_author + " (" + m_email + ")\n"
        + m_license + "\n"} {}

const string& Version::getMsg() const { return m_msg; }

//----------------------------------------------------------------------------//