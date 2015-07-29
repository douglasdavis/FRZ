FRZ - Fake Rate using Z+jets
============

Author: Douglas Davis < ddavis@phy.duke.edu > < douglas.davis@cern.ch >

with _heavy_ inspiration from the AIDA group AIDA-analysis code:

https://git.cern.ch/web/AIDA-analysis.git

https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/AIDACodeSetup

Instructions for building
------------
In the top level directory execute the following:

    $ source config/setup
    $ make

Now the environment variable `FRZ_BASE` is defined and can
be accessed with `$FRZ_BASE`. The library is in the $FRZ_BASE/lib
directory. Added to the `PATH` variable: `$FRZ_BASE/python`,
and added to the `LD_LIBRARY_PATH` variable: `$FRZ_BASE/lib`


Instructions for generating MC sample ROOT file
------------
The AIDA code reads MC sample information from a json file in python.
This is pretty simple because python is great for json, I wanted to
be able to read in C++ and my work around (in order to not use an
external C++ json library) was to store the info in a C++ class in a 
ROOT file to hold all of the sample information. It is very simple
to generate a ROOT file holding this class; after building, run the
`FRZ.py` steering python script with the flag `--json-to-root`