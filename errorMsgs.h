/**
 * @file        errorMsgs.h
 *
 * @project     ColDataUtil
 * @version     0.4
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       All the error messages.
 *
 */

#ifndef ERRORMSGS_H
#define ERRORMSGS_H

#include "namespaces.h"

inline const string

errorNoArguments{"colDataUtil requires arguments. At least the location of the "
    "input file to be processed must be provided."},
errorInvalidOption{"An invalid option has been specified."},
errorInvalidValue{"An invalid value has been specified, which is not preceded "
    "by a proper option."},
errorFileInNameMissing{"File input option has been used but the file name has "
    "not been specified."},
errorFileInMissing{"The location of the input file to be processed must be "
    "provided."},
errorDlmSpecifiedAlready{"Multiple delimiters cannot be specified."},
errorFileInNamedAlready{"Multiple input file names cannot be specified."},
errorCalcNameInvalid{"An invalid calculation name has been specified."},
errorColIsTimeStep{"The timestep column has been specified. Calculations "
    "cannot be performed on the timestep column."},
errorColNamesInvalid{"None of the specified columns match the columns in the "
    "input file."},
errorFileOutNameMissing{"File output option has been used but the file name "
    "has not been specified."},

errorColNameDouble{"Double vector column name repeated. Please change one of "
    "the column names."},
errorColNameInt{"Integer vector column name repeated. Please change one of the "
    "column names."},
errorHeaderFormatIncorrect{"An incorrect format has been used for the header "
    "line in the file."},
errorDlmFormatIncorrect{"An incorrect delimiter has been specified OR an "
    "incorrect data format has been used in the file. The size of the columns "
    "do not match."},
errorHeaderDlmTypeUndefined{"The delimiter type of header was undefined. This "
    "could be due to an incorrect formatting of the header line. Please report "
    "this issue. Thank you."},
errorDataFormatIncorrect{"An incorrect format has been used for the numeric "
    "data in the file."},
errorDataInvalid{"Invalid data entered. Data does not cross the given center "
    "value."},
errorColNameAbsent{"Requested column name not found."},
errorColNoAbsent{"Requested column number not found."},
errorColAbsent{"Requested column not found."},
errorRowRangeInvalid{"Requested range of rows is invalid."},
errorRowsAlreadySpecified{"Please specify only two values for the row option."},
errorRowTimestepConflict{"The row option and the timestep option both have "
    "been specified. Please choose only one of these options."},
errorDataTimestepInconsistent{"The correct timestep column was not identified "
    "from the input file or it has negative values."},
errorTimestepRangeInvalid{"Requested range of timesteps is invalid."},
errorTimeStepTooSmall{"Invalid timestep specified. The specified timestep is "
    "less than the smallest found in the input file."},
errorTimeStepTooLarge{"Invalid timestep specified. The specified timestep is "
    "larger than the largest found in the input file."},
errorTimestepsAlreadySpecified{"Please specify only two values for the "
    "timestep option."},
errorCycleInvalid{"Requested number of cycles is invalid."},
errorCycleMissing{"Number of cycles have not been specified. Please enter the "
    "required number of cycles or remove the \"first\" or \"last\" flag."},
errorCycleCenterInvalid{"Requested value for center of cycles is invalid."},
errorCycleNotAvailable{"Requested number of cycles are not available in the "
    "given data file for the given column."},
errorCycleRowTimestepConflict{"The cycle option has been specified along with "
    "the row or the timestep option. The row or timestep options cannot be "
    "with the cycle option. Please enter the required rows or timesteps as "
    "arguments to the cycle option, e.g. \"r=50 r=200\", or \"t=50 t=200\"."},
errorCycleWithRowTimestepConflict{"The row argument as well as and the "
    "timestep argument both have been specified. Please specify only one of "
    "these two types of arguments to the cycle option."},
errorCycleAlreadySpecified{"Number of cycles has already been spcecified."},
errorCycleArgumentInvalid{"An invalid argument has been specified for the "
    "cycle option."},
errorCycleColNameInvalid{"The column name specified for the cycle option is "
    "invalid."},
errorCycleColNameMissing{"A mandatory argument for the cycle option is "
    "missing. Please specify the column to determine the cycles."},
errorCycleTooFewArguments{"Too few arguments have been specified for the cycle "
    "option."},
errorCycleTooManyArguments{"Too many arguments have been specified for the "
    "cycle option."},
errorCycleInvalidForCalc{"No cycles were found for calculation."},
errorInputFile{"Input file could not be opened."},
errorOutputFile{"Output file could not be opened."},
errorUnknown{"Unknown error. Please contact the developer."},

generalErrorMessage{"\n\tPlease use the option -h or --help for additional "
    "information, or consult the documentation. Please contact the developer if "
    "further assistance is required. Thank you."}
;

#endif