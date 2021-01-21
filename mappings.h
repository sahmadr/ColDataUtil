/**
 * @file        mappings.h
 *
 * @project     colDataUtil
 * @version     0.4
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       All the maps.
 *
 */

#ifndef MAPPINGS_H
#define MAPPINGS_H

#include "namespaces.h"

using namespace CmdArgs;

inline const unordered_map<string, Option> CmdArgs::mapStrToOption {
    // {"-s",              Option::fileIn},
    {"-i",              Option::fileIn},
    {"-f",              Option::function},
    {"-c",              Option::column},
    {"-r",              Option::row},
    {"-t",              Option::timestep},
    // {"-d",              Option::fileOut},
    {"-o",              Option::fileOut},
    {"-h",              Option::help},
    {"-v",              Option::version},
    {"--delimiter",     Option::delimiter},
    {"--separator",     Option::delimiter},
    // {"--source",        Option::fileIn},
    {"--input",         Option::fileIn},
    {"--fnc",           Option::function},
    {"--function",      Option::function},
    {"--col",           Option::column},
    {"--column",        Option::column},
    {"--columns",       Option::column},
    {"--row",           Option::row},
    {"--rows",          Option::row},
    {"--time",          Option::timestep},
    {"--timestep",      Option::timestep},
    {"--timesteps",     Option::timestep},
    // {"--destination",   Option::fileOut},
    {"--output",        Option::fileOut},
    {"--print-data",    Option::printData},
    {"--file-data",     Option::fileData},
    {"--help",          Option::help},
    {"--version",       Option::version},
};

inline const std::unordered_map<string, CalcId> CmdArgs::mapStrToCalc {
    {"min",                 CalcId::findMin},
    {"minimum",             CalcId::findMin},
    {"max",                 CalcId::findMax},
    {"maximum",             CalcId::findMax},
    {"abs-min",             CalcId::findAbsMin},
    {"absmin",              CalcId::findAbsMin},
    {"absolute-minimum",    CalcId::findAbsMin},
    {"abs-max",             CalcId::findAbsMax},
    {"absmax",              CalcId::findAbsMax},
    {"absolute-maximum",    CalcId::findAbsMax},
    {"mean",                CalcId::findMean},
    {"avg",                 CalcId::findMean},
    {"average",             CalcId::findMean},
    {"rms",                 CalcId::findQuadraticMean},
    {"rms-mean",            CalcId::findQuadraticMean},
    {"rms-avg",             CalcId::findQuadraticMean},
    {"quadratic",           CalcId::findQuadraticMean},
    {"quadratic-rms",       CalcId::findQuadraticMean},
    {"quadratic-mean",      CalcId::findQuadraticMean},
    {"cubic",               CalcId::findCubicMean},
    {"cubic-mean",          CalcId::findCubicMean},
    {"cubic-avg",           CalcId::findCubicMean},
};

template<typename T> inline const unordered_map<CmdArgs::CalcId, calcType>
        Output::mapCalcIdToCalc {
// template<typename T>
// inline const std::unordered_map<CalcId, calcType> CmdArgs::mapCalcIdToCalc {
    {CmdArgs::CalcId::findMin,            CalcFnc::findMin},
    {CmdArgs::CalcId::findMax,            CalcFnc::findMax},
    {CmdArgs::CalcId::findAbsMin,         CalcFnc::findAbsMin},
    {CmdArgs::CalcId::findAbsMax,         CalcFnc::findAbsMax},
    {CmdArgs::CalcId::findMean,           CalcFnc::findMean},
    {CmdArgs::CalcId::findQuadraticMean,  CalcFnc::findQuadraticMean},
    {CmdArgs::CalcId::findCubicMean,      CalcFnc::findCubicMean}
};

// template<typename T>
// inline const std::unordered_map<calcType, string> CalcFnc::mapCalcToStr {
//     {CalcFnc::findMin,           "minimum"},
//     {CalcFnc::findMax,           "maximum"},
//     {CalcFnc::findAbsMin,        "absolute minimum"},
//     {CalcFnc::findAbsMax,        "absolute maximum"},
//     {CalcFnc::findMean,          "mean"},
//     {CalcFnc::findQuadraticMean, "quadratic mean (RMS)"},
//     {CalcFnc::findCubicMean,     "cubic mean"}
// };

template<typename T>
inline const std::unordered_map<calcType, string> CalcFnc::mapCalcToStr {
    {CalcFnc::findMin,           "minimum"},
    {CalcFnc::findMax,           "maximum"},
    {CalcFnc::findAbsMin,        "absolute minimum"},
    {CalcFnc::findAbsMax,        "absolute maximum"},
    {CalcFnc::findMean,          "mean"},
    {CalcFnc::findQuadraticMean, "quadratic mean (RMS)"},
    {CalcFnc::findCubicMean,     "cubic mean"}
};

#endif