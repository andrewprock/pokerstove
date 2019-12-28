PokerStove
==========

[![OpenHUB Metrics](https://www.openhub.net/p/pokerstove/widgets/project_thin_badge.gif)](https://www.openhub.net/p/pokerstove)

PokerStove is a highly hand optimized C++ poker hand evaluation library.  The Win32 Hold'em
GUI was first released in 2002, and has been available as freeware since
it's first release.

The core libraries of pokerstove are being open sourced.  The project is
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
* cmake, version 2.4 or higher

### Linux

To install the dependencies with apt get:

    apt-get install libboost-all-dev cmake

To build under linux using cmake, create a build directory,
invoke cmake on the programs directory, then build.

    git clone https://github.com/andrewprock/pokerstove.git
    mkdir pokerstove/build
    cd pokerstove/build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make && make test

You should then be able to execute the simple command line
example:

    ~/cmake/programs$ ./programs/ps-eval/ps-eval
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

pokerstove project includes Visual Studio 2019 project files located in
"VisualStudio" sub folder.\
VS Project files are adjusted to link against required `boost` and `gtest`
3rd party libraries, and project inter dependencies are all set.

For getting boost and gtest libraries the easiest way on Windows is to use [vcpkg](https://github.com/microsoft/vcpkg) package manager.

After reading vcpkg README file on the link above and perfoming easy initial setup all you need to do is first
figure out the name of packages by typing:\
`.\vcpkg search gtest` and\
`.\vcpkg search boost`

to install them simply type following by replacing package name and it's version number for example:\
`.\vcpkg install gtest` and\
`.\vcpkg install boost-1.72`

That's it, Visual Studio will autmatically pick up your installed boost and gtest installed by vcpkg!

However if you already have boost or gtest installed you need to adjust project settings in visual studio, To do so open Visual Studio solution and adjust following properties in property manager:

`Solution Setup -> C/C++ -> General -> Additional include directories`: adjust path to your boost root directory.\
`Solution Setup -> Linker -> Additional library directoris`: adjust path to your boost libs.\
`Solution Setup -> C/C++ -> Preprocessor`: *BOOST_ALL_DYN_LINK* is defied to link against dynamic boost libs,
remove this macro if you want to link against static boost libraries.

Once all this is set up, simply open VS solution, select your configuration (x64/x86, Debug/Release) and hit "Build solution",
found in VS menu:\
`Build -> Build Solution`

Otherwise if you use an older version of Visual Studio you can try to replicate existing VS project files
manually by hand, then the rest of the process is the same as described above.


### OSX

In order to build under Max OSX, you'll need to install XCode,
git, cmake, macports, and boost.  The first four can be installed
in the conventional manner, with XCode coming form the App Store, cmake,
git and macports downloaded from the web.  The macports package is a
typical unix package management utility and is required to install boost.
Once you've installed and selfupdate'd macports, you can install boost:

    sudo port install boost -no_static

From there you can run the cmake gui as in windows.  This will create
an XCode project which should compile the sample utility. Alternatively,
follow the command  line `cmake` instructions in the [Linux section](#linux).
