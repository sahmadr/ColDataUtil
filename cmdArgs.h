/**
 * @version     ColDataUtil 1.4
 * @author      Syed Ahmad Raza (git@ahmads.org)
 * @copyright   GPLv3+: GNU Public License version 3 or later
 *
 * @file        cmdArgs.h
 * @brief       Read command line input options.
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
    Cycle*                  m_cycleP;       // number of cycles
    Fourier*                m_fourierP;     // Fourier transform (FFT)
    FileOut*                m_fileOutP;     // file where to save calculations
    PrintData*              m_printDataP;   // print data to the screen
    FileData*               m_fileDataP;    // file data to a file
    Help*                   m_helpP;        // help
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
    const Cycle* getCycleP() const;
    const Fourier* getFourierP() const;
    const FileOut* getFileOutP() const;
    const PrintData* getPrintDataP() const;
    const FileData* getFileDataP() const;
    const Help* getHelpP() const;
    const Version* getVersionP() const;

    void resolveRowVsTimestep();
    void resolveCycle();
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
    vector<int>                   m_intInputColSet{};
    vector<string>                m_strInputColSet{};
    int                           m_dataColTotal{0};
    vector<int>                   m_dataDoubleColSet{};
    vector<ColData::DoubleV*>     m_dataDoubleVSetP;

    Column(const Column&) = delete;
    Column& operator=(const Column&) = delete;

  public:
    explicit Column() = default;
    explicit Column(int c, int argC, const vector<string>& argV);

    void init(int c, int argC, const vector<string>& argV);
    void importDataColTotal(int dataColTotal);
    void importDataDouble(const vector<ColData::DoubleV*>& dataDoubleVSetP);

    void process(const ColData::IntV* dataTimestepIVP);

    const vector<int>& getIntInputColSet() const;
    const vector<string>& getStrInputColSet() const;
    int getDataColTotal() const;
    const vector<int>& getDataDoubleColSet() const;
    const vector<ColData::DoubleV*>& getDataDoubleVSetP() const;
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

    void importDataRowTotal(size_t dataRowTotal);
    void process(bool rowDefined1, bool rowDefined2,
        size_t row1, size_t row2, CycleInit cycleInitStatus);
    void process();

    void setRowBgn(size_t val);
    void setRowEnd(size_t val);
    void setRowEndFromData();
    size_t getDataRowTotal() const;
    tuple<size_t, size_t> getRange() const;
    size_t getRowBgn() const;
    size_t getRowEnd() const;
    tuple<bool, bool> getDefStatus() const;
};

//----------------------------------------------------------------------------//
//**************************** CmdArgs::Timestep *****************************//
//----------------------------------------------------------------------------//

class CmdArgs::Timestep {
  private:
    bool                        m_timestepBgnDefined{false};
    bool                        m_timestepEndDefined{false};
    size_t                      m_timestepBgn{0};
    size_t                      m_timestepEnd{0};
    bool                        m_timestepConsistent{false};
    const ColData::IntV*        m_dataTimestepIVP{nullptr};
    tuple<bool, size_t, size_t> m_dataTimestepRange{0,0,0};

    Timestep(const Timestep&) = delete;
    Timestep& operator=(const Timestep&) = delete;

  public:
    explicit Timestep() = default;
    explicit Timestep(int c, int argC, const vector<string>& argV);
    void setTimestepEnd(int c, int argC, const vector<string>& argV);

    void importDataTimestep(const ColData::IntV* dataTimestepIVP);
    void process(bool timestepDefined1, bool timestepDefined2,
        size_t timestep1, size_t timestep2, CycleInit cycleInitStatus,
        double timeIncrement);
    void process();

    void setTimestepBgn(size_t val);
    void setTimestepEnd(size_t val);
    void setTimestepBgnFromData();
    void setTimestepEndFromData();
    void setTimestepBgnFromRow(size_t val);
    void setTimestepEndFromRow(size_t val);

    const ColData::IntV* getDataTimestepIVP() const;
    tuple<bool, size_t, size_t> getDataTimestepRange() const;
    tuple<size_t, size_t> getRange() const;
    size_t getTimestepBgn() const;
    size_t getTimestepEnd() const;
    tuple<bool, bool> getDefStatus() const;
    bool isTimestepConsistent() const;
};

//----------------------------------------------------------------------------//
//****************************** CmdArgs::Cycle ******************************//
//----------------------------------------------------------------------------//

