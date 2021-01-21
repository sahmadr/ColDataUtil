/**
 * @file        colData.cpp
 *
 * @project     colDataUtil
 * @version     0.4
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       Handle the column data; read it from the files and store it
 *              correctly.
 */

#include "namespaces.h"
#include "colData.h"
#include "calcFnc.h"
#include "mappings.h"
#include "errorMsgs.h"

using namespace ColData;
using   std::istringstream, std::all_of, std::any_of, std::remove_if;

//----------------------------------------------------------------------------//
//*************************** ColData::IntV Class ****************************//
//----------------------------------------------------------------------------//
IntV::IntV(int colNo, string colName) :
  m_id{s_total++}, m_colNo{colNo}, m_colName{colName}, m_data{} {
    for (IntV* iVP : s_intVSetP) {
        if (m_colName == iVP->m_colName) {
            throw runtime_error(errorColNameInt);
        }
    }
    s_intVSetP.push_back(this);
}
void IntV::addValue(int value)          { m_data.push_back(value); }
void IntV::insertColNoSet(int colNo)    { s_intVColNoSet.insert(colNo); }

int IntV::getId() const                 { return m_id; }
int IntV::getColNo() const              { return m_colNo; }
const string& IntV::getColName() const  { return m_colName; }
int IntV::getCol(int colNo) const       { return colNo = m_colNo; }
string& IntV::getCol(string& colName) const { return colName = m_colName; }
const vector<int>& IntV::getData() const{ return m_data; }
tuple<bool, size_t, size_t> IntV::getTimestepColRange() const {
    // bool represents timestep column consistency (true for consistent)
    // first size_t is beginning timestep and last one is final timestep
    if (m_data[0] < 0) {
        return {false, 0, 0};
    }
    for (size_t i=1; i<m_data.size(); ++i) {
        if (m_data[i] <= m_data[i-1]) {
            return {false, 0, 0};
        }
    }
    return {true, m_data[0], m_data[m_data.size() - 1]};
}
size_t IntV::getRow(int timestepVal) const {
    return std::distance(
        m_data.cbegin(), std::find(m_data.cbegin(), m_data.cend(), timestepVal)
    );
}

int IntV::getTotal()                    { return s_total; }
set<int>& IntV::getColNoSet()           { return s_intVColNoSet; }
vector<IntV*>& IntV::getSetP()          { return s_intVSetP; }
IntV* IntV::getOneP(const int id)       { return s_intVSetP[id]; }
/*
IntV* IntV::getOnePFromCol(const int inputColNo) {
    int id{-1};
    for (IntV* iVP : s_intVSetP) {
        if (inputColNo == iVP->m_colNo) { id = iVP->m_id; }
    }
    if (id<0) { throw runtime_error(errorColNoAbsent); }
    return s_intVSetP[id];
}
IntV* IntV::getOnePFromCol(const string inputColName) {
    int id{-1};
    for (IntV* iVP : s_intVSetP) {
        if (inputColName == iVP->m_colName) { id = iVP->m_id; }
    }
    if (id<0) { throw runtime_error(errorColNameAbsent); }
    return s_intVSetP[id];
}
 */
//----------------------------------------------------------------------------//
//************************** ColData::DoubleV Class **************************//
//----------------------------------------------------------------------------//
DoubleV::DoubleV(int colNo, string colName) :
  m_id{s_total++}, m_colNo{colNo}, m_colName{colName}, m_data{} {
    for (DoubleV* dVP : s_doubleVSetP) {
        if (m_colName == dVP->m_colName) {
            throw runtime_error(errorColNameDouble);
        }
    }
    s_doubleVSetP.push_back(this);
}
void DoubleV::addValue(double value)          { m_data.push_back(value); }
void DoubleV::insertColNoSet(int colNo)       {s_doubleVColNoSet.insert(colNo);}

int DoubleV::getId() const                    { return m_id; }
int DoubleV::getColNo() const                 { return m_colNo; }
const string& DoubleV::getColName() const     { return m_colName; }
const vector<double>& DoubleV::getData() const{ return m_data; }

int DoubleV::getTotal()                       { return s_total; }
set<int>& DoubleV::getColNoSet()              { return s_doubleVColNoSet; }
vector<DoubleV*>& DoubleV::getSetP()          { return s_doubleVSetP; }
DoubleV* DoubleV::getOneP(const int id)       { return s_doubleVSetP[id]; }
DoubleV* DoubleV::getOnePFromCol(const int inputColNo) {
    int id{-1};
    for (DoubleV* dVP : s_doubleVSetP) {
        if (inputColNo == dVP->m_colNo) { id = dVP->m_id; }
    }
    if (id<0) { throw runtime_error(errorColNoAbsent); }
    return s_doubleVSetP[id];
}
DoubleV* DoubleV::getOnePFromCol(const string inputColName) {
    int id{-1};
    for (DoubleV* dVP : s_doubleVSetP) {
        if (inputColName == dVP->m_colName) { id = dVP->m_id; }
    }
    if (id<0) { throw runtime_error(errorColNameAbsent); }
    return s_doubleVSetP[id];
}

