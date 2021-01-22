/**
 * @file        cmdArgs.h
 *
 * @project     ColDataUtil
 * @version     0.4
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
    FileData*               m_fileDataP;    // file data to a file
    Version*                m_versionP;     // version information

    Args() = delete;
    Args(const Args&) = delete;
    Args& operator=(const Args&) = delete;

  public:
    explicit Args(int argc, char* argv[]);
    void process();

    static int setCount(int newCount);
    static int getCount();

    int getArgC() const;
    const vector<string>& getArgV() const;
    const string getProgramName() const;

    const Delimiter* getDelimiterP() const;
    const FileIn* getFileInP() const;
    const Calc* getCalcP() const;
    const Column* getColumnP() const;
    const Row* getRowP() const;
    const Timestep* getTimestepP() const;
    const FileOut* getFileOutP() const;
    const PrintData* getPrintDataP() const;
    const FileData* getFileDataP() const;
    const Version* getVersionP() const;

    void resolveRowVsTimestep();
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
    string          m_fileLocation{""};
    Delimitation    m_dataDlmType{Delimitation::undefined};

    FileIn() = delete;
    FileIn(const FileIn&) = delete;
    FileIn& operator=(const FileIn&) = delete;

  public:
    explicit FileIn(int c, int argC, const vector<string>& argV);
    explicit FileIn(const vector<string>& argV);

    void importDataDlmType(Delimitation dataDlmType);

    const string& getFileLocation() const;
    Delimitation getDataDlmType() const;
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

    void init(int c, int argC, const vector<string>& argV);

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
    int             m_dataColTotal{0};
    vector<int>     m_dataDoubleColSet{};

    Column(const Column&) = delete;
    Column& operator=(const Column&) = delete;

  public:
    explicit Column() = default;
    explicit Column(int c, int argC, const vector<string>& argV);

    void init(int c, int argC, const vector<string>& argV);
    void importDataColTotal(int dataColTotal);

    void process();

    const vector<int>& getIntInputColSet() const;
    const vector<string>& getStrInputColSet() const;
    int getDataColTotal() const;
    const vector<int>& getDataDoubleColSet() const;
};

//----------------------------------------------------------------------------//
//******************************* CmdArgs::Row *******************************//
//----------------------------------------------------------------------------//

class CmdArgs::Row {
  private:
    bool    m_rowBgnDefined{false};
    bool    m_rowEndDefined{false};
    size_t  m_rowBgn{0};
    size_t  m_rowEnd{0};
    size_t  m_dataRowTotal{0};

    Row(const Row&) = delete;
    Row& operator=(const Row&) = delete;

  public:
    explicit Row() = default;
    explicit Row(int c, int argC, const vector<string>& argV);
    void setRowEnd(int c, int argC, const vector<string>& argV);

    void setRowBgn(size_t val);
    void setRowEnd(size_t val);
    void importDataRowTotal(size_t dataRowTotal);

    void process();

    size_t getDataRowTotal() const;
    tuple<size_t, size_t> getRange() const;
    tuple<bool, bool> getDefStatus() const;
};

//----------------------------------------------------------------------------//
//**************************** CmdArgs::Timestep *****************************//
//----------------------------------------------------------------------------//

class CmdArgs::Timestep {
  private:
    bool    m_timestepBgnDefined{false};
    bool    m_timestepEndDefined{false};
    size_t  m_timestepBgn{0};
    size_t  m_timestepEnd{0};
    bool    m_timestepConsistent{false};
    tuple<bool, size_t, size_t> m_dataTimestepRange{0,0,0};

    Timestep(const Timestep&) = delete;
    Timestep& operator=(const Timestep&) = delete;

  public:
    explicit Timestep() = default;
    explicit Timestep(int c, int argC, const vector<string>& argV);
    void setTimestepEnd(int c, int argC, const vector<string>& argV);

    void setTimestepBgn(size_t val);
    void setTimestepEnd(size_t val);
    void importDataTimestepRange(tuple<bool, size_t, size_t> dataTimestepRange);

    void process();

    tuple<bool, size_t, size_t> getDataTimestepRange() const;
    tuple<size_t, size_t> getRange() const;
    tuple<bool, bool> getDefStatus() const;
    bool isTimestepConsistent() const;
};

//----------------------------------------------------------------------------//
//***************************** CmdArgs::FileOut *****************************//
//----------------------------------------------------------------------------//

class CmdArgs::FileOut {
  private:
    vector<string>  m_fileOutLocSet{};

    FileOut() = delete;
    FileOut(const FileOut&) = delete;
    FileOut& operator=(const FileOut&) = delete;

  public:
    explicit FileOut(int c, int argC, const vector<string>& argV);
    void init(int c, int argC, const vector<string>& argV);
    void process(const string& fileInName);
    const vector<string> getFileLocSet() const;
};

//----------------------------------------------------------------------------//
//**************************** CmdArgs::PrintData ****************************//
//----------------------------------------------------------------------------//

class CmdArgs::PrintData {
  private:
    string m_delimiter{","};

    PrintData() = delete;
    PrintData(const PrintData&) = delete;
    PrintData& operator=(const PrintData&) = delete;

  public:
    explicit PrintData(int c, int argC, const vector<string>& argV);
    const string& getDelimiter() const;
};

//----------------------------------------------------------------------------//
//**************************** CmdArgs::FileData *****************************//
//----------------------------------------------------------------------------//

class CmdArgs::FileData {
  private:
    string m_fileDataName{""};
    string m_delimiter{","};

    FileData() = delete;
    FileData(const FileData&) = delete;
    FileData& operator=(const FileData&) = delete;

  public:
    explicit FileData(int c, int argC, const vector<string>& argV);
    void process(const string& fileInName);
    const string& getFileDataName() const;
    const string& getDelimiter() const;
};

//----------------------------------------------------------------------------//
//**************************** CmdArgs::Delimiter ****************************//
//----------------------------------------------------------------------------//

class CmdArgs::Version {
  private:
    const string
        m_program     {"ColDataUtil"},
        m_version     {"0.4"},
        m_copyright   {"Copyright (C) 2020"},
        m_author      {"Syed Ahmad Raza"},
        m_email       {"git@ahmads.org"},
        m_msg;

    Version(const Version&) = delete;
    Version& operator=(const Version&) = delete;

  public:
    explicit Version();
    const string& getMsg() const;
};

//----------------------------------------------------------------------------//

#endif