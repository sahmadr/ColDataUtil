/**
 * @file        helpDoc.h
 *
 * @project     colDataUtil
 * @version     0.2
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
"This program requires an input data file to work. The filename may be\n"
"entered as the first argument. Otherwise, the filename must be\n"
"preceded by an option flag -i or --input if it is given after other\n"
"options.\n"
}

;

#endif