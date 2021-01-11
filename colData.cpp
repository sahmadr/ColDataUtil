/**
 * @file        colData.cpp
 *
 * @project     colDataUtil
 * @version     0.2
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
int IntV::getId() const                 { return m_id; }
int IntV::getColNo() const              { return m_colNo; }
const string& IntV::getColName() const  { return m_colName; }
int IntV::getCol(int colNo) const       { return colNo = m_colNo; }
string& IntV::getCol(string& colName) const { return colName = m_colName; }
const vector<int>& IntV::getData() const{ return m_data; }
void IntV::addValue(int value)          { m_data.push_back(value); }
void IntV::insertColNoSet(int colNo)    { s_intVColNoSet.insert(colNo); }
int IntV::getTotal()                    { return s_total; }
set<int>& IntV::getColNoSet()           { return s_intVColNoSet; }
vector<IntV*>& IntV::getSetP()          { return s_intVSetP; }
IntV* IntV::getOneP(const int id)       { return s_intVSetP[id]; }
// IntV* IntV::getOnePFromCol(const int inputColNo) {
//     int id{-1};
//     for (IntV* iVP : s_intVSetP) {
//         if (inputColNo == iVP->m_colNo) { id = iVP->m_id; }
//     }
//     if (id<0) { throw runtime_error(errorColNoAbsent); }
//     return s_intVSetP[id];
// }
// IntV* IntV::getOnePFromCol(const string inputColName) {
//     int id{-1};
//     for (IntV* iVP : s_intVSetP) {
//         if (inputColName == iVP->m_colName) { id = iVP->m_id; }
//     }
//     if (id<0) { throw runtime_error(errorColNameAbsent); }
//     return s_intVSetP[id];
// }

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
int DoubleV::getId() const                    { return m_id; }
int DoubleV::getColNo() const                 { return m_colNo; }
const string& DoubleV::getColName() const     { return m_colName; }
const vector<double>& DoubleV::getData() const{ return m_data; }
void DoubleV::addValue(double value)          { m_data.push_back(value); }
void DoubleV::insertColNoSet(int colNo)       {s_doubleVColNoSet.insert(colNo);}
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
 * Check if the line is a number line, i.e. it has numerical data.
 */
bool ColData::isNumberLine(stringV lineStr, string dlm) {
    string comparisonStr{"0123456789Ee-+."};
    comparisonStr.append(dlm);
    return std::all_of(lineStr.begin(), lineStr.end(), [&](char c) {
        return (
            std::isspace(c) || (comparisonStr.find(c) != string::npos)
        );
    });
}

/*
 * Return the header line position
 */
tuple<streampos, streampos> ColData::findLinePositions(ifstream& iFile,
        const string& dlm) {
    string line;
    iFile.clear(), iFile.seekg(0);
    streampos oldLinePos{0}, newLinePos{0};

    do {
        oldLinePos = newLinePos;
        newLinePos = iFile.tellg();
        getline(iFile, line);
    }
    while (!isNumberLine(line, dlm));

    return {oldLinePos, newLinePos};
}

/*
 * Parse header line for a number of alternatives and return the  correctly
 * formatted header line.
 */
