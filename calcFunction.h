/**
 * @file        calcFunction.h
 *
 * @project     colDataUtil
 * @version     0.1
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 * @date        2020-11-19
 *
 * @brief       Functions for calculations on column data.
 */

#ifndef CALCFUNCTION_H
#define CALCFUNCTION_H

#include "columnData.h"
#include <cstddef>
#include <unordered_map>
#include <string>
#include <cmath>

using std::string;
using namespace ColData;

namespace CalcFunction {
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
    template<typename T> const std::unordered_map<
        double(*)(T, size_t, size_t), string> mapCalcFncToStr;
}

template<typename T>
const std::unordered_map<double(*)(T, size_t, size_t), string>
    CalcFunction::mapCalcFncToStr = {
        {CalcFunction::findMin,           "minimum"},
        {CalcFunction::findMax,           "maximum"},
        {CalcFunction::findAbsMin,        "absolute minimum"},
        {CalcFunction::findAbsMax,        "absolute maximum"},
        {CalcFunction::findMean,          "mean"},
        {CalcFunction::findQuadraticMean, "quadratic mean (RMS)"},
        {CalcFunction::findCubicMean,     "cubic mean"}
    };

/*
 * Find the maximum value of the given column using its Id number or name and
 * return the result.
 */
template<typename T>
double CalcFunction::findMax(const T column,
        const size_t rowBgn, const size_t rowEnd) {
    const vector<double> colData{DoubleV::getOnePFromCol(column)->getData()};
    double maxVal{colData[rowBgn]};
    for (size_t row=rowBgn+1; row<=rowEnd; ++row) {
        if (maxVal<colData[row]) { maxVal = colData[row]; }
    }
    return maxVal;
}

/*
 * Find the minimum value of the given column using its Id number or name and
 * return the result.
 */
template<typename T>
double CalcFunction::findMin(const T column,
        const size_t rowBgn, const size_t rowEnd) {
    const vector<double> colData{DoubleV::getOnePFromCol(column)->getData()};
    double minVal{colData[rowBgn]};
    for (size_t row=rowBgn+1; row<=rowEnd; ++row) {
        if (minVal>colData[row]) { minVal = colData[row]; }
    }
    return minVal;
}

/*
 * Find the absolute maximum value of the given column using its Id number or
 * name and return the result.
 */
template<typename T>
double CalcFunction::findAbsMax(const T column,
        const size_t rowBgn, const size_t rowEnd) {
    const vector<double> colData{DoubleV::getOnePFromCol(column)->getData()};
    double maxVal{std::abs(colData[rowBgn])};
    for (size_t row=rowBgn+1; row<=rowEnd; ++row) {
        if (maxVal<std::abs(colData[row])) { maxVal = std::abs(colData[row]); }
    }
    return maxVal;
}

/*
 * Find the absolute minimum value of the given column using its Id number or
 * name and return the result.
 */
template<typename T>
double CalcFunction::findAbsMin(const T column,
        const size_t rowBgn, const size_t rowEnd) {
    const vector<double> colData{DoubleV::getOnePFromCol(column)->getData()};
    double minVal{std::abs(colData[rowBgn])};
    for (size_t row=rowBgn+1; row<=rowEnd; ++row) {
        if (minVal>std::abs(colData[row])) { minVal = std::abs(colData[row]); }
    }
    return minVal;
}

/*
 * Find the mean of the given column using its Id number or name and return
 * the result.
 */
template<typename T>
double CalcFunction::findMean(const T column,
        const size_t rowBgn, const size_t rowEnd) {
    return DoubleV::getOnePFromCol(column)->getSum(rowBgn, rowEnd)
            /(rowEnd - rowBgn + 1);
}

/*
 * Find the quadratic mean (RMS) of the given column using its Id number or
 * name and return the result.
 */
template<typename T>
double CalcFunction::findQuadraticMean(const T column,
        const size_t rowBgn, const size_t rowEnd) {
    return std::sqrt(
                DoubleV::getOnePFromCol(column)->getSumOfSquares(rowBgn, rowEnd)
                    /(rowEnd - rowBgn + 1)
            );
}

/*
 * Find the cubic mean of the given column using its Id number or
 * name and return the result.
 */
template<typename T>
double CalcFunction::findCubicMean(const T column,
        const size_t rowBgn, const size_t rowEnd) {
    return std::cbrt(
                DoubleV::getOnePFromCol(column)->getSumOfCubes(rowBgn, rowEnd)
                    /(rowEnd - rowBgn + 1)
            );
}

#endif