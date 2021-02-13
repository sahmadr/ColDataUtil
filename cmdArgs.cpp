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
            m_cycleP->getInitType()
        );
    }
    else {
        m_rowP->process();
        m_timestepP->process();
    }
    resolveRowVsTimestep();
    if (m_cycleP) {
        DoubleV* cycleColDVP{DoubleV::getOnePFromCol(m_cycleP->getColNo())};
        int cycles{0};
        double rowInitial{0}, rowFinal{0};

        if (m_cycleP->getInitType() == CmdArgs::CycleInit::full) {
            tie(cycles, rowInitial, rowFinal) =
                cycleColDVP->findCycles(m_rowP->getRowBgn(),
                    m_rowP->getRowEnd(), m_cycleP->getCenter());
            m_cycleP->setCycleCount(cycles);
        }
        else if (m_cycleP->getInitType() == CmdArgs::CycleInit::first) {
            tie(cycles, rowInitial, rowFinal) =
                cycleColDVP->findCyclesFirst(m_rowP->getRowBgn(),
                    m_rowP->getRowEnd(), m_cycleP->getCenter(),
                    m_cycleP->getCount());
            if (cycles != m_cycleP->getCycleCount()) {
                m_cycleP->setCycleCount(cycles);
            }
        }
        else if (m_cycleP->getInitType() == CmdArgs::CycleInit::last) {
            tie(cycles, rowInitial, rowFinal) =
                cycleColDVP->findCyclesLast(m_rowP->getRowBgn(),
                    m_rowP->getRowEnd(), m_cycleP->getCenter(),
                    m_cycleP->getCount());
            if (cycles != m_cycleP->getCycleCount()) {
                m_cycleP->setCycleCount(cycles);
            }
        }

        // Update cycles, rows and timesteps
        m_rowP->setRowBgn(rowInitial);
        m_rowP->setRowEnd(rowFinal);
        if (m_timestepP->isTimestepConsistent()) {
            m_timestepP->setTimestepBgnFromRow(rowInitial);
            m_timestepP->setTimestepEndFromRow(rowFinal);
        }
    }
    if (m_fourierP) {
        if (m_cycleP) {
            m_fourierP->process(
                m_columnP->getDataDoubleVSetP(),
                m_cycleP->getColNo(),
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
            bool colExists{0};
            for (int intInput : m_intInputColSet) {
                colExists = (intInput == dataTimestepIVP->getColNo());
            }
            if (!colExists) {
                for (string& strInput : m_strInputColSet) {
                    colExists = (strInput == dataTimestepIVP->getColName());
                }
            }
            if (colExists) { throw invalid_argument(errorColIsTimeStep); }
        }
        // Match the input to the respective column in the given file
        for (ColData::DoubleV* dVP : m_dataDoubleVSetP) {
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
        size_t timestep1, size_t timestep2, CycleInit cycleInitStatus) {
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
    Timestep::process();
}
void Timestep::process() {
    if (get<0>(m_dataTimestepRange)) {// is input timestep range consistent
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
    while (c+1 < argC && argV[c+1][0] != '-' && m_cycleArgV.size() < 5) {
        Args::setCount(++c);
        ++m_cycleArgC;
        m_cycleArgV.push_back(argV[c]);
    }
}
void Cycle::process(const vector<ColData::DoubleV*>& dataDoubleVSetP) {
    for (string cycleArg : m_cycleArgV) {
        if (all_of(cycleArg.begin(), cycleArg.end(), isdigit)) {
            if (m_cycleCount<0) {
                m_cycleCount = stoi(cycleArg);
                if (m_cycleCount < 0) {
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
            else if (m_cycleCenter == 0.0
                    && ((pos=cycleArg.find("m=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                string numStr{"0123456789Ee-+."};
                if (all_of(cycleArg.begin(), cycleArg.end(), [&](char c) {
                        return (numStr.find(c) != string::npos); })) {
                    m_cycleCenter = stod(cycleArg);
                }
                else {
                    throw invalid_argument{errorCycleCenterInvalid};
                }
            }
            else if (!get<0>(m_cycleRowDefined)
                     && ((pos=cycleArg.find("r=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                if (stoi(cycleArg) < 0){
                    throw invalid_argument(errorRowRangeInvalid);
                }
                get<0>(m_cycleRow) = stoi(cycleArg);
                get<0>(m_cycleRowDefined) = true;
            }
            else if (!get<1>(m_cycleRowDefined)
                     && ((pos=cycleArg.find("r=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                if (stoi(cycleArg) <= 0){
                    throw invalid_argument(errorRowRangeInvalid);
                }
                get<1>(m_cycleRow) = stoi(cycleArg);
                get<1>(m_cycleRowDefined) = true;
            }
            else if (!get<0>(m_cycleTimestepDefined)
                     && ((pos=cycleArg.find("t=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                if (stoi(cycleArg) < 0) {
                    throw invalid_argument(errorTimestepRangeInvalid);
                }
                get<0>(m_cycleTimestep) = stoi(cycleArg);
                get<0>(m_cycleTimestepDefined) = true;
            }
            else if (!get<1>(m_cycleTimestepDefined)
                     && ((pos=cycleArg.find("t=")) != string::npos)) {
                cycleArg.erase(0, pos+2);
                if (stoi(cycleArg) <= 0) {
                    throw invalid_argument(errorTimestepRangeInvalid);
                }
                get<1>(m_cycleTimestep) = stoi(cycleArg);
                get<1>(m_cycleTimestepDefined) = true;
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
                        throw invalid_argument(errorColNoAbsent);
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
    // if (m_cycleCount<0 &&
    //         !get<1>(m_cycleRowDefined) && !get<1>(m_cycleTimestepDefined)) {
    //     throw logic_error(errorCycleTooFewArguments);
    // }
    if (m_cycleCount>0 &&
            (get<1>(m_cycleRowDefined) || get<1>(m_cycleTimestepDefined))) {
        throw logic_error(errorCycleTooManyArguments);
    }
    if (get<0>(m_cycleRowDefined) && get<0>(m_cycleTimestepDefined)) {
        throw logic_error(errorCycleWithRowTimestepConflict);
    }
    if (m_cycleCount<0
            && (m_cycleInit == CycleInit::first
                || m_cycleInit == CycleInit::last)) {
        throw logic_error(errorCycleMissing);
    }
    // Automatic assignment of variables
    if (m_cycleCount<0 && m_cycleInit == CycleInit::empty) {
        m_cycleInit = CycleInit::full;
    }
    if (m_cycleCount>0 && m_cycleInit == CycleInit::empty) {
        m_cycleInit = CycleInit::last;
    }

    /* cout<< "\n Cycle arguments:\n"
        << (m_cycleInit==CycleInit::empty ? " CycleInit::empty" :
            (m_cycleInit==CycleInit::first ? " CycleInit::first" :
             (m_cycleInit==CycleInit::last ? " CycleInit::last" :
              " CycleInit::full")))
        << "\n CycleCount     = " << m_cycleCount
        << "\n CycleColNo     = " << m_cycleColNo
        << "\n CycleMean      = " << m_cycleCenter
        << "\n CycleRow1      = " << get<0>(m_cycleRow)
        << "\n CycleRow2      = " << get<1>(m_cycleRow)
        << "\n CycleTimestep1 = " << get<0>(m_cycleTimestep)
        << "\n CycleTimestep2 = " << get<1>(m_cycleTimestep) << '\n'; */
}
void Cycle::setCycleCount(int cycles)   { m_cycleCount = cycles; }
int Cycle::getCycleCount() const        { return m_cycleCount; }
CycleInit Cycle::getInitType() const    { return m_cycleInit; }
int Cycle::getCount() const             { return m_cycleCount; }
int Cycle::getColNo() const             { return m_cycleColNo; }
double Cycle::getCenter() const         { return m_cycleCenter; }
const tuple<size_t, size_t> Cycle::getRowDefRange() const {
    return m_cycleRow;
}
const tuple<size_t, size_t> Cycle::getTimestepDefRange() const {
    return m_cycleTimestep;
}
const tuple<bool, bool> Cycle::getRowDefStatus() const {
    return m_cycleRowDefined;
}
const tuple<bool, bool> Cycle::getTimestepDefStatus() const {
    return m_cycleTimestepDefined;
}

//----------------------------------------------------------------------------//
//***************************** CmdArgs::Fourier *****************************//
//----------------------------------------------------------------------------//

Fourier::Fourier(int c, int argC, const vector<string>& argV) {
    init(c, argC, argV);
}
void Fourier::init(int c, int argC, const vector<string>& argV) {
    while (c+1 < argC && argV[c+1][0] != '-' && m_fourierArgV.size() < 2) {
        Args::setCount(++c);
        ++m_fourierArgC;
        m_fourierArgV.push_back(argV[c]);
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
    for (string fourierArg : m_fourierArgV) {
        if (m_fileFourierName == ""
                && ((pos=fourierArg.find("o=")) != string::npos)) {
            fourierArg.erase(0, pos+2);
            m_fileFourierName = fourierArg + ".csv";
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
    if (m_fileFourierName == "") {
        string fileNameAffix{
            "_fft_c" + to_string(m_colNo) + "_r" + to_string(rowBgn) + "to"
            + to_string(rowEnd) + ".csv"
        };
        size_t posBgn{0};
        string temp;
        if (fileInName[0] == '.') {
            if (fileInName[1] == '.') {
                posBgn = 2;
            }
            else {
                posBgn = 1;
            }
        }
        temp = fileInName.substr(posBgn, fileInName.length());
        if ((pos = temp.find_last_of('.')) != string::npos) {
            m_fileFourierName = fileInName.substr(posBgn, pos+posBgn)
                                + fileNameAffix;
        }
        else {
            m_fileFourierName = fileInName + fileNameAffix;
        }
    }
}

int Fourier::getColNo() const { return m_colNo; }
const tuple<size_t, size_t> Fourier::getRowRange() const {
    return m_rowRange;
}
const string& Fourier::getFileFourierName() const {
    return m_fileFourierName;
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
        size_t posBgn{0};
        string fileOutName;
        string temp;
        string fileNameAffix{"_calc.csv"};

        if (fileInName[0] == '.') {
            if (fileInName[1] == '.') {
                posBgn = 2;
            }
            else {
                posBgn = 1;
            }
        }
        temp = fileInName.substr(posBgn, fileInName.length());
        if ((pos = temp.find_last_of('.')) != string::npos) {
            fileOutName = fileInName.substr(posBgn, pos+posBgn)
                            + fileNameAffix;
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
    size_t posBgn{0};
    string fileOutName;
    string temp;
    string fileNameAffix{"_data.csv"};

    if (fileInName[0] == '.') {
        if (fileInName[1] == '.') {
            posBgn = 2;
        }
        else {
            posBgn = 1;
        }
    }
    temp = fileInName.substr(posBgn, fileInName.length());
    if ((pos = temp.find_last_of('.')) != string::npos) {
        fileOutName = fileInName.substr(posBgn, pos+posBgn)
                        + fileNameAffix;
    }
    else {
        fileOutName = fileInName + fileNameAffix;
    }
}
const string& FileData::getFileDataName() const { return m_fileDataName; }
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
        + m_copyright + ' ' + m_author + " (" + m_email + ")\n\n"} {}

const string& Version::getMsg() const { return m_msg; }

//----------------------------------------------------------------------------//