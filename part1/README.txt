Build instructions for Linux
============================

Dependencies
------------
gcc/g++, CMake 2.6 (or higher), make, OpenGL, GLEW

These dependencies can be installed via the package manager of your
Linux distribution.

GLFW will be built on-the-fly when you build the assignment programs.

When CMake tries to compile GLFW it might complain if two additional
libraries, Xi and Xmu, are not installed. On Ubuntu 12.04 you can
install these two libraries by typing

 sudo apt-get install libxmu-dev libxi-dev

If GLEW triggers linking errors, install the binutils-gold package.

Environment variables
---------------------
The environment variable ASSIGNMENT3_ROOT must be set pointing to the
extracted assignment3 directory. Example:

  export ASSIGNMENT3_ROOT=$HOME/assignment3

Building and running the program for Part 1
-------------------------------------------
To build the program, open a terminal, navigate to
$ASSIGNMENT3_ROOT/part1/build, and type

  mkdir linux
  cd linux
  cmake ../
  make

To run the program, navigate to the resulting executable (part1), and
type

  ./part1

Alternatively, run the attached script build.sh (which will perform
all these steps for you):

  ./build.sh

Tip: You don't have to run CMake every time you change something in
the source files. Just use the generated makefile (or the
build.sh script) to rebuild the program.


Build instructions for OS X (experimental)
==========================================

Should be similar as for Linux. You can install and use CMake to
generate Unix makefiles or XCode solutions. GLFW will be built
on-the-fly when you build the assignment programs.

If you cannot get CMake working, try to set up an XCode project
manually. The part1.cpp and part2.cpp files shows you which files and
dependencies you need.
