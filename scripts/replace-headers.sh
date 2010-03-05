#!/bin/sh
#
# ./replace-headers.sh
#
# The purpose of this script is to update old header information from e.g.
# cpp/h files in Coin. The script indicates that it is working by showing
# a . for each file. When the script is done it will show the amount of files
# updated.
#
# Usage: Simply run the script from the directory you wish to update. It will
#        do recursive scanning.
#
# Variables:
#   DEBUG      - Setting this to "true" displays the change without updating,
#                and "false" to do the changes in the files.
#   STARTSWITH - The start pattern(regex) to look for.
#   ENDSWITH   - The pattern(regex) to look for after the start pattern.
#   HEADER     - The new header information to be used.
#   IGNORE     - The file path pattern(regex) to ignore.
#
# Note: This script does not take backup of changed files. Use with caution!
#

DEBUG=true
HEADER="/**************************************************************************\\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  (\"GPL\") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/"
STARTSWITH="^/\*\*\*"
ENDSWITH="^\\\*\*\*"
IGNORE="(/boost/|/\.hg/)"
TEMP=".replace-headers.tmp"
COUNTER=0

echo "Updating headers... (This may take several minutes)"

for file in $(find . -type f)
do
  # indicate work in progress...
  echo -n .

  # skip hidden and binary files
  [[ "$file" =~ "/\." ]] && continue
  [[ "$file" =~ "$IGNORE" ]] && continue
  [[ "$(file $file)" =~ ": .*text" ]] || continue

  # fetch the header line numbers
  LINES=$(wc -l $file | sed 's/ .*//')
  FROM=$(grep -n "$STARTSWITH" $file | head -n 1 | sed 's/:.*//')
  [ "$FROM" = "" ] && continue
  LENGTH=$(cat $file | tail -n $(($LINES-$FROM)) | grep -n "$ENDSWITH" | head -n 1 | sed 's/:.*//')
  [ "$LENGTH" = "" ] && continue

  if ($DEBUG)
  then
    #debug
    clear
    echo "Showing \"$file\" from line $FROM-$LENGTH:"
    cat $file | head -n $(($FROM+$LENGTH)) | tail -n $(($LENGTH+1))
    #echo
    #echo "Hit ENTER to continue."
    #read
  else
    # update file
    cat $file | head -n $(($FROM-1)) > $TEMP
    echo "$HEADER" >> $TEMP
    cat $file | tail -n $(($LINES-$FROM-$LENGTH)) >> $TEMP
    mv $TEMP $file
  fi

  # increase amount of updated files
  COUNTER=$(($COUNTER+1))
done

echo
echo "Done! $COUNTER file(s) updated."
