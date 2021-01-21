/**
 * @file        helpDoc.h
 *
 * @project     colDataUtil
 * @version     0.3
 *
 * @author      Syed Ahmad Raza (git@ahmads.org)
 *
 * @brief       Help; documentation on program usage.
 *
 */

#ifndef HELPDOC_H
#define HELPDOC_H

#include "namespaces.h"

inline const string

helpIntro{
"colDataUtil 0.2\n"
"Copyright (C) 2021 Syed Ahmad Raza (git@ahmads.org)\n\n"
"A utility program for loading, writing and performing calculations\n"
"on column data in comma or space delimited files. This program has\n"
"been written primarily for timestep column data obtained from solvers\n"
"used widely in computational fluid dynamics, it can be used for any\n"
"data file having the correct formatting. The calculations can only\n"
"be performed for the non-integer columns.\n"
},
helpGeneral{
"This program requires a correctly formatted input data file to work.\n\n"
"The filename may be entered as the first argument. Otherwise, the\n"
"filename must be preceded by an option flag -i or --input if it is\n"
"given after other options.\n\n"
"By default, a whitespace- or comma-delimited file is expected.\n"
"If your input file includes a different delimiter, it may be\n"
"specified using --separator option.\n"
"The numbering of columns and rows follows zero-based indexing.\n\n"
}

;

#endif