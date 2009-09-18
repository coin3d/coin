#!/bin/sh

cd $(dirname $0)/../..
export COINDIR=$(cygpath -w $(pwd))
cd build/misc
./generate.bat msvc6
./generate.bat msvc7
./generate.bat msvc8
./generate.bat msvc9

