#!/bin/sh

UPCASEBASE=`basename $1 | sed 's/\.iv$//' | tr [a-z] [A-Z]`

echo "#ifndef _SO${UPCASEBASE}_IV_H_"
echo "#define _SO${UPCASEBASE}_IV_H_"
echo

echo "static const char draggergeometry[] = \"#Inventor V2.1 ascii"

cat $1 | sed 's/\\/\\\\/g' | sed 's/"/\\"/g' | egrep -v '^[ \t]*#' | sed 's/#.*//g'

echo "\";"

echo
echo "#endif /* !_SO${UPCASEBASE}_IV_H_*/"
