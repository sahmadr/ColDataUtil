/**
 * @file        namespaces.h
 *
 * @project     colDataUtil
 * @version     0.2
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       All the includes, namespaces and aliases.
 *
 */

#ifndef NAMESPACES_H
#define NAMESPACES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <string_view>
#include <vector>
#include <tuple>
#include <set>
#include <limits>
#include <algorithm>
#include <cstddef>
#include <cmath>

using   std::string, std::vector, std::set, std::tuple, std::unordered_map,
        std::numeric_limits, std::tie, std::ios_base, std::to_string,
        std::cout, std::endl, std::flush, std::setw, std::left, std::right,
        std::stoi, std::stod,
        std::ifstream, std::ofstream, std::getline, std::streampos,
        std::invalid_argument, std::logic_error, std::runtime_error;

using stringV = std::string_view;
using calcType = double(*)(int, size_t, size_t);

enum class Delimitation { undefined, spaced, delimited, spacedAndDelimited };

//----------------------------------------------------------------------------//
//**************************** ColData Namespace *****************************//
//----------------------------------------------------------------------------//

namespace ColData {
    // inline constexpr int maxRowsToCheckInt = 1000;
    // inline constexpr int delimiterLenLimit = 3;
    class DoubleV;
    class IntV;
    bool isNumberLine(stringV lineStr, string dlm);
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
    tuple<int, Delimitation> loadData(const string& fileName,
        const string& dlm);
    void printAvailableTimestepRange();
    const tuple<size_t, size_t> returnRows(const int column,
        const size_t timestepBgn, const size_t timestepEnd);
    // const string& rowTimestepsStrMaker
}

//----------------------------------------------------------------------------//
//**************************** CmdArgs Namespace *****************************//
//----------------------------------------------------------------------------//

namespace CmdArgs {
    enum class Option { delimiter, fileIn, function, column, row, timestep,
        fileOut, printData, fileData, help, version };
    enum class CalcId { findMin, findMax, findAbsMin, findAbsMax, findMean,
        findQuadraticMean, findCubicMean };
    class Args;
    class Delimiter;
    class FileIn;
    class Calc;
    class Column;
    class Row;
    class Timestep;
    class FileOut;
    class PrintData;
    class FileData;
    class Help;
    class Version;
    extern const unordered_map<string, Option> mapStrToOption;
    extern const unordered_map<string, CalcId> mapStrToCalc;
}

//----------------------------------------------------------------------------//
//**************************** CalcFnc Namespace *****************************//
//----------------------------------------------------------------------------//

namespace CalcFnc {
    template<typename T> const unordered_map<calcType, string> mapCalcToStr;
    template<typename T> double findMax(const T column,
        const size_t rowBgn, const size_t rowEnd);
    template<typename T> double findMin(const T column,
        const size_t rowBgn, const size_t rowEnd);
    template<typename T> double findAbsMax(const T column,
        const size_t rowBgn, const size_t rowEnd);
    template<typename T> double findAbsMin(const T column,
        const size_t rowBgn, const size_t rowEnd);
    template<typename T> double findMean(const T column,
        const size_t rowBgn, const size_t rowEnd);
    template<typename T> double findQuadraticMean(const T column,
        const size_t rowBgn, const size_t rowEnd);
    template<typename T> double findCubicMean(const T column,
        const size_t rowBgn, const size_t rowEnd);
}

//----------------------------------------------------------------------------//
//***************************** Output Namespace *****************************//
//----------------------------------------------------------------------------//

namespace Output {
    void printInputDataInfo(int dataColTotal, Delimitation dataDlmType);
    void output(CmdArgs::Args* argsP);
    void printer(
        const tuple<size_t, size_t> rowRange,
        const vector<int>& doubleColSet,
        const vector<CmdArgs::CalcId>& calcIdSet);
    void filer(const string& fileOutStr,
        const tuple<size_t, size_t> rowRange,
        const vector<int>& doubleColSet,
        const vector<CmdArgs::CalcId>& calcIdSet);
    void printData(const string dlm);
    void fileData(const string& fileName, const string& dlm);
    template<typename T> const unordered_map<CmdArgs::CalcId, calcType>
        mapCalcIdToCalc;
}

#endif