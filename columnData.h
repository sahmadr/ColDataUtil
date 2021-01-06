/**
 * @file        columnData.h
 *
 * @project     colDataUtil
 * @version     0.1
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 * @date        2020-11-19
 *
 * @brief       Handling the column data; reading it from the files and storing
 *              it correctly.
 */

#ifndef COLUMNDATA_H
#define COLUMNDATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <vector>
#include <tuple>
#include <set>
#include <string>
#include <algorithm>
#include <string_view>

using   std::string, std::vector, std::set, std::tuple, std::tie, std::ios_base,
        std::numeric_limits, std::cout, std::endl,
        std::ifstream, std::ofstream, std::getline, std::streampos;
using stringV = std::string_view;

inline const string errorColNameDouble{"Double vector column name repeated. "
    "Please change one of the column names."};
inline const string errorColNameInt{"Integer vector column name repeated. "
    "Please change one of the column names."};
inline const string errorHeaderFormatIncorrect{"An incorrect format has been "
    "used for the header line in the file."};
inline const string errorDlmFormatIncorrect{"An incorrect delimiter has "
    "been specified OR an incorrect data format has been used in the file. "};
inline const string errorHeaderDlmTypeUndefined{"The delimiter type of header "
    "was undefined. This could be due to an incorrect formatting of the header "
    "line. Please report this issue. Thank you."};
inline const string errorDataFormatIncorrect{"An incorrect format has been "
    "used for the numeric data in the file."};
inline const string errorColNameAbsent{"Requested column name not found."};
inline const string errorColNoAbsent{"Requested column number not found."};
inline const string errorColAbsent{"Requested column not found."};
inline const string errorTimeStepTooSmall{"Invalid timestep specified. "
    "The specified timestep is less than the smallest found in the file."};
inline const string errorTimeStepTooLarge{"Invalid timestep specified. "
    "The specified timestep is larger than the largest found in the file."};
inline const string errorInputFile{"Input file could not be opened."};
inline const string errorOutputFile{"Output file could not be opened."};

enum class Delimitation { spaced, delimited, spacedAndDelimited };

//----------------------------------------------------------------------------//
//**************************** ColData Namespace *****************************//
//----------------------------------------------------------------------------//

namespace ColData {
    inline constexpr int maxRowsToCheckInt = 1000;
    inline constexpr int delimiterLenLimit = 3;
    class DoubleV;
    class IntV;
    bool isNumberLine(std::string_view lineStr, std::string dlm);
    tuple<streampos, streampos> findLinePositions(ifstream& iFile,
        const string& dlm);
    tuple<string, Delimitation> parseHeaderLine(ifstream& iFile,
        const string& dlm, const streampos headerLinePos);
    tuple<int, vector<string>> identifyColumnHeaders(string headerLine,
        const string& dlm, const Delimitation headerDlmType);
    Delimitation parseColumnData(ifstream& iFile, const string& dlm,
        const streampos dataLinePos);
    void classifyColumns(ifstream& iFile, const string& dlm,
        const Delimitation dataDlmType, const streampos dataLinePos,
        const int colTotal);
    void createVectors(const vector<string>& colNames);
    void populateVectors(ifstream& iFile, const string& dlm,
        const Delimitation dataDlmType, const streampos dataLinePos);
    void loadData(const string& fileName, const string& dlm);
    void printAvailableTimestepRange();
    void printColNames();
    void printData(string dlm=",");
    void fileData(string fileName, string dlm);
    const tuple<size_t, size_t> returnRows(const int column,
        const size_t timestepBgn, const size_t timestepEnd);
    void printer(const string& colName,
        const string& timeStepsStr, const string& fncName, double value);
    void filer(const string& fileName, const string& colName,
        const string& timeStepsStr, const string& fncName, double value);
    void outputValue(double(*calcFnc)(int, size_t, size_t),
        const int column, const size_t timestepBgn=1,
        const size_t timestepEnd=0);
    void outputValue(const string& fileName,
        double(*calcFnc)(int , size_t, size_t), const int column,
        const size_t timestepBgn=1, const size_t timestepEnd=0);
    // const string& rowTimestepsStrMaker
}

//----------------------------------------------------------------------------//
//*************************** ColData::IntV Class ****************************//
//----------------------------------------------------------------------------//
/*
 * Integer vector class for column data.
 */
class ColData::IntV {
  private:
    const int                   m_id;
    const int                   m_colNo;
    const string                m_colName;
    vector<int>                 m_data;
    inline static int           s_total{};
    inline static set<int>      s_intVColNoSet{};
    inline static vector<IntV*> s_intVSetP{};

    IntV() = delete;
    IntV(const IntV&) = delete;
    IntV& operator=(const IntV&) = delete;

  public:
    explicit IntV(int colNo, string colName);
    int getId() const;
    int getColNo() const;
    const string& getColName() const;
    int getCol(int colNo) const;
    string& getCol(string& colName) const;
    const vector<int>& getData() const;
    void addValue(int value);
    static void insertColNoSet(int);
    static int getTotal();
    static set<int>& getColNoSet();
    static vector<IntV*>& getSetP();
    static IntV* getOneP(const int id);
    static IntV* getOnePFromCol(const int inputColNo);
    static IntV* getOnePFromCol(const string inputColName);
    template<typename T>
    IntV* getOnePFromCol(const T inputCol) {
    int id{-1};
    T col{inputCol};
    for (IntV* iVP : s_intVSetP) {
        if (inputCol == iVP->getCol(col)) { id = iVP->m_id; }
    }
    if (id<0) { throw std::runtime_error(errorColAbsent); }
    return s_intVSetP[id];
    }
};

//----------------------------------------------------------------------------//
//************************ ColData::DoubleV Class *************************//
//----------------------------------------------------------------------------//
/*
 * Double vector class for column data.
 */
class ColData::DoubleV {
  private:
    const int                       m_id;
    const int                       m_colNo;
    const string                    m_colName;
    vector<double>                  m_data;
    inline static int               s_total{};
    inline static set<int>          s_doubleVColNoSet{};
    inline static vector<DoubleV*>  s_doubleVSetP{};

    DoubleV() = delete;
    DoubleV(const DoubleV&) = delete;
    DoubleV& operator=(const DoubleV&) = delete;

  public:
    explicit DoubleV(int colNo, string colName);
    int getId() const;
    int getColNo() const;
    const string& getColName() const;
    const vector<double>& getData() const;
    void addValue(double value);
    static void insertColNoSet(int colNo);
    double getSum(const size_t rowBgn, const size_t rowEnd) const;
    double getSumOfSquares(const size_t rowBgn, const size_t rowEnd) const;
    double getSumOfCubes(const size_t rowBgn, const size_t rowEnd) const;
    static int getTotal();
    static set<int>& getColNoSet();
    static vector<DoubleV*>& getSetP();
    static DoubleV* getOneP(const int id);
    static DoubleV* getOnePFromCol(const int inputColNo);
    static DoubleV* getOnePFromCol(const string inputColName);
};

#endif