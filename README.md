NAME
        ColDataUtil

SYNOPSIS
        coldatautil [FILE] [OPTIONS]...

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

        A brief description of the usage and options is given below.

OPTIONS
        -h, --help
                output this file and exit

        -i, --input <FILENAME>
                specify the FILENAME of the file for processing; FILENAME is
                mandatory; if the FILENAME is specified as the first option
                after the program name, then preceding it with -i or --input is
                not necessary

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

        -C, -a, --calc, --calculations [CALCULATION1, CALCULATION2, ...]
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

        -y, --cycle COLUMNNAME [c=COLUMN(NUMBER or NAME)]
                    [r=BEGINROW ENDROW] [t=BEGINTIMESTEP ENDTIMESTEP]
                    [f/l/first/last] [NUMBEROFCYCLES] [m=MEAN]
                command to count the number of cycles in the given file;
                the name of the column to be used for counting cycles must be
                given, either stated directly after the option or given using
                c=COLUMNNAME, or alternatively, the column number may be given
                using c=COLUMNNUMBER (using zero-based indexing);
                other options are described below:

                r=BEGINROW ENDROW: by default, all of the data is traversed from
                        start to finish to find the total number of cycles.
                        However, the BEGINROW and ENDROW may be specified to
                        count the number of cycles within this range of data. If
                        a number of cycles is specified, then only one
                        argument r=ROW may be passed to specify the BEGINROW to
                        count the number of cycles from the end to BEGINROW,
                        which by default will use the "last" option, or f/first
                        may be specified along with the number of cycles and
                        r=ROW to fix the ENDROW and count the number of cycles
                        from the beginning to ENDROW.
                t=BEGINTIMESTEP ENDTIMESTEP: by default, all of the data is
                        traversed from start to finish to find the total number
                        of cycles. However, the BEGINTIMESTEP and ENDTIMESTEP
                        may be specified to count the number of cycles within
                        this range of data. If a number of cycles is specified,
                        then only one argument t=TIMESTEP may be passed to
                        specify the BEGINTIMESTEP to count the number of cycles
                        from the end to BEGINTIMESTEP, which by default will use
                        the "last" option, or f/first may be specified along
                        with the number of cycles and t=TIMESTEP to fix the
                        ENDTIMESTEP and count the number of cycles from the
                        beginning to ENDTIMESTEP. A correctly formatted timestep
                        column must be available to use this option.
                f/l/first/last: by default, the given range is traversed from
                        start to finish to find the total number of cycles.
                        However, any of f/l/first/last may be specified to count
                        cycles from the beginning of the given range (f or
                        first), or the end of the given range (l or last). The
                        NUMBEROFCYCLES must be specified along with this option.
                        E.g. f 20 will count the first 20 cycles; l 15 will
                        count the last 15 cycles from the end of the given range
                        of data (or the whole data column if no range is
                        specified using r= or t=).
                NUMBEROFCYCLES: by default, the given range is traversed from
                        start to finish to find the total number of cycles. If
                        NUMBEROFCYCLES is specified without specifying any of
                        f/l/first/last, then "last" is assumed and the given
                        number of cycles are counted from the end of the given
                        range of data (or the whole data column if no range is
                        specified using r= or t=). Otherwise, one of f/l/first/
                        last may be specified along with the NUMBEROFCYCLES. In
                        either case, the given NUMBEROFCYCLES is searched for
                        and the data range is output to the screen by specifying
                        the range of rows (and timesteps, if a correctly
                        formatted timestep column is available). All the
                        calculations (if specified) and the FFT is determined
                        (if specified) for the identified range of data.
                m=MEAN: by default, the cycles are counted assuming a mean
                        position of zero. However, another mean position can be
                        stated using this option.

        -f, --fourier COLUMNNAME [c=COLUMN(NUMBER or NAME)] [o=FILENAME]
                command to calculate the Fast Fourier Transform (FFT) of the
                given range of data in the given file;
                the name of the column to be used for counting cycles must be
                given, either stated directly after the option or given using
                c=COLUMNNAME, or alternatively, the column number may be given
                using c=COLUMNNUMBER (using zero-based indexing); however, if
                the COLUMNNAME has already been specified for the Cycle option
                (see above), then it cannot be specified again;
                by default, the filename is generated automatically using the
                input filename, the COLUMNNUMBER and the BEGINROW and ENDROW but
                another filename may be given using o=FILENAME.

        -o, --output [FILENAME]
                specify the FILENAME of the output file; if the option -o or
                --output is specified but a FILENAME is not given, by default,
                the FILENAME is generated automatically using the input filename

        --print-data [DELIMITER]
                command to print the given input data range to the screen; a
                DELIMITER may optionally be specified

        --file-data [DELIMITER]
                command to file the given input data range to a file whose
                filename is generated automatically; a DELIMITER may optionally
                be specified

        --delimiter, --separator <DELIMITER>
                a DELIMITER for the input file may be specified using this
                option; by default, a comma-separated file (.csv) or space-
                separated file is assumed

        -v, --version
                output program information, version, etc. and exit
