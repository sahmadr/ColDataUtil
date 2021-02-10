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

        -i, --input <FILENAME>
                specify the FILENAME of the file for processing; FILENAME is
                mandatory

        -r, --row [START] [END]
                specify the starting row, or both the starting and the ending
                row for calculations; index starts from zero

        -t, --timestep [START] [END]
                specify the starting timestep, or both the starting and the
                ending timestep for calculations;
                a correctly formatted timestep column is mandatory for this
                option to work;
                the timestep column should be an integer column with "step" in
                its column header

        -c, --col [COLUMN1, COLUMN2, ...]
                specify the column (or columns) for calculations; column(s) can
                be specified by its index (with index starting from zero) or its
                exact name

        -a, --calc, --calculations [CALCULATION1, CALCULATION2, ...]
                specify the calculations to perform;
                by default, following will be performed:...;
                the names of all the possible calculation options are given
                below, followed by the possible keywords that can be used to
                call them:
                1.  Minimum: min, minimum
                2.  Maximum: max, maximum
                3.  Absolute minimum: abs-min, absmin, absolute-minimum
                4.  Absolute maximum: abs-max, absmax, absolute-maximum
                5.  Mean: mean, avg, average
                6.  Quadratic mean (RMS): rms, quadratic, quadratic-mean
                7.  Cubic mean: cubic, cubic-mean, cubic-avg


[To be continued... In the meanwhile, please contact the developer for further
assistance.]
