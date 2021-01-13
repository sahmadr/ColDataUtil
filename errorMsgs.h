/**
 * @file        errorMsgs.h
 *
 * @project     colDataUtil
 * @version     0.2
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

errorNoArguments{"colDataUtil requires arguments. At "
    "least the location of the input file to be processed must be provided. "
    "Please consult the documentation for additional information."},
errorInvalidOption{"An invalid option has been specified. Please consult the "
    "documentation."},
errorInvalidValue{"An invalid value has been specified, which is not preceded "
    "by a proper option. Please consult the documentation."},
errorFileInNameMissing{"File input option has been used but the file name has "
    "not been specified."},
errorFileInMissing{"The location of the input file to be processed must be "
    "provided. Please consult the documentation for additional information."},
errorDlmSpecifiedAlready{"Multiple delimiters cannot be specified."},
errorFileInNamedAlready{"Multiple input file names cannot be specified."},
errorCalcNameInvalid{"An invalid function name has been specified. Please "
    "consult the documentation."},
errorColIsInt{"An integer column has been specified. Calculations cannot be "
    "performed on an integer column."},
errorColNamesInvalid{"None of the specified columns match the columns in the "
    "input file."},
errorFileOutNameMissing{"File output option has been used but the file name "
    "has not been specified."},
generalErrorMessage{"Please contact the developer if further assistance is "
    "required. Thank you."},

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
errorColNameAbsent{"Requested column name not found."},
errorColNoAbsent{"Requested column number not found."},
errorColAbsent{"Requested column not found."},
errorTimeStepTooSmall{"Invalid timestep specified. The specified timestep is "
    "less than the smallest found in the file."},
errorTimeStepTooLarge{"Invalid timestep specified. The specified timestep is "
    "larger than the largest found in the file."},
errorInputFile{"Input file could not be opened."},
errorOutputFile{"Output file could not be opened."}

;

#endif