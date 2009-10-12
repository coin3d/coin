#!/bin/sh

SolutionName=coin4

cd $(dirname $0)/../..
export COINDIR=$(cygpath -w $(pwd))
cd build/misc
./generate.bat msvc6 ${SolutionName}
./generate.bat msvc7 ${SolutionName}
./generate.bat msvc8 ${SolutionName}
./generate.bat msvc9 ${SolutionName}

#msvc6 is only our starting point, but cannot be built.
#Uncomment to keep the files.
rm -rf msvc6
