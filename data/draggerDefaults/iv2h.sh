#!/bin/sh

UPCASEBASE=`basename $1 | sed 's/\.iv$//' | tr [a-z] [A-Z]`

echo "#ifndef SO${UPCASEBASE}_IV_H"
echo "#define SO${UPCASEBASE}_IV_H"
echo
echo "static const char ${UPCASEBASE}_draggergeometry[] = \"#Inventor V2.1 ascii\""

# Make sure there's at least two linefeeds after the header string,
# to be compliant with the fileformat spec. (The strange syntax is a
# workaround for ``echo'' not behaving as expected on SGI IRIX with
# ``echo \"\\n\"'').
echo | sed 's/$/"\\n\\n"/'

cat $1 | sed 's/\\/\\\\/g' | sed 's/"/\\"/g' | egrep -v '^[ \t]*#' | sed 's/#.*//g' | sed 's/$/\\n"/' | sed 's/^/"/'

echo ';'

echo
echo "#endif /* !SO${UPCASEBASE}_IV_H */"
