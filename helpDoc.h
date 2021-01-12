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
"Copyright (C) 2021 Syed Ahmad Raza (git@ahmads.org)\n"

},
helpGeneral{
"This program requires an input data file to work. The filename may be \n"
"entered as the first argument. Otherwise, the filename must be preceded by\n"
"an option flag -i or --input if it is given after other options.\n"
}

;

#endif