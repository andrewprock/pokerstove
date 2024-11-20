PokerStove
==========

Code available at: https://github.com/andrewprock/pokerstove

PokerStove is a highly hand optimized C++ poker hand evaluation library.

The core libraries of pokerstove have been open sourced.  The project is
currently in the process of reviewing and publishing the code.  As code is
reviewed and code sanitized further commits will be added.

Please find the old installer in the win32 directory.  That installer should
install a version of PokerStove which will not expire at any time.  You can also
find the apk file for the Android version of the utility in the android folder.

## Libraries

### peval

This is a c++ poker hand evaluation library.  The main design goals of the library
are generality, extensibility, and ease of use.  There are evaluators for fourteen
variants of poker.  Additionally, there are various card manipulation and query tools built
into the CardSet class.

## Programs

### ps-eval

A tool for poker hand evaluation.  It demonstrates how to use the peval library, and to create
evaluators for the different variants of poker.

### ps-colex

A utility for viewing colexicographical index for sets of cards.

## Building

The pokerstove libraries come with build scripts for cmake.  This
should allow you to build it on any platform with minimal
tweaking.  This project has been successfully build under linux/g++,
windows/vc2010 and OSX/XCode so far.

In order to build the libraries you'll need the following
installed on your platform of choice:

* boost, version 1.46 or higher
* cmake, version 3.14 or higher

### Linux

To install the dependencies with apt get:

    apt-get install libboost-all-dev cmake

To build under linux using cmake, create a build directory,
invoke cmake on the programs directory, then build. The command
below uses four threads, but you may set it according to your
own system.

    git clone https://github.com/andrewprock/pokerstove.git
    cmake -DCMAKE_BUILD_TYPE=Release -S \. -B build
    cmake --build build --target all test -j 4

You should then be able to execute the simple command line
example:

    pokerstove/build $ ./bin/ps-eval
    Allowed options:
      -? [ --help ]          produce help message
      -g [ --game ] arg (=h) game to use for evaluation
      -b [ --board ] arg     community cards for he/o/o8
      -h [ --hand ] arg      a hand for evaluation
      -q [ --quiet ]         produce no output

       For the --game option, one of the follwing games may be
       specified.
         h     hold'em
         o     omaha/8
         O     omaha high
         r     razz
         s     stud
         e     stud/8
         q     stud high/low no qualifier
         d     draw high
         l     lowball (A-5)
         k     Kansas City lowball (2-7)
         t     triple draw lowball (2-7)
         T     triple draw lowball (A-5)
         b     badugi
         3     three-card poker

       examples:
           ps-eval acas
           ps-eval AcAs Kh4d --board 5c8s9h
           ps-eval AcAs Kh4d --board 5c8s9h
           ps-eval --game l 7c5c4c3c2c
           ps-eval --game k 7c5c4c3c2c
           ps-eval --game kansas-city-lowball 7c5c4c3c2c


### Windows

Getting boost to work under windows can be a bit of a challenge.
One of the easier ways is to install precompiled librares.  There
is a batch of them available at sourceforge. If you're working
with Visual Studio 2010, you will probably need the 32 bit
libraries.  [boost precomplied libraries]
(http://sourceforge.net/projects/boost/files/boost-binaries/1.53.0/)

Under windows, the cmake gui can be used to construct solution
and project files for Visual Studio 2010.  To do this, browse
source to locate the programs directory git/pokerstove/programs.
Then create a build dir for the project.  At the bottom of the
gui click Configure, then Generate.  You may have to edit the
git/pokerstove/programs/CMakeLists.txt to point cmake to your
installation of boost.

Once you've done that, you should be able to select

    Menu->Build->Build Solution

to build the sample program.


### OSX

In order to build under Max OSX, you'll need to install XCode,
git, cmake, macports, and boost.  The first four can be installed
in the conventional manner, with XCode coming form the App Store, cmake,
git and macports downloaded from the web.  The macports package is a
typical unix package management utility and is required to install boost.
Once you've installed and selfupdate'd macports, you can install boost:

    sudo port install boost -no_static

Or

    brew install boost cmake

From there you can run the cmake gui as in windows.  This will create
an XCode project which should compile the sample utility. Alternatively,
follow the command  line `cmake` instructions in the [Linux section](#linux).

# Python support

Python support is done via swig integration. This has only been developed
and tested for Ubuntu at this point in time.

In order for python libraries to work, you will need to install:

    sudo apt install python3 swig

If you would like to also build the Python library as well, append the
directive when the first cmake invocation is run:

    cmake -DCMAKE_BUILD_TYPE=Release -S \. -B build -DBUILD_PYTHON=ON
    cmake --build build --target all test -j 16

Once you have built the project with Python support there will be a
python loader file and a shared object file. To test run the script
from the build directory:

    PYTHONPATH=build/python/pokerstove/ ./src/lib/python/test-python

For regular use you'll want to export the PYTHONPATH variable to your
shell:

    export PYTHONPATH=~/git/pokerstove/build/python/pokerstove/

## Python wheel package

You will need to install pipx to build with scikit-build-core, using
apt on Ubuntu and brew on MacOS.

There is also a `pyproject.toml` file which can be used to create an
installable wheel for the pythong package. The commands below can be
used to build/install/verify the package.

    git clean -fxd && pipx run build
    python3 -m venv venv && . venv/bin/activate
    pip install dist/pokerstove-*.whl
    python src/lib/python/test-python
    deactivate

# Breaking changes

Version 1.2 will migrate from boost::shared_ptr to std::shared_ptr, breaking API compatibility.
