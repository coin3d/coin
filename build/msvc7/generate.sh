#! /bin/sh
#
# This script generates the Visual Studio 6 build files for Windows, which
# should then be upgraded to Visual Studio 7 build files through a manual
# process.
#
# 20041214 larsa

rm -rf src
rm -f coin3.dsp coin3.dsw coin3.vcproj coin3.sln installcoinheaders.bat

build_pwd=`pwd`
build="`cygpath -w $build_pwd | sed -e 's/\\\\/\\\\\\\\/g'`"
build_pwd="`pwd | sed -e 's/\\//\\\\\\\\/g'`\\\\"

source_pwd=`cd ../..; pwd`
source="`cygpath -w $source_pwd | sed -e 's/\\\\/\\\\\\\\/g'`"
source_pwd="`(cd ../..; pwd) | sed -e 's/\\//\\\\\\\\/g'`"

../../configure --with-msvcrt=mtd --with-suffix=d \
  --enable-debug --enable-symbols || exit 1
cp include/config.h include/config-debug.h

../../configure --with-msvcrt=mt \
  --disable-debug --disable-symbols --enable-optimization || exit 1
cp include/config.h include/config-release.h

cp config-wrapper.h include/config.h

rm -f generate.log
( make 2>&1 | tee generate.log ) || exit 1

# sed \
#   -e "s/$build/./g" \
#   -e "s/$build_pwd//g" \
#   -e "s/$source/..\\\\../g" \
#   -e "s/$source_pwd/..\\\\../g" \
#   -e 's/$/\r/g' \
#   <coin3.dsp >new.dsp
# 
# mv new.dsp coin3.dsp
# 
# sed \
#   -e "s/$build/./g" \
#   -e "s/$build_pwd//g" \
#   -e "s/$source/..\\\\../g" \
#   -e "s/$source_pwd/..\\\\../g" \
#   -e 's/$/\r/g' \
#   <installcoinheaders.bat >new.bat
# 
# mv new.bat installcoinheaders.bat

cp ../msvc6/coin3.dsw ../msvc6/coin3.dsp ../msvc6/install-headers.bat .

echo "Now upgrade the project files to MSVC7.1..."
echo "Hopefully you remembered to generate in msvc6 first."
echo "Run 'devenv coin3.dsw' and save all solution files."
echo "Make sure the resulting .vcproj file has no absolute paths."
grep -i c: coin3.dsp /dev/null

# How can I avoid the modal upgrade prompt-dialog for MSVC7.1 here???
# devenv /command "File.OpenProject $build\\coin3.dsp"