tuple<string, Delimitation> ColData::parseHeaderLine(
        ifstream& iFile, const string& dlm, const streampos headerLinePos) {
    string iLine, oLine;
    Delimitation headerDlmType;
    iFile.clear(), iFile.seekg(headerLinePos);
    getline(iFile, iLine);

    if (iLine.find('=') != string::npos) {
        std::istringstream iLineStream{iLine};
        getline(iLineStream, oLine, '=');   // Read until the '=' sign
        getline(iLineStream, oLine);        // Read rest of line and overwrite
        if (oLine.find(dlm) != string::npos) {
            headerDlmType = Delimitation::delimited;
            oLine.erase(std::remove_if(oLine.begin(), oLine.end(), isspace),
                oLine.end());               // Remove spaces from the string
        }
        else if (std::any_of(oLine.begin(), oLine.end(), isspace)) {
            headerDlmType = Delimitation::spaced;
        }
        else {
            throw runtime_error(errorHeaderFormatIncorrect);
        }
    }
    else if (iLine.find(dlm) != string::npos) {
        headerDlmType = Delimitation::delimited;
        iLine.erase(std::remove_if(iLine.begin(), iLine.end(), isspace),
            iLine.end());                   // Remove spaces from the string
        oLine = iLine;
    }
    else if (std::any_of(iLine.begin(), iLine.end(), isspace)) {
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
    int colTotal{0};
    vector<string> colNames{};

    if (headerDlmType == Delimitation::delimited) {
        while (pos != string::npos) {
            string header{headerLine.substr(0, pos=headerLine.find(dlm))};
            if (!std::all_of(header.cbegin(), header.cend(), isspace)) {
                colNames.push_back(header);
                ++colTotal;
            }
            headerLine.erase(0, pos+dlmLen);
        }
    }
    else if (headerDlmType == Delimitation::spaced) {
        std::istringstream headerLineStream{headerLine};
        string header;
        while (headerLineStream >> header) {
            colNames.push_back(header);
            ++colTotal;
        }
    }
    else {
        throw runtime_error(errorHeaderDlmTypeUndefined);
    }
    return {colTotal, colNames};
}

/*
 * Parse column data to find the delimitation type of column data and the
 * starting position of the column data.
 */
Delimitation ColData::parseColumnData(ifstream& iFile,
        const string& dlm, const streampos dataLinePos) {
    string  spacedNumStr{"0123456789Ee-+."}, delimitedNumStr{spacedNumStr+dlm},
            line;
    iFile.clear(), iFile.seekg(dataLinePos);
    getline(iFile, line);
    if (std::all_of(line.begin(), line.end(), [&](char c) {
            return (std::isspace(c) || (spacedNumStr.find(c) != string::npos));
            })) {
        return Delimitation::spaced;
    }
    else if (std::all_of(line.begin(), line.end(), [&](char c) {
            return (delimitedNumStr.find(c) != string::npos); })) {
        return Delimitation::delimited;
    }
    else if (std::all_of(line.begin(), line.end(), [&](char c) {
            return (std::isspace(c) || (delimitedNumStr.find(c)!=string::npos));
            })) {
        return Delimitation::spacedAndDelimited;
    }
    else {
        throw runtime_error(errorDataFormatIncorrect);
    }
}

/*
 * Classify the columns into integers and doubles by checking each row.
 */
void ColData::classifyColumns(ifstream& iFile, const string& dlm,
        const Delimitation dataDlmType, const streampos dataLinePos,
        const int colTotal) {
    size_t pos{0}, dlmLen{dlm.length()};
    string line;
    set<int> intColNos{}, doubleColNos{};

    iFile.clear(), iFile.seekg(dataLinePos);

    if (dataDlmType == Delimitation::spaced) {
        while(getline(iFile, line)) {
            std::istringstream lineStream{line};
            for (int c=0; c<colTotal; ++c) {
                string numStr;
                lineStream >> numStr;
                if (!DoubleV::getColNoSet().count(c)) {
                    if (!std::all_of(numStr.begin(), numStr.end(), isdigit)) {
                        DoubleV::insertColNoSet(c);
                    }
                }
            }
        }
    }
    else {
        while(getline(iFile, line) && line.find(dlm) != string::npos) {
            if (dataDlmType == Delimitation::spacedAndDelimited) {
                line.erase(std::remove_if(line.begin(), line.end(), isspace),
                    line.end());        // Remove spaces from the string
            }
            for (int c=0; c<colTotal; ++c) {
                pos = line.find(dlm);
                if (!DoubleV::getColNoSet().count(c)) {
                    string numStr{line.substr(0, pos)};
                    if (!std::all_of(numStr.begin(), numStr.end(), isdigit)) {
                        DoubleV::insertColNoSet(c);
                    }
                }
                line.erase(0, pos+dlmLen);
            }
        }
    }
    for (int c=0; c<colTotal; ++c) {
        if (!DoubleV::getColNoSet().count(c)) {
            IntV::insertColNoSet(c);
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
void ColData::populateVectors(ifstream& iFile, const string& dlm,
        const Delimitation dataDlmType, const streampos dataLinePos) {
    size_t dlmLen{dlm.length()}, pos{0}, lineNo{0};
    int colTotal{IntV::getTotal() + DoubleV::getTotal()};
    string line;
    vector<IntV*> iVSetP{IntV::getSetP()};
    vector<DoubleV*> dVSetP{DoubleV::getSetP()};

    iFile.clear(), iFile.seekg(dataLinePos);

    if (dataDlmType == Delimitation::spaced) {
        while(getline(iFile, line)) {
            if (std::all_of(line.cbegin(), line.cend(), isspace)) { continue; }
            std::istringstream lineStream{line};
            int iIndex{0}, dIndex{0};
            for (int c=0; c<colTotal; ++c) {
                string numStr;
                lineStream >> numStr;
                if (DoubleV::getColNoSet().count(c)) {     // if double column
                    dVSetP.at(dIndex++)->addValue(std::stod(numStr));
                }
                else {
                    iVSetP.at(iIndex++)->addValue(std::stoi(numStr));
                }
            }
            ++lineNo;
        }
    }
    else {
        while(getline(iFile, line)) {
            if (line.find(dlm) == string::npos) { continue; }
            if (dataDlmType == Delimitation::spacedAndDelimited) {
                line.erase(std::remove_if(line.begin(), line.end(), isspace),
                    line.end());        // Remove spaces from the string
            }
            int iIndex{0}, dIndex{0};
            for (int c=0; c<colTotal; ++c) {
                string numStr{line.substr(0, pos=line.find(dlm))};
                if (DoubleV::getColNoSet().count(c)) {     // if double column
                    dVSetP.at(dIndex++)->addValue(std::stod(numStr));
                }
                else {
                    iVSetP.at(iIndex++)->addValue(std::stoi(numStr));
                }
                line.erase(0, pos+dlmLen);
            }
        ++lineNo;
        }
    }
    // Match the number of lines of data:
    bool matchLineCount{true};
    for (IntV* iVP : IntV::getSetP()) {
        if (lineNo != iVP->getData().size()) { matchLineCount = false; }
    }
    for (DoubleV* dVP : DoubleV::getSetP()) {
        if (lineNo != dVP->getData().size()) { matchLineCount = false; }
    }
    if (lineNo == 0 || !matchLineCount) {
        throw runtime_error(errorDlmFormatIncorrect);
    }
}

/*
 * Load file and call createVectors and populateVectors functions.
 */
void ColData::loadData(const string& fileName, const string& dlm) {
    streampos headerLinePos, dataLinePos;
    string headerLine;
    Delimitation headerDlmType, dataDlmType;
    int colTotal;
    vector<string> colNames;
    ifstream iFile{fileName};

    if (!iFile) { throw runtime_error(errorInputFile); }
    iFile.setf(ios_base::scientific);
    iFile.precision(numeric_limits<double>::max_digits10);

    tie(headerLinePos, dataLinePos) = findLinePositions(iFile, dlm);
    tie(headerLine, headerDlmType) = parseHeaderLine(iFile, dlm, headerLinePos);
    tie(colTotal, colNames) = identifyColumnHeaders(
        headerLine, dlm, headerDlmType);
    dataDlmType = parseColumnData(iFile, dlm, dataLinePos);
    classifyColumns(iFile, dlm, dataDlmType, dataLinePos, colTotal);
    createVectors(colNames);
    populateVectors(iFile, dlm, dataDlmType, dataLinePos);

    iFile.close();

    cout << "\nTotal columns =\t\t" << colTotal;
    cout << "\n" << "Integer columns:\t";
    for (int c : IntV::getColNoSet()) { cout << c << ", "; }
    cout << "\n" << "Double columns:\t\t";
    for (int c : DoubleV::getColNoSet()) { cout << c << ", "; }
    cout << std::endl;
    cout << "\n" << "Integer columns:\n";
    for (IntV* iVP : IntV::getSetP()) {
        cout << iVP->getColNo() << ".\t" << iVP->getColName() << '\n';
    }
    cout << "\n" << "Double columns:\n";
    for (DoubleV* dVP : DoubleV::getSetP()) {
        cout << dVP->getColNo() << ".\t" << dVP->getColName() << '\n';
    }
    cout << "\n" << "Integer column size:\n" << IntV::getTotal() << endl;
    cout << "\n" << "Data delimitation:\n"
        << ((dataDlmType == Delimitation::delimited) ? "delimited" :
            ((dataDlmType == Delimitation::spacedAndDelimited) ?
                "spacedAndDelimited" : "spaced")) << '\n' << endl;
}

//----------------------------------------------------------------------------//
//******************** Printing information on loaded data *******************//
//----------------------------------------------------------------------------//
/*
 * Print the total timesteps available in the file, assuming that
 * the first column of the loaded data is an integer representation of the
 * number of timesteps.
 */
void ColData::printAvailableTimestepRange() {
    vector<int> timesteps{IntV::getOneP(0)->getData()};
    cout << timesteps.size() << '\n';
    cout    << "\nThe timestep range available in the source file is from "
            << timesteps.at(0) << " to ";
            // << timesteps.at(timesteps.size()-1) << " timesteps." << endl;
}

/*
 * Print the names of all the vector columns; first integers and then doubles.
 */
void ColData::printColNames() {
    cout << "Column names:\n===================================\n";
    for (IntV* iVP : IntV::getSetP()) {
        cout    << std::setw(3) << std::left
                << iVP->getColNo() << iVP->getColName() << '\n';
    }
    for (DoubleV* dVP : DoubleV::getSetP()) {
        cout    << std::setw(3) << std::left
                << dVP->getColNo() << dVP->getColName() << '\n';
    }
    cout << "-----------------------------------\n";
    // printAvailableTimestepRange();
}

//----------------------------------------------------------------------------//
//********************* Printing and filing loaded data **********************//
//----------------------------------------------------------------------------//
/*
 * Print the data of all the vector columns; first integers and then doubles.
 */
void ColData::printData(string dlm) {
    vector<IntV*>    iVSetP{IntV::getSetP()};
    vector<DoubleV*> dVSetP{DoubleV::getSetP()};
    size_t totalRows{dVSetP.at(DoubleV::getTotal()-1)->getData().size()};
    int totalColumns{IntV::getTotal() + DoubleV::getTotal()};

    cout    << "Total columns\t= "  << totalColumns << '\n'
            << "Total rows\t= "     << totalRows    << "\n\n";
    cout.setf(ios_base::scientific);
    cout.precision(numeric_limits<double>::max_digits10);

    for (size_t row=0; row<totalRows; ++row) {
        for (IntV* iVP : iVSetP)    { cout << iVP->getData()[row] << dlm;}
        for (DoubleV* dVP : dVSetP) { cout << dVP->getData()[row] << dlm;}
        cout << endl;
    }
}

/*
 * File the data of all the vector columns; first integers and then doubles.
 * Note: Presently it is NOT available through command line arguments (CmdArgs).
 */
void ColData::fileData(string fileName, string dlm) {
    ofstream oFile{fileName};
    oFile.setf(ios_base::scientific);
    oFile.precision(numeric_limits<double>::max_digits10);

    vector<IntV*>    iVSetP{IntV::getSetP()};
    vector<DoubleV*> dVSetP{DoubleV::getSetP()};
    size_t totalRows {dVSetP.at(DoubleV::getTotal()-1)->getData().size()};

    // File the header line
    for (IntV* iVP : iVSetP)     { oFile << iVP->getColName() << dlm; }
    for (DoubleV* dVP : dVSetP) { oFile << dVP->getColName() << dlm; }
    oFile << endl;

    // File the data
    for (size_t row=0; row<totalRows; ++row) {
        for (IntV* iVP : iVSetP) {
            oFile << iVP->getData()[row] << dlm;
        }
        for (DoubleV* dVP : dVSetP) {
            oFile << dVP->getData()[row] << dlm;
        }
        oFile << endl;
    }
    oFile.close();
}

//----------------------------------------------------------------------------//
//***************** Printing and filing calculation results ******************//
//----------------------------------------------------------------------------//
/*
 * Return the appropriate row begin and end values.
 */
const tuple<size_t, size_t> ColData::returnRows(const int column,
        const size_t timestepBgn, const size_t timestepEnd) {
    size_t rowBgn{timestepBgn - 1}, rowEnd;
    if (timestepEnd == 0) {
        rowEnd = DoubleV::getOnePFromCol(column)->getData().size() - 1;
    }
    else { rowEnd = timestepEnd - 1; }
    return {rowBgn, rowEnd};
}
/*
 * Perform the selected operation on the selected column and print the result
 * to the terminal.
 */
void ColData::outputValue(calcType calc, const int column,
        const size_t timestepBgn, const size_t timestepEnd) {
    vector<int> timesteps{IntV::getOneP(0)->getData()};
    DoubleV* dVP{DoubleV::getOnePFromCol(column)};

    size_t rowBgn, rowEnd;
    tie(rowBgn, rowEnd) = returnRows(column, timestepBgn, timestepEnd);

    string timeStepsStr = "timesteps " + std::to_string(timesteps.at(rowBgn))
                            + " to " + std::to_string(timesteps.at(rowEnd));
    printer(dVP->getColName(), timeStepsStr,
        CalcFnc::mapCalcToStr<int>.at(calc),
        calc(column, rowBgn, rowEnd));
}

/*
 * Perform the selected operation on the selected column and file the result.
 */
void ColData::outputValue(const string& fileName, calcType calc,
        const int column, const size_t timestepBgn, const size_t timestepEnd) {
    vector<int> timesteps{IntV::getOneP(0)->getData()};
    DoubleV* dVP{DoubleV::getOnePFromCol(column)};

    size_t rowBgn, rowEnd;
    tie(rowBgn, rowEnd) = returnRows(column, timestepBgn, timestepEnd);

    string timeStepsStr = "timesteps " + std::to_string(timesteps.at(rowBgn))
                            + " to " + std::to_string(timesteps.at(rowEnd));

    filer(fileName, dVP->getColName(), timeStepsStr,
        CalcFnc::mapCalcToStr<int>.at(calc),
        calc(column, rowBgn, rowEnd));
}

/*
 * Printer for ColData::outputValue.
 */
void ColData::printer(const string& colName, const string& timeStepsStr,
        const string& fncName, double value) {
    cout.setf(ios_base::scientific);
    cout.precision(numeric_limits<double>::max_digits10);
    cout<< "\n" << std::left
        << std::setw(25) << colName
        << std::setw(25) << timeStepsStr
        << std::setw(22) << fncName
        << value << endl;
}

/*
 * Filer for ColData::outputValue.
 */
void ColData::filer(const string& fileName, const string& colName,
        const string& timeStepsStr, const string& fncName, double value) {
    ofstream oFile;
    oFile.open(fileName, ios_base::app);
    oFile.setf(ios_base::scientific);
    oFile.precision(numeric_limits<double>::max_digits10);
    if(!oFile) { throw runtime_error(errorOutputFile); }
    oFile   << colName << "," << timeStepsStr << "," << fncName << ","
            << value << "," << endl;
    oFile.close();
}



/*
 * Lower the characters of a string.
 * Source: https://en.cppreference.com/w/cpp/string/byte/tolower
 */
/* string strToLower(string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                    [](unsigned char c){ return std::tolower(c); });
    return s;
} */