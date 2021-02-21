/**
 * @version     ColDataUtil 1.1
 * @author      Syed Ahmad Raza (git@ahmads.org)
 * @copyright   GPLv3+: GNU Public License version 3 or later
 *
 * @file        colData.h
 * @brief       Handle the column data; read it from the files and store it
 *              correctly.
 */

#ifndef COLDATA_H
#define COLDATA_H

#include "namespaces.h"
#include "errorMsgs.h"

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
    explicit IntV(int colNo, string colName, size_t dataRowTotal);

    void addValue(int value);
    static void insertColNoSet(int);

    int getId() const;
    int getColNo() const;
    const string& getColName() const;
    int getCol(int colNo) const;
    string& getCol(string& colName) const;
    const vector<int>& getData() const;
    tuple<bool, size_t, size_t> getTimestepRange() const;
    size_t getRow(int timestepVal) const;

    static int getTotal();
    static set<int>& getColNoSet();
    static vector<IntV*>& getSetP();
    static IntV* getOneP(const int id);
    // static IntV* getOnePFromCol(const int inputColNo);
    // static IntV* getOnePFromCol(const string inputColName);
    template<typename T>
    IntV* getOnePFromCol(const T inputCol) {
        int id{-1};
        T col{inputCol};
        for (IntV* iVP : s_intVSetP) {
            if (inputCol == iVP->getCol(col)) { id = iVP->m_id; }
        }
        if (id<0) { throw runtime_error(errorColAbsent); }
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
    explicit DoubleV(int colNo, string colName, size_t dataRowTotal);

    void addValue(double value);
    static void insertColNoSet(int colNo);

    int getId() const;
    int getColNo() const;
    const string& getColName() const;
    const vector<double>& getData() const;

    static int getTotal();
    static set<int>& getColNoSet();
    static vector<DoubleV*>& getSetP();
    static DoubleV* getOneP(const int id);
    static DoubleV* getOnePFromCol(const int inputColNo);
    static DoubleV* getOnePFromCol(const string inputColName);

    double getSum(const size_t rowBgn, const size_t rowEnd) const;
    double getSumOfSquares(const size_t rowBgn, const size_t rowEnd) const;
    double getSumOfCubes(const size_t rowBgn, const size_t rowEnd) const;
    tuple<int, size_t, size_t> findCycles(const size_t rowBgn,
        const size_t rowEnd, const double mean) const;
    tuple<int, size_t, size_t> findCyclesFirst(const size_t rowBgn,
        const size_t rowEnd, const double mean, const int cycles) const;
    tuple<int, size_t, size_t> findCyclesLast(const size_t rowBgn,
        const size_t rowEnd, const double mean, const int cycles) const;
};

#endif