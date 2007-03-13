#! /bin/sh
#
# This manages the generation of the Visual Studio build files for Windows.
#
# 20041214 larsa

proper=true;

rm -f coin3.dsp coin3.dsw coin3.vcproj coin3.sln install-headers.bat;
if $proper; then
  rm -rf src;
fi

build_pwd=`pwd`;
build="`cygpath -w $build_pwd | sed -e 's/\\\\/\\\\\\\\/g'`";
build_pwd="`pwd | sed -e 's/\\//\\\\\\\\/g'`\\\\";

source_pwd=`cd ../..; pwd`;
source="`cygpath -w $source_pwd | sed -e 's/\\\\/\\\\\\\\/g'`";
source_pwd="`(cd ../..; pwd) | sed -e 's/\\//\\\\\\\\/g'`";

if $proper; then
  ../../configure --with-msvcrt=mtd --with-suffix=d \
    --enable-3ds-import \
    --enable-debug --enable-symbols || exit 1;
  mv include/config.h include/config-debug.h;

  ../../configure --enable-msvcdsp --with-msvcrt=mt \
    --enable-3ds-import \
    --disable-debug --disable-symbols --enable-optimization || exit 1;
  mv include/config.h include/config-release.h;

  cp ../misc/config-wrapper.h include/config.h;
else
  ./config.status --file=cfg/gendsp.pl;
fi

make || exit 1

if $proper; then
  sed \
    -e "s/$build/./g" \
    -e "s/$build_pwd//g" \
    -e "s/$source/..\\\\../g" \
    -e "s/$source_pwd/..\\\\../g" \
    -e 's/$/\r/g' \
    <coin3.dsp >new.dsp

  mv new.dsp coin3.dsp

  sed \
    -e "s/$build/./g" \
    -e "s/$build_pwd//g" \
    -e "s/$source/..\\\\../g" \
    -e "s/$source_pwd/..\\\\../g" \
    -e 's/$/\r/g' \
    <install-headers.bat >new.bat

  mv new.bat install-headers.bat
fi

echo "Done."
echo "Make sure the coin3.dsp file does not contain any absolute paths."

grep -i "\\(c:\\|@\\)" coin3.dsp /dev/null
grep -i "cygdrive" coin3.dsp /dev/null
sort install-headers.bat | uniq -c | grep -v " 1 "
grep "SOURCE=" coin3.dsp | sort | uniq -c | grep -v " 1 "

echo "Now upgrade the project files with devenv, if necessary."

