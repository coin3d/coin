#!/bin/sh

UPCASEBASE=`basename $1 | sed 's/\.iv$//' | tr [a-z] [A-Z]`

echo "#ifndef SO${UPCASEBASE}_IV_H"
echo "#define SO${UPCASEBASE}_IV_H"
echo
echo "static const char draggergeometry[] = \"#Inventor V2.1 ascii\""
echo '"\\n"'

cat $1 | sed 's/\\/\\\\/g' | sed 's/"/\\"/g' | egrep -v '^[ \t]*#' | sed 's/#.*//g' | sed 's/$/\\n"/' | sed 's/^/"/'

echo '"\\n";'

echo
echo "#endif /* !SO${UPCASEBASE}_IV_H */"
