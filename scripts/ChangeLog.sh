#!/bin/sh
############################################################################

module=Coin
cvs2cl=/home/sigma/larsa/store/cvs/cvs2cl/cvs2cl.pl
moduledir=$HOME/code/$module
headerfile=/tmp/$module.header

cd $moduledir

############################################################################
cat > $headerfile <<ENDOFHEADER
ChangeLog
=========

This is the ChangeLog file for the $module module.  This file is automatically
generated every night.  Entries are in reversed chronological order.  Check
also out the following ChangeLog file:

  ./conf-macros/ChangeLog

See http://www.red-bean.com/~kfogel/cvs2cl.shtml for information about the
cvs2cl script used to generate this file.

============================================================================

ENDOFHEADER
############################################################################

cvs update -dP

cvs log -d">= 1 January 2000" | $cvs2cl --stdin --header $headerfile --separate-header --prune \
  --ignore 'ChangeLog$' \
  --ignore '(Makefile\.in|configure|aclocal\.m4|config\.sub|config\.guess)$' \
  --ignore '(ltconfig|ltmain\.sh|missing|mkinstalldirs|stamp-h.*|install-sh)$' \
  --ignore 'config\.h\.in$' \
  --ignore 'conf-macros/'

rm ChangeLog.bak $headerfile

cvs commit -m "Automatic ChangeLog generation" ChangeLog

