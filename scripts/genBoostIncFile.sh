#!/bin/env bash
#Run this script ro regenerate the Boost include file, when updating the boost headerlibs.


DELIM=' \
	'

cd $(dirname $0)/..
(
echo -ne "BOOST_HEADER_FILES = \\
\t"
find include/boost -name '*.hpp' | xargs | cut -d\  -f1- --output-delimiter="${DELIM}") > include/Makefile.inc
echo '<COIN_SOURCEDIR>/include/Makefile.inc' generated
