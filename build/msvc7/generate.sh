#! /bin/sh
#
# This script generates the Visual Studio 6 build files for Windows.
#
# 20041214 larsa

rm -rf coin3.dsp coin3.dsw src

../../configure --enable-msvcdsp --with-msvcrt=mt
make

build_pwd=`pwd`
build="`cygpath -w $build_pwd | sed -e 's/\\\\/\\\\\\\\/g'`"
source_pwd=`cd ../..; pwd`
source="`cygpath -w $source_pwd | sed -e 's/\\\\/\\\\\\\\/g'`"

cp coin3.dsp orig.dsp

sed \
  -e "s/$build/./g" \
  -e "s/$source/..\\\\../g" \
  -e 's/$/\r/g' \
  <coin3.dsp >new.dsp
mv new.dsp coin3.dsp

# How can I avoid the modal upgrade prompt-dialog for MSVC7.1 here???
# devenv /command "File.OpenProject $build\\coin3.dsp"