// Summations ----------------------------------------------------------------//
double DoubleV::getSum(const size_t rowBgn, const size_t rowEnd) const {
    double sum{0};
    for (size_t r=rowBgn; r<=rowEnd; ++r) {
        sum += m_data[r];
    }
    return sum;
}
double DoubleV::getSumOfSquares(const size_t rowBgn, const size_t rowEnd) const{
    double sum{0};
    for (size_t r=rowBgn; r<=rowEnd; ++r) {
        double x{m_data[r]};
        sum += x*x;
    }
    return sum;
}
double DoubleV::getSumOfCubes(const size_t rowBgn, const size_t rowEnd) const {
    double sum{0};
    for (size_t r=rowBgn; r<=rowEnd; ++r) {
        double x{m_data[r]};
        sum += x*x*x;
    }
    return sum;
}

//----------------------------------------------------------------------------//
//************************* Data loading and storage *************************//
//----------------------------------------------------------------------------//
/*
 * Load file and call all the relevant functions to process it and return all
 * the required variables to CmdArgs::FileIn::process().
 */
tuple<Delimitation, int, size_t, tuple<bool, size_t, size_t>> ColData::loadData(
        const string& fileName, const string& dlm) {
    // Open file -------------------------------------------------------------//
    ifstream iFile{fileName};
    if (!iFile) { throw runtime_error(errorInputFile); }
    else { cout << "File found. Program initiated." << flush; }

    // Process the header ----------------------------------------------------//
    streampos headerLinePos, dataLinePos;
    tie(headerLinePos, dataLinePos) = findLinePositions(iFile, dlm);

    string headerLine;
    Delimitation headerDlmType;
    tie(headerLine, headerDlmType) = parseHeaderLine(iFile, dlm, headerLinePos);

    int dataColTotal;
    vector<string> colNames;
    tie(dataColTotal, colNames) =
        identifyColumnHeaders(headerLine, dlm, headerDlmType);

    set<int> timeStepColCandidates{
        findTimestepColCandidates(dataColTotal, colNames)
    };

    // Process the data ------------------------------------------------------//
    Delimitation dataDlmType = parseColumnData(iFile, dlm, dataLinePos);
    cout << "\rIn progress: Parsing column data..." << flush;
    classifyColumns(iFile, dlm, dataDlmType, dataLinePos, dataColTotal,
                    timeStepColCandidates);

    // Store the data --------------------------------------------------------//
    createVectors(colNames);
    cout << "\rIn progress: Processing column data..." << flush;
    const size_t dataRowTotal{
        populateVectors(iFile, dlm, dataColTotal, dataDlmType, dataLinePos)
    };
    cout << '\r' << string(38, ' ') << "\n" << flush;
    iFile.close();

    tuple<bool, size_t, size_t> dataTimestepRange{0, 0, 0};
    if (!IntV::getSetP().empty()) {
        dataTimestepRange = IntV::getOneP(0)->getTimestepColRange();
    }

    return {dataDlmType, dataColTotal, dataRowTotal, dataTimestepRange};
}

/*
 * Check if the line is a number line, i.e. it has numerical data.
 */
bool ColData::isNumberLine(stringV lineStr, string dlm) {
    string comparisonStr{"0123456789Ee-+."};
    comparisonStr.append(dlm);
    return all_of(lineStr.begin(), lineStr.end(), [&](char c) {
        return (isspace(c) || (comparisonStr.find(c) != string::npos));
    });
}

/*
 * Return the header line position.
 */
tuple<streampos, streampos> ColData::findLinePositions(ifstream& iFile,
        const string& dlm) {
    string line;
    iFile.clear(), iFile.seekg(0);
    streampos currentPos{0}, lastPos{0}, headerLinePos{0}, dataLinePos{0};

    while (getline(iFile, line)) {
        lastPos = currentPos;
        currentPos = iFile.tellg();
        if (!line.empty()) {
            if (!isNumberLine(line, dlm)) {
                headerLinePos = lastPos;
            }
            else {
                dataLinePos = lastPos;
                break;
            }
        }
    }
    return {headerLinePos, dataLinePos};
}

/*
 * Parse header line for a number of alternative formats and return the
 * correctly formatted header line.
 */
