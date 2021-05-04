/**
 * @version     ColDataUtil 1.4
 * @author      Syed Ahmad Raza (git@ahmads.org)
 * @copyright   GPLv3+: GNU Public License version 3 or later
 *
 * @file        mappings.h
 * @brief       All the maps.
 */

#ifndef MAPPINGS_H
#define MAPPINGS_H

#include "namespaces.h"

using namespace CmdArgs;

inline const unordered_map<string, Option> CmdArgs::mapStrToOption {
    // {"-s",              Option::fileIn},
    {"-i",              Option::fileIn},
    {"-c",              Option::column},
    {"-r",              Option::row},
    {"-t",              Option::timestep},
    {"-y",              Option::cycle},
    {"-f",              Option::fourier},
    {"-C",              Option::calculation},
    // {"-d",              Option::fileOut},
    {"-o",              Option::fileOut},
    {"-h",              Option::help},
    {"-v",              Option::version},

    {"--delimiter",     Option::delimiter},
    {"--separator",     Option::delimiter},
    // {"--source",        Option::fileIn},
    {"--input",         Option::fileIn},
    {"--col",           Option::column},
    // {"--column",        Option::column},
    // {"--columns",       Option::column},
    {"--row",           Option::row},
    // {"--rows",          Option::row},
    // {"--time",          Option::timestep},
    {"--timestep",      Option::timestep},
    // {"--timesteps",     Option::timestep},
    {"--cycle",         Option::cycle},
    // {"--cycles",        Option::cycle},
    {"--fourier",       Option::fourier},
    {"--calc",          Option::calculation},
    // {"--calculation",   Option::calculation},
    // {"--destination",   Option::fileOut},
    {"--output",        Option::fileOut},
    {"--print-data",    Option::printData},
    {"--file-data",     Option::fileData},
    {"--help",          Option::help},
    {"--version",       Option::version},
};

inline const unordered_map<string, CalcId> CmdArgs::mapStrToCalc {
    {"min",                 CalcId::findMin},
    {"minimum",             CalcId::findMin},
    {"max",                 CalcId::findMax},
    {"maximum",             CalcId::findMax},
    {"absmin",              CalcId::findAbsMin},
    {"minabs",              CalcId::findAbsMin},
    // {"absolute-minimum",    CalcId::findAbsMin},
    {"absmax",              CalcId::findAbsMax},
    {"maxabs",              CalcId::findAbsMax},
    // {"absolute-maximum",    CalcId::findAbsMax},
    {"mean",                CalcId::findMean},
    {"avg",                 CalcId::findMean},
    {"average",             CalcId::findMean},
    {"rms",                 CalcId::findRMS},
    // {"rms-mean",            CalcId::findRMS},
    // {"rms-avg",             CalcId::findRMS},
    {"quadratic",           CalcId::findRMS},
    // {"quadratic-rms",       CalcId::findRMS},
    // {"quadratic-mean",      CalcId::findRMS},
    {"frms",                CalcId::findFluctuationRMS},
    {"rmsf",                CalcId::findFluctuationRMS},
    {"cubic",               CalcId::findCubicMean},
    {"cubicmean",           CalcId::findCubicMean},
    {"cubicavg",            CalcId::findCubicMean},
    // {"fourier",             CalcId::findFourier},
};

template<typename T> inline const unordered_map<CmdArgs::CalcId, calcType>
        Output::mapCalcIdToCalc {
    {CmdArgs::CalcId::findMin,            CalcFnc::findMin},
    {CmdArgs::CalcId::findMax,            CalcFnc::findMax},
    {CmdArgs::CalcId::findAbsMin,         CalcFnc::findAbsMin},
    {CmdArgs::CalcId::findAbsMax,         CalcFnc::findAbsMax},
    {CmdArgs::CalcId::findMean,           CalcFnc::findMean},
    {CmdArgs::CalcId::findRMS,            CalcFnc::findRMS},
    {CmdArgs::CalcId::findFluctuationRMS, CalcFnc::findFluctuationRMS},
    {CmdArgs::CalcId::findCubicMean,      CalcFnc::findCubicMean},
    // {CmdArgs::CalcId::findFourier,        CalcFnc::findFourier},
};

template<typename T>
inline const unordered_map<calcType, string> CalcFnc::mapCalcToStr {
    {CalcFnc::findMin,           "minimum"},
    {CalcFnc::findMax,           "maximum"},
    {CalcFnc::findAbsMin,        "absolute minimum"},
    {CalcFnc::findAbsMax,        "absolute maximum"},
    {CalcFnc::findMean,          "mean"},
    {CalcFnc::findRMS,           "RMS (quadratic mean)"},
    {CalcFnc::findFluctuationRMS,"Fluctuation RMS"},
    {CalcFnc::findCubicMean,     "cubic mean"},
    // {CalcFnc::findFourier,       "fourier"}
};

inline const unordered_map<string, CycleInit> CmdArgs::mapStrToCycleInit {
    {"f",       CycleInit::first},
    {"first",   CycleInit::first},
    {"l",       CycleInit::last},
    {"last",    CycleInit::last}
};

#endif