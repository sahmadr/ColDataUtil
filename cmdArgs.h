/**
 * @file        cmdArgs.h
 *
 * @project     colDataUtil
 * @version     0.2
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       Read command line input options.
 *
 */

#ifndef CMDARGS_H
#define CMDARGS_H

#include "namespaces.h"
#include "colData.h"
#include "calcFnc.h"
#include "errorMsgs.h"

//----------------------------------------------------------------------------//
//****************************** CmdArgs::Args *******************************//
//----------------------------------------------------------------------------//

class CmdArgs::Args {
  private:
    inline static int       s_c{0};         // current count of processed args.
    const int               m_argc;         // total number of arguments
    const vector<string>    m_argv;         // complete set of arguments
    const string            m_programName;  // program name
    Delimiter*              m_delimiterP;   // delimiter
    FileIn*                 m_fileInP;      // file with data to be processed
    Calc*                   m_calcP;        // value functions to be calculated
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
//**************************** CmdArgs::Delimiter ****************************//
//----------------------------------------------------------------------------//

class CmdArgs::Delimiter {
  private:
    string m_delimiter{","};

    Delimiter(const Delimiter&) = delete;
    Delimiter& operator=(const Delimiter&) = delete;

  public:
    explicit Delimiter() = default;
    explicit Delimiter(int c, const vector<string>& argV);
    const string& getDelimiter() const;
};

//----------------------------------------------------------------------------//
//***************************** CmdArgs::FileIn ******************************//
//----------------------------------------------------------------------------//

class CmdArgs::FileIn {
  private:
    string m_fileLocation{""};

    FileIn() = delete;
    FileIn(const FileIn&) = delete;
    FileIn& operator=(const FileIn&) = delete;

  public:
    explicit FileIn(int c, int argC, const vector<string>& argV);
    explicit FileIn(const vector<string>& argV);
    void process(const string& delimiter);
    const string& getFileLocation() const;
};

//----------------------------------------------------------------------------//
//****************************** CmdArgs::Calc *******************************//
//----------------------------------------------------------------------------//

class CmdArgs::Calc {
  private:
    vector<CmdArgs::CalcId>   m_calcIdSet{};

    Calc(const Calc&) = delete;
    Calc& operator=(const Calc&) = delete;

  public:
    explicit Calc() = default;
    explicit Calc(int c, int argC, const vector<string>& argV);
    void setCalcIdSet(int c, int argC, const vector<string>& argV);
    void process();
    const vector<CmdArgs::CalcId>& getCalcIdSet() const;
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