/**
 * @file        cmdArgs.h
 *
 * @project     colDataUtil
 * @version     0.1
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 * @date        2020-11-22
 *
 * @brief       Reading command line input options.
 */
#ifndef CMDARGS_H
#define CMDARGS_H

#include "errorStatements.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <string_view>

using std::string, std::vector, std::unordered_map;
using stringV = std::string_view;

namespace CmdArgs {
    enum class Option { fileIn, function, column, row, timestep, fileOut,
        printData, help, version };
    enum class CalcFncId { findMin, findMax, findAbsMin, findAbsMax, findMean,
        findQuadraticMean, findCubicMean };
    class Args;
    class FileIn;
    class CalcFnc;
    class Column;
    class Row;
    class Timestep;
    class FileOut;
    class PrintData;
    class Help;
    class Version;
    extern const unordered_map<string, Option> mapStrToOption;
    extern const unordered_map<string, CalcFncId> mapStrToCalcFnc;
    template<typename T> const std::unordered_map<
        CalcFncId, double(*)(T, size_t, size_t)> mapCalcFncIdToCalcFnc;
}

//----------------------------------------------------------------------------//
//****************************** CmdArgs::Args *******************************//
//----------------------------------------------------------------------------//

class CmdArgs::Args {
  private:
    inline static int       s_c{0};         // current count of processed args.
    const int               m_argc;         // total number of arguments
    const vector<string>    m_argv;         // complete set of arguments
    const string            m_programName;  // program name
    FileIn*                 m_fileInP;      // file with data to be processed
    CalcFnc*                m_calcFncP;     // value functions to be calculated
    Column*                 m_columnP;      // columns to be processed
    Row*                    m_rowP;         // range of rows
    Timestep*               m_timestepP;    // range of timesteps
    FileOut*                m_fileOutP;     // file where to save calculations
    PrintData*              m_printDataP;   // print data to the screen

    Args() = delete;
    Args(const Args&) = delete;
    Args& operator=(const Args&) = delete;

  public:
    explicit Args(int argc, char* argv[]);
    void process();
    void output();
    static int setCount(int newCount);
    static int getCount();
    int getArgC() const;
    const vector<string>& getArgV() const;
    const string getProgramName() const;
};

//----------------------------------------------------------------------------//
//***************************** CmdArgs::FileIn ******************************//
//----------------------------------------------------------------------------//

class CmdArgs::FileIn {
  private:
    string m_fileLocation{""};
    string m_delimiter{","};

    FileIn() = delete;
    FileIn(const FileIn&) = delete;
    FileIn& operator=(const FileIn&) = delete;

  public:
    explicit FileIn(int c, int argC, const vector<string>& argV);
    explicit FileIn(int argC, const vector<string>& argV);
    void process();
    const string& getFileLocation() const;
    const string& getDelimiter() const;
};

//----------------------------------------------------------------------------//
//***************************** CmdArgs::CalcFnc ******************************//
//----------------------------------------------------------------------------//

class CmdArgs::CalcFnc {
  private:
    vector<CmdArgs::CalcFncId>   m_calcFncIdSet{};

    CalcFnc(const CalcFnc&) = delete;
    CalcFnc& operator=(const CalcFnc&) = delete;

  public:
    explicit CalcFnc() = default;
    explicit CalcFnc(int c, int argC, const vector<string>& argV);
    void setCalcFncIdSet(int c, int argC, const vector<string>& argV);
    void process();
    const vector<CmdArgs::CalcFncId>& getCalcFncIdSet() const;
};

//----------------------------------------------------------------------------//
//***************************** CmdArgs::Column ******************************//
//----------------------------------------------------------------------------//

class CmdArgs::Column {
  private:
    vector<int>     m_intInputColSet{};
    vector<string>  m_strInputColSet{};
    vector<int>     m_colSet{};

    Column(const Column&) = delete;
    Column& operator=(const Column&) = delete;

  public:
    explicit Column() = default;
    explicit Column(int c, int argC, const vector<string>& argV);
    void setColInputSets(int c, int argC, const vector<string>& argV);
    void process();
    const vector<int>& getColSet() const;
    const vector<int>& getIntInputColSet() const;
    const vector<string>& getStrInputColSet() const;
};

//----------------------------------------------------------------------------//
//***************************** CmdArgs::FileOut *****************************//
//----------------------------------------------------------------------------//

class CmdArgs::FileOut {
  private:
    vector<string>  m_fileLocationSet{};

    FileOut() = delete;
    FileOut(const FileOut&) = delete;
    FileOut& operator=(const FileOut&) = delete;

  public:
    explicit FileOut(int c, int argC, const vector<string>& argV);
    void setFileLocationSet(int c, int argC, const vector<string>& argV);
    void process(const string& fileInName);
    const vector<string> getFileLocationSet() const;
};

//----------------------------------------------------------------------------//
//**************************** CmdArgs::PrintData ****************************//
//----------------------------------------------------------------------------//

class CmdArgs::PrintData {
  private:
    string  m_delimiter{","};   // currently not

    PrintData() = delete;
    PrintData(const PrintData&) = delete;
    PrintData& operator=(const PrintData&) = delete;

  public:
    explicit PrintData(int c, int argC, const vector<string>& argV);
    void process();
    const string& getDelimiter() const;
};

//----------------------------------------------------------------------------//
//******************************* CmdArgs::Row *******************************//
//----------------------------------------------------------------------------//

class CmdArgs::Row {
  private:
    size_t  m_rowBgn{0};
    size_t  m_rowEnd{0};

    Row() = delete;
    Row(const Row&) = delete;
    Row& operator=(const Row&) = delete;

  public:
    explicit Row(int c, int argC, const vector<string>& argV);
    void process();
    const std::tuple<size_t, size_t> getRowRange() const;
};

//----------------------------------------------------------------------------//
//**************************** CmdArgs::Timestep *****************************//
//----------------------------------------------------------------------------//

#endif