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

        For options that allow a delimiter to be specified, if a special
        character (e.g. the tab character '\t') needs to be passed on the
        command line, the correct way to specify it on Bash is preceded it with
        a $ sign to do the correct conversion to whitespace, e.g. $'\t'.

        A brief description of the usage and options is given below.

OPTIONS
        -h, --help
                display README.md help file

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
                its column header name

        -c, --col [COLUMN1, COLUMN2, ...]
                specify the column (or columns) for calculations; column(s) can
                be specified by its index (with index starting from zero) or its
                exact column header name

        -C, --calc, --calculations [CALCULATION1, CALCULATION2, ...]
                specify the calculations to perform;
                by default, only a few common ones will be performed;
                the names of all the possible calculation options are given
                below, followed by the possible keywords that can be used to
                call them:

                1. Minimum value in the selected data: min, minimum
                2. Maximum value in the selected data: max, maximum
                3. Absolute minimum value in the selected data: absmin, minabs
                4. Absolute maximum value in the selected data: absmax, maxabs
                5. Mean of the selected data: mean, avg, average
                6. RMS (quadratic mean) of the selected data: rms, quadratic
                7. Fluctuation RMS, which is calculated by taking the square
                        root of the average of squares of the differences
                        between the values in the selected data and their mean
                        value: frms, rmsf
                8. Cubic mean: cubic, cubicmean, cubicavg

        -y, --cycle COLUMNNAME [c=COLUMN(NUMBER or NAME)] [o/o=FILENAME]
                        [r=BEGINROW r=ENDROW] [t=BEGINTIMESTEP t=ENDTIMESTEP]
                        [f/l/first/last] [NUMBEROFCYCLES]
                        [m=CENTER] [a=MINAMPLITUDE] [n=MINROWINTERVAL]
                        [dt=TIMEINCREMENT] [st=SIMTIMECOLUMN]
                command to count the number of cycles in the given file and
                find the peaks of each cycle using its crest and trough, which
                is then used to calculate the maximum peak and the various
                mean values (described below), and optionally, the frequency;
                the name of the column to be used for counting cycles must be
                given, either stated directly after the option or given using
                c=COLUMNNAME, or alternatively, the column number may be given
                using c=COLUMNNUMBER (using zero-based indexing);
                by default, the filing of the crest, trough and peak values is
                suppressed but this behavior may be changed by passing "o" as a
                parameter after specifying -y or --cycle, which automatically
                generates the file using the input filename, the COLUMNNUMBER
                and the BEGINROW and ENDROW, or another filename may be given
                using o=FILENAME;
                the various types of mean values calculated are listed below,
                followed by the other possible options:

                1. Crests mean: is the mean of all the crests in the given
                        range.
                2. Troughs mean: is the mean of all the troughs in the given
                        range.
                3. Peaks mean: is the mean of all the peaks (or amplitudes),
                        where a peak is calculated by subtracting the mean from
                        each crest or trough and taking its absolute value.
                4. 1/3rd peaks mean: is the mean of the highest 1/3rd of the
                        peaks, similar to the significant wave height (SWH) used
                        in physical oceanography.
                5. 1/10th peaks mean: is the mean of the highest 1/10th of the
                        peaks.

                r=BEGINROW r=ENDROW: by default, all of the data is traversed
                        from start to finish to find the total number of cycles.
                        However, the BEGINROW and ENDROW may be specified to
                        count the number of cycles within this range of data. If
                        a number of cycles is specified, then only one
                        argument r=ROW may be passed to specify the BEGINROW to
                        count the number of cycles from the end to BEGINROW,
                        which by default will use the "last" option, or f/first
                        may be specified along with the number of cycles and
                        r=ROW to fix the ENDROW and count the number of cycles
                        from the beginning to ENDROW.
                t=BEGINTIMESTEP t=ENDTIMESTEP: by default, all of the data is
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
                        NUMBEROFCYCLES (an integer number) is specified without
                        specifying any of f/l/first/last, then "last" is assumed
                        and the given number of cycles are counted from the end
                        of the given range of data (or the whole data column if
                        no range is specified using r= or t=). Otherwise, one of
                        f/l/first/last may be specified along with the
                        NUMBEROFCYCLES. In either case, the given NUMBEROFCYCLES
                        is searched for and the data range is output to the
                        screen by specifying the range of rows (and timesteps,
                        if a correctly formatted timestep column is available).
                        All the calculations (if specified) and the FFT is
                        determined (if specified) for the identified range of
                        data.
                m=CENTER: by default, the cycles are counted assuming a center
                        or mean position of zero. However, another center or
                        mean position can be input using this option.
                a=MINAMPLITUDE: by default, all cycles are counted, however
                        small their amplitude maybe. However, in order to filter
                        out the irrelevant minor fluctuations, a minimum
                        amplitude may be specified using this option to count
                        only those cycles that have an amplitude greater than
                        MINAMPLITUDE, which should be input as an absolute
                        value, without the negative sign. This option maybe
                        specified along with n=MINROWINTERVAL, in which case,
                        both the conditions must be satisfied for a cycle to be
                        counted.
                n=MINROWINTERVAL: by default, all cycles are counted, however
                        small the row interval (or half the wave period)
                        maybe. However, in order to filter out the irrelevant
                        minor fluctuations, a minimum row interval maybe
                        specified, which should be a positive integer value. It
                        should be selected considering half the wave period
                        (equal to half the minimum number of rows or data points
                        requried for a cycle). This option maybe specified along
                        with a=MINAMPLITUDE, in which case, both the conditions
                        must be satisfied for a cycle to be counted.
                dt=TIMEINCREMENT: may be used to calculate the frequency of
                        cycles by specifying a value for the time increment, if
                        a correctly formatted timestep column is available in
                        the file (and if the time increment is constant), which
                        should be recognized automatically. The timestep column
                        should be a column with only increasing (ascending)
                        integer values and "step" in the column header name. It
                        cannot be specified together with st=SIMTIMECOLUMN.
                st=SIMTIMECOLUMN: may be used to calculate the frequency of
                        cycles by specifying the column with simulation time. It
                        cannot be specified together with dt=TIMEINCREMENT.

        -f, --fourier COLUMNNAME [c=COLUMN(NUMBER or NAME)] [o/o=FILENAME]
                command to calculate the Fast Fourier Transform (FFT) of the
                given range of data in the given file;
                the name of the column to be used for counting cycles must be
                given, either stated directly after the option or given using
                c=COLUMNNAME, or alternatively, the column number may be given
                using c=COLUMNNUMBER (using zero-based indexing); however, if
                the COLUMNNAME has already been specified for the Cycle option
                (see above), then it cannot be specified again;
                by default, the filing of complete FFT results is suppressed but
                this behavior may be changed by passing "o" as a parameter after
                specifying -f or --fourier, which automatically generates the
                file using the input filename, the COLUMNNUMBER and the BEGINROW
                and ENDROW, or another filename may be given using o=FILENAME.

                NOTE: To calculate FFT, this program uses the popular FFTW
                library by Matteo Frigo and Steven G. Johnson. Reference:
                Matteo Frigo and Steven G. Johnson, “The design and
                implementation of FFTW3,” Proc. IEEE 93 (2), 216–231 (2005).

        -o, --output [FILENAME]
                specify the FILENAME of the output file for calculation results;
                if the option -o or --output is specified but a FILENAME is not
                given, by default, the FILENAME is generated automatically using
                the input filename

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
