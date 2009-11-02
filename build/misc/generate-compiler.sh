#!/bin/sh

upgrade() {
  echo Upgrading from $1 to $2
  devenv /upgrade $3.sln
  rm -rf Backup UpgradeLog.XML _UpgradeReport_Files *.user *.old *.dsw *.suo *.dsp
}

error() {
  echo $@ >/dev/stderr
}

case $1
in 
  msvc*)
  compilerversion=$(echo $1 | cut -c5-)
  ;;
 *)
  error $1 is not a valid compiler
  exit 1
  ;;
esac

if [ ${compilerversion} -lt 6 ] || [ ${compilerversion} -gt 10 ] 
then
  error "ERROR msvc${compilerversion} is not supported yet"
  exit 1
fi

cd $(dirname $0)/..
rm -rf $1
mkdir $1
cd $1
../misc/generate.sh
if [ ${compilerversion} -gt 6 ]
then
  for file in *.dsp
  do
    fName=$(basename ${file} .dsp)
    cscript.exe ../misc/Convert.js $(cygpath -w $(pwd)/${file}) $(cygpath -w $(pwd)/${fName}.vcproj)
    rm ${file}
  done
  cp ../data/$2.sln .
fi
if [ ${compilerversion} -gt 7 ]
then
  upgrade msvc7 $1 $2
fi
