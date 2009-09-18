#!/bin/sh

upgrade() {
  echo Upgrading from $1 to $2
  rm -rf $2
  cp -r $1 $2
  cd $2
  devenv /upgrade coin4.sln
  rm -rf Backup UpgradeLog.XML _UpgradeReport_Files *.user *.old *.dsw *.suo *.dsp
}

cd $(dirname $0)/..
if [ "$1" == "msvc6" ]
then
  rm -rf msvc6
  mkdir $1
  cd $1
  ../misc/generate.sh
elif [ "$1" == "msvc7" ]
then
  rm -rf msvc7
  cp -r msvc6 msvc7
  cd msvc7
  for file in *.dsp
  do
    fName=$(basename ${file} .dsp)
    cscript.exe ../misc/Convert.js $(cygpath -w $(pwd)/${file}) $(cygpath -w $(pwd)/${fName}.vcproj)
    rm ${file}
  done
  cp ../misc/coin4.sln .
elif [ "$1" == "msvc8" ]
then
  upgrade msvc7 msvc8
elif [ "$1" == "msvc9" ]
then
  upgrade msvc8 msvc9
else
  echo ERROR $1 is not supported yet
fi