class CmdArgs::Cycle {
  private:
    size_t                  m_argC{0};
    vector<string>          m_argV{};
    const size_t            m_maxArgs{10};
    CycleInit               m_cycleInit{CycleInit::empty};
    int                     m_cycleInputCount{-1};
    int                     m_cycleColNo{-1};
    double                  m_center{0.0};
    tuple<bool, bool>       m_rowDefined{false, false};
    tuple<size_t, size_t>   m_rowRange{0, 0};
    tuple<bool, bool>       m_timestepDefined{false, false};
    tuple<size_t, size_t>   m_timestepDefRange{0, 0};
    string                  m_fileName{""};
    ColData::CycleData      m_calcCycleData{};
    int                     m_simTimeColNo{-1};
    double                  m_timeIncrement{-1.0};
    double                  m_frequency{-1.0};
    double                  m_minAmplitude{0.0};
    size_t                  m_minRowInterval{0};

    Cycle(const Cycle&) = delete;
    Cycle& operator=(const Cycle&) = delete;

  public:
    explicit Cycle() = default;
    explicit Cycle(int c, int argC, const vector<string>& argV);

    void init(int c, int argC, const vector<string>& argV);

    void process(const vector<ColData::DoubleV*>& dataDoubleVSetP);

    void setCalcCDataAndCycleInputCount(ColData::CycleData);
    void setFrequency(size_t rowBgn, size_t rowEnd);
    void setFrequency(tuple <size_t, size_t> timestepRange);
    void setAutoFileName(const size_t rowBgn, const size_t rowEnd,
        const string& fileInName);

    int getInputCount() const;
    CycleInit getInitType() const;
    int getCycleColNo() const;
    int getSimTimeColNo() const;
    double getCenter() const;
    double getTimeIncrement() const;
    double getFrequency() const;
    double getMinAmplitude() const;
    size_t getMinRowInterval() const;
    const tuple<size_t, size_t> getRowDefRange() const;
    const tuple<size_t, size_t> getTimestepDefRange() const;
    const tuple<bool, bool> getRowDefStatus() const;
    const tuple<bool, bool> getTimestepDefStatus() const;
    const string& getFileName() const;
    const ColData::CycleData& getCalcCycleData() const;
};

//----------------------------------------------------------------------------//
//***************************** CmdArgs::Fourier *****************************//
//----------------------------------------------------------------------------//

class CmdArgs::Fourier {
  private:
    size_t                  m_argC{0};
    vector<string>          m_argV{};
    int                     m_colNo{-1};
    tuple<size_t, size_t>   m_rowRange{0, 0};
    string                  m_fileName{""};

    Fourier() = delete;
    Fourier(const Fourier&) = delete;
    Fourier& operator=(const Fourier&) = delete;

  public:
    explicit Fourier(int c, int argC, const vector<string>& argV);

    void init(int c, int argC, const vector<string>& argV);

    void process(const vector<ColData::DoubleV*>& dataDoubleVSetP,
        const int colNo, const size_t rowBgn, const size_t rowEnd,
        const string& fileInName);

    int getColNo() const;
    const tuple<size_t, size_t> getRowRange() const;
    const string& getFileName() const;
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
    const string& getFileName() const;
    const string& getDelimiter() const;
};

//----------------------------------------------------------------------------//
//****************************** CmdArgs::Help *******************************//
//----------------------------------------------------------------------------//

class CmdArgs::Help {
  private:
    const string
        m_helpFileName {"/opt/ColDataUtil/README.md"};

    Help(const Help&) = delete;
    Help& operator=(const Help&) = delete;

  public:
    explicit Help() = default;
    const string& getHelpFileName() const;
};

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//***************************** CmdArgs::Version *****************************//
//----------------------------------------------------------------------------//

class CmdArgs::Version {
  private:
    const string
        m_program     {"ColDataUtil"},
        m_version     {"1.4"},
        m_copyright   {"Copyright (C) 2020"},
        m_author      {"Syed Ahmad Raza"},
        m_email       {"git@ahmads.org"},
        m_license     {"This is free software: you are free to change and "
                       "redistribute it.\nThere is NO WARRANTY, to the extent "
                       "permitted by law; see the source."},
        m_msg;

    Version(const Version&) = delete;
    Version& operator=(const Version&) = delete;

  public:
    explicit Version();
    const string& getMsg() const;
};

//----------------------------------------------------------------------------//

#endif