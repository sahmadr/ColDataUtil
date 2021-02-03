NAME
        ColDataUtil

SYNOPSIS
        coldatautil [FILE] [OPTION]...

DESCRIPTION
        A utility program for loading, writing and performing calculations
        on column data in spaced or delimited files. This program has been
        written primarily for timestep column data obtained from solvers used
        widely in computational fluid dynamics, it can be used for any data
        file having the correct formatting.

        This program requires a correctly formatted input data file. The
        filename may be entered as the first argument. Otherwise, the
        filename must be preceded by an option flag -i or --input if it is
        given after other options.

        By default, a whitespace- or comma-delimited file is expected.
        If your input file includes a different delimiter, it may be
        specified using --separator or --delimiter option. The numbering of
        columns and rows follows zero-based indexing.

        A brief description of the usage and possible options is given below.

OPTIONS
        -v, --version
                output program information, version, etc. and exit

        -h, --help
                output this file and exit

        -i, --input [FILENAME]


[To be continued... In the meanwhile, please contact the developer for further
assistance.]