tuple<string, Delimitation> ColData::parseHeaderLine(
        ifstream& iFile, const string& dlm, const streampos headerLinePos) {
    string iLine, oLine;
    Delimitation headerDlmType;
    iFile.clear(), iFile.seekg(headerLinePos);
    getline(iFile, iLine);

    if (iLine.find('=') != string::npos) {
        istringstream iLineStream{iLine};
        getline(iLineStream, oLine, '=');   // Read until the '=' sign
        getline(iLineStream, oLine);        // Read rest of line and overwrite
        if (oLine.find(dlm) != string::npos) {
            headerDlmType = Delimitation::delimited;
            oLine.erase(remove_if(oLine.begin(), oLine.end(), isspace),
                oLine.end());               // Remove spaces from the string
        }
        else if (any_of(oLine.begin(), oLine.end(), isspace)) {
            headerDlmType = Delimitation::spaced;
        }
        else {
            throw runtime_error(errorHeaderFormatIncorrect);
        }
    }
    else if (iLine.find(dlm) != string::npos) {
        headerDlmType = Delimitation::delimited;
        iLine.erase(remove_if(iLine.begin(), iLine.end(), isspace),
            iLine.end());                   // Remove spaces from the string
        oLine = iLine;
    }
    else if (any_of(iLine.begin(), iLine.end(), isspace)) {
        headerDlmType = Delimitation::spaced;
        oLine = iLine;
    }
    else {
        throw runtime_error(errorHeaderFormatIncorrect);
    }
    return {oLine, headerDlmType};
}

/*
 * Find the total number of columns and the column names.
 */
tuple<int, vector<string>> ColData::identifyColumnHeaders(string headerLine,
        const string& dlm, const Delimitation headerDlmType) {
    size_t pos{0}, dlmLen{dlm.length()};
    int dataColTotal{0};
    vector<string> colNames{};

    if (headerDlmType == Delimitation::delimited) {
        while (pos != string::npos) {
            string header{headerLine.substr(0, pos=headerLine.find(dlm))};
            if (!all_of(header.cbegin(), header.cend(), isspace)) {
                colNames.push_back(header);
                ++dataColTotal;
            }
            headerLine.erase(0, pos+dlmLen);
        }
    }
    else if (headerDlmType == Delimitation::spaced) {
        istringstream headerLineStream{headerLine};
        string header;
        while (headerLineStream >> header) {
            colNames.push_back(header);
            ++dataColTotal;
        }
    }
    else {
        throw runtime_error(errorHeaderDlmTypeUndefined);
    }
    return {dataColTotal, colNames};
}

/*
 * Find the possible timestep columns by searching for "step" in the column
 * names and return the set of candidates.
 */
set<int> ColData::findTimestepColCandidates(int dataColTotal,
        vector<string> colNames) {
    set<int> timestepColCandidates{};
    for (int c=0; c<dataColTotal; ++c) {
        string& colName{colNames[c]};
        std::transform(colName.begin(), colName.end(), colName.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if (colName.find("step") != string::npos) {
            timestepColCandidates.insert(c);
        }
    }
    return timestepColCandidates;
}

/*
 * Parse column data to find the delimitation type of column data.
 */
Delimitation ColData::parseColumnData(ifstream& iFile,
        const string& dlm, const streampos dataLinePos) {
    string  spacedNumStr{"0123456789Ee-+."}, delimitedNumStr{spacedNumStr+dlm},
            line;
    iFile.clear(), iFile.seekg(dataLinePos);
    getline(iFile, line);
    if (all_of(line.begin(), line.end(), [&](char c) {
            return (isspace(c) || (spacedNumStr.find(c) != string::npos)); })) {
        return Delimitation::spaced;
    }
    else if (all_of(line.begin(), line.end(), [&](char c) {
            return (delimitedNumStr.find(c) != string::npos); })) {
        return Delimitation::delimited;
    }
    else if (all_of(line.begin(), line.end(), [&](char c) {
            return (isspace(c) || (delimitedNumStr.find(c)!=string::npos));})) {
        return Delimitation::spacedAndDelimited;
    }
    else {
        throw runtime_error(errorDataFormatIncorrect);
    }
}

/*
 * Classify the columns into timestep column and data columns. Timestep column
 * is identified by selecting the leftmost integer column from the set of
 * timestepColCandidates.
 */
void ColData::classifyColumns(ifstream& iFile, const string& dlm,
        const Delimitation dataDlmType, const streampos dataLinePos,
        const int dataColTotal, set<int>& timestepColCandidates) {
    size_t pos{0}, dlmLen{dlm.length()};
    string line;

    iFile.clear(), iFile.seekg(dataLinePos);

    if (dataDlmType == Delimitation::spaced) {
        while(getline(iFile, line)) {
            if (all_of(line.cbegin(), line.cend(), isspace)) { continue; }
            istringstream lineStream{line};
            for (int c=0; c<dataColTotal; ++c) {
                string numStr;
                lineStream >> numStr;
                if (timestepColCandidates.count(c)) {
                    if (all_of(numStr.begin(), numStr.end(), isdigit)) {
                        break;
                    }
                    else {
                        timestepColCandidates.erase(c);
                        iFile.clear(), iFile.seekg(dataLinePos); // Reset
                        break;
                    }
                }
            }
        }
    }
    else {
        while(getline(iFile, line)) {
            if (line.find(dlm) == string::npos) { continue; }
            if (dataDlmType == Delimitation::spacedAndDelimited) {
                line.erase(remove_if(line.begin(), line.end(), isspace),
                    line.end());
            }
            for (int c=0; c<dataColTotal; ++c) {
                pos = line.find(dlm);
                if (timestepColCandidates.count(c)) {
                    string numStr{line.substr(0, pos)};
                    if (all_of(numStr.begin(), numStr.end(), isdigit)) {
                        break;
                    }
                    else {
                        timestepColCandidates.erase(c);
                        iFile.clear(), iFile.seekg(dataLinePos); // Reset
                        break;
                    }
                }
                line.erase(0, pos+dlmLen);
            }
        }
    }
    for (int c=0; c<dataColTotal; ++c) {
        if (timestepColCandidates.count(c)) {
            IntV::insertColNoSet(c);
            break;
        }
    }
    for (int c=0; c<dataColTotal; ++c) {
        if (!IntV::getColNoSet().count(c)) {
            DoubleV::insertColNoSet(c);
        }
    }
}

/*
 * Create vectors for storing column data using the first line of the file.
 */
void ColData::createVectors(const vector<string>& colNames) {
    for (int colNo : IntV::getColNoSet()) {
        new IntV(colNo, colNames[colNo]);
    }
    for (int colNo : DoubleV::getColNoSet()) {
        new DoubleV(colNo, colNames[colNo]);
    }
}

/*
 * Populate the column vectors by reading the respective column data from file.
 */
size_t ColData::populateVectors(ifstream& iFile, const string& dlm,
        const int dataColTotal, const Delimitation dataDlmType,
        const streampos dataLinePos) {
    size_t pos{0}, dataRowTotal{0}, dlmLen{dlm.length()};
    string line;
    vector<IntV*> iVSetP{IntV::getSetP()};
    vector<DoubleV*> dVSetP{DoubleV::getSetP()};

    iFile.clear(), iFile.seekg(dataLinePos);

    if (dataDlmType == Delimitation::spaced) {
        while(getline(iFile, line)) {
            if (all_of(line.cbegin(), line.cend(), isspace)) { continue; }
            istringstream lineStream{line};
            int iIndex{0}, dIndex{0};
            for (int c=0; c<dataColTotal; ++c) {
                string numStr;
                lineStream >> numStr;
                if (DoubleV::getColNoSet().count(c)) {
                    dVSetP[dIndex++]->addValue(stod(numStr));
                }
                else {
                    iVSetP[iIndex++]->addValue(stoi(numStr));
                }
            }
            ++dataRowTotal;
        }
    }
    else {
        while(getline(iFile, line)) {
            if (line.find(dlm) == string::npos) { continue; }
            if (dataDlmType == Delimitation::spacedAndDelimited) {
                line.erase(remove_if(line.begin(), line.end(), isspace),
                    line.end());
            }
            int iIndex{0}, dIndex{0};
            for (int c=0; c<dataColTotal; ++c) {
                string numStr{line.substr(0, pos=line.find(dlm))};
                if (DoubleV::getColNoSet().count(c)) {
                    dVSetP[dIndex++]->addValue(stod(numStr));
                }
                else {
                    iVSetP[iIndex++]->addValue(stoi(numStr));
                }
                line.erase(0, pos+dlmLen);
            }
        ++dataRowTotal;
        }
    }
    // Match the number of lines of data:
    bool matchLineCount{true};
    for (IntV* iVP : IntV::getSetP()) {
        if (dataRowTotal != iVP->getData().size()) { matchLineCount = false; }
    }
    for (DoubleV* dVP : DoubleV::getSetP()) {
        if (dataRowTotal != dVP->getData().size()) { matchLineCount = false; }
    }
    if (dataRowTotal == 0 || !matchLineCount) {
        throw runtime_error(errorDlmFormatIncorrect);
    }
    return dataRowTotal;
}