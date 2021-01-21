/**
 * @file        cmdArgs.h
 *
 * @project     colDataUtil
 * @version     0.3
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
    Delimitation            m_dataDlmType;  // data delimitation type
    int                     m_dataColTotal; // total number of columns
    size_t                  m_dataRowTotal; // total number of rows
    Delimiter*              m_delimiterP;   // delimiter
    FileIn*                 m_fileInP;      // file with data to be processed
    Calc*                   m_calcP;        // value functions to be calculated
    Column*                 m_columnP;      // columns to be processed
    Row*                    m_rowP;         // range of rows
    Timestep*               m_timestepP;    // range of timesteps
    FileOut*                m_fileOutP;     // file where to save calculations
    PrintData*              m_printDataP;   // print data to the screen
    FileData*               m_fileDataP;    // file data to a file

    Args() = delete;
    Args(const Args&) = delete;
    Args& operator=(const Args&) = delete;

  public:
    explicit Args(int argc, char* argv[]);
    void process();

    static int setCount(int newCount);
    static int getCount();

    void setDataColTotal(int dataColTotal) { m_dataColTotal = dataColTotal; }
    void setDataDlmType(Delimitation dataDlmType) {m_dataDlmType = dataDlmType;}

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
    int             m_dataColTotal{0};
    size_t          m_dataRowTotal{0};
    Delimitation    m_dataDlmType{Delimitation::undefined};


    FileIn() = delete;
    FileIn(const FileIn&) = delete;
    FileIn& operator=(const FileIn&) = delete;

  public:
    explicit FileIn(int c, int argC, const vector<string>& argV);
    explicit FileIn(const vector<string>& argV);
    void process(const string& delimiter);
    const string& getFileLocation() const;
    int getDataColTotal() const;
    size_t getDataRowTotal() const;
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
    vector<int>     m_doubleColSet{};

    Column(const Column&) = delete;
    Column& operator=(const Column&) = delete;

  public:
    explicit Column() = default;
    explicit Column(int c, int argC, const vector<string>& argV);
    void setColInputSets(int c, int argC, const vector<string>& argV);
    void process();
    const vector<int>& getDoubleColSet() const;
    const vector<int>& getIntInputColSet() const;
    const vector<string>& getStrInputColSet() const;
};

//----------------------------------------------------------------------------//
//******************************* CmdArgs::Row *******************************//
//----------------------------------------------------------------------------//

class CmdArgs::Row {
  private:
    size_t  m_rowBgn{0};
    size_t  m_rowEnd{0};

    Row(const Row&) = delete;
    Row& operator=(const Row&) = delete;

  public:
    explicit Row() = default;
    explicit Row(int c, int argC, const vector<string>& argV);
    void process(const FileIn* fileIn);
    void setRowEnd(int c, int argC, const vector<string>& argV);
    const tuple<size_t, size_t> getRowRange() const;
};

//----------------------------------------------------------------------------//
//**************************** CmdArgs::Timestep *****************************//
//----------------------------------------------------------------------------//

class CmdArgs::Timestep {
  private:
    size_t  m_timestepBgn{0};
    size_t  m_timestepEnd{0};

    Timestep() = delete;
    Timestep(const Timestep&) = delete;
    Timestep& operator=(const Timestep&) = delete;

  public:
    explicit Timestep(int c, int argC, const vector<string>& argV);
    void process(const FileIn* fileIn);
    const tuple<size_t, size_t> getTimestepRange() const;
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
    void setFileLocSet(int c, int argC, const vector<string>& argV);
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
//**************************** CmdArgs::Timestep *****************************//
//----------------------------------------------------------------------------//

#endif