PokerStove
==========

The core libraries of pokerstove are being open sourced.  The project
is currently in the process of reviewing the code and evaluating
licences.  Once a license has been settled on, and the code sanitized
the first commits can start.  Until then, please find the old
installer in the win32 directory.

## Libraries

### peval

This is a basic c++ hand evaluation library with the main goal being generality,
extensibility, and ease of use.  There are evaluators for fourteen variants of
poker.

## Programs

### eval

A basic evaluation tool which demonstrates how to use the peval library.

## Building

The pokerstove libraries come with build scripts for cmake.  This
should allow you to build it on any platform with minimal
tweaking.  I've only built this project under linux/g++ and
windows/vc2010 so far.

In order to build the libraries you'll need the following
installed on your platform of choice:

* boost, version 1.46 or higher
* cmake, version 2.4 or higher

### Linux

To build under linux using cmake, create a build directory,
invoke cmake on the programs directory, then build.

    mkdir -p cmake/programs
    cd cmake/programs
    cmake <path-to-programs-dir>
    make

You should then be able to execute the simple command line
example:

    ~/cmake/programs$ ./pseval/pseval
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
           pseval acas
           pseval AcAs Kh4d --board 5c8s9h
           pseval AcAs Kh4d --board 5c8s9h
           pseval --game l 7c5c4c3c2c
           pseval --game k 7c5c4c3c2c
           pseval --game kansas-city-lowball 7c5c4c3c2c

    
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


[![Bitdeli Badge]
(https://d2weczhvl823v0.cloudfront.net/andrewprock/pokerstove/trend.png)]
(https://bitdeli.com/free "Bitdeli Badge")
