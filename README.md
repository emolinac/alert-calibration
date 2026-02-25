# alert-calibration
Simple macros to extract the ToT calibration factors from RG-L runs.

## Before starting
Some details before starting:
- The output directories specified in each macros should not be changed. 
- The input directory in `macro_ntuple_elastics.cpp` should be modified according to where the initial input file is located. 

Regarding the base input files, it should be used those obtained [using this version of hipo converter](https://github.com/emolinac/hipo_alert).

## Events
This package is designed to work with elastic events. The elastic events are selected by:
- Choosing a low energy run (2-3 GeV or lower)
- Applying the kinematic cuts specified in `macro_ntuple_elastics.cpp`  
        - $W < 1\:GeV$  
        - $|\Delta\phi(\text{electron, particle in KF bank})|>170°$

Additional cuts can be seen in `macro_ntuple_elastics.cpp`

## How to use?
1. Run `root -b -q macro_ntuple_elastics.cpp` to process the input file from the hipo converter.
2. Run `root -b -q macro_create_wire_tot_histos.cpp` to fit all the wires in the respective layers, and to obtain a TNtuple with the relevant parameters for the calibration table.
3. Run `root -b -q -l macro_create_table.cpp > table.txt` to get the table with the columns and rows formatted for the CCDB.

## Misc
There are other printing macros to check on how the variables look. They need the TNtuple from `macro_ntuple_elastics.cpp` to work.