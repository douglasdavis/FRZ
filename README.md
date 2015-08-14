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
be accessed with `$FRZ_BASE`. The library is in the `$FRZ_BASE/lib`
directory. Added to the `PATH` variable: `$FRZ_BASE`, `$FRZ_BASE/plotting`
and added to the `LD_LIBRARY_PATH` variable: `$FRZ_BASE/lib`

What is going on here?
------------
The goal of this project is to determine the probability that a
jet of a certain pT will be reconstructed as a lepton. The `FRZ`
framework works in a number of steps. First, a top group mini ntuple
must be "swizzled." This generates a ROOT file containing only events
that we want to deal with, in a nice EDM format. A sample swizzling:

    $ FRZ.py --swizzle --pileup --in-file [infile] 

Second, a group of "swizzled" files must be put in a directory
called `swizzled_files` inside the `FRZ_BASE` directory. Then
we can generate histograms out of those files. The `HistMaker` class
sorts the events that it reads from the `$FRZ_BASE/swizzled_files`
directory into the proper process type histograms. A sample
histogram generation step:

    $ FRZ.py --make-hists --third-pdg 11 --third-loose 1

Now we have a `.root` file in our directory containing histograms.
To generate nice looking plots from these histograms we use `--gen-plots`:

    $ FRZ.py --gen-plots [histogram file name]

There is an optional flag `--mpl` to generate the plots using matplotlib
instead of ROOT. To extract the ratios for third lepton pT bins:

    $ FRZ.py --third-ratios [histogram file name]

this prints a table in LaTeX format showing useful information about those
pT bins. To see all options just use the `--help` flag.

Instructions for generating MC sample ROOT file
------------
The AIDA code reads MC sample information from a json file in python.
This is pretty simple because python is great for json, I wanted to
be able to read in C++ and my work around (in order to not use an
external C++ json library) was to store the info in a C++ class in a 
ROOT file to hold all of the sample information. It is very simple
to generate a ROOT file holding this class; after building, run the
`FRZ.py` steering python script with the flag `--json-to-root`