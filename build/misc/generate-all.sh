#!/bin/sh

cd $(dirname $0)/../..
export COINDIR=$(cygpath -w $(pwd))
cd build/misc
./generate.bat msvc6
./generate.bat msvc7
./generate.bat msvc8
./generate.bat msvc9

#msvc6 is only our starting point, but cannot be built.
#Uncomment to keep the files.
rm -rf msvc6
