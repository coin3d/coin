#!/bin/sh

# Regenerate all files which are constructed by the autoconf, automake
# and libtool tool-chain. Note: only developers should need to use
# this script.

# Author: Morten Eriksen, <mortene@sim.no>. Loosely based on Ralph
# Levien's script for Gnome.

DIE=0

PROJECT=Coin
MACRODIR=conf-macros

echo "Checking the installed configuration tools..."

AUTOCONF_VER=2.14.1  # Autoconf from CVS.
if test -z "`autoconf --version | grep \" $AUTOCONF_VER\" 2> /dev/null`"; then
    echo
    echo "You must have autoconf version $AUTOCONF_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT." 
#    echo "Get ftp://ftp.gnu.org/pub/gnu/autoconf-*.tar.gz"
    echo "Get autoconf from CVS: "
    echo "  cvs -d :pserver:anoncvs@anoncvs.cygnus.com:/cvs/autoconf co autoconf"
    DIE=1
fi

LIBTOOL_VER=1.3.4  # Latest release of libtool
if test -z "`libtool --version | grep \" $LIBTOOL_VER \" 2> /dev/null`"; then
    echo
    echo "You must have libtool version $LIBTOOL_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT." 
    echo "Get ftp://ftp.gnu.org/pub/gnu/libtool-*.tar.gz"
    DIE=1
fi

AUTOMAKE_VER=1.4a  # Automake from CVS.
if test -z "`automake --version | grep \" $AUTOMAKE_VER\" 2> /dev/null`"; then
    echo
    echo "You must have automake version $AUTOMAKE_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT." 
#    echo "Get ftp://ftp.gnu.org/pub/gnu/automake-*.tar.gz"
    echo "Get automake from CVS: "
    echo "  cvs -d :pserver:anoncvs@anoncvs.cygnus.com:/cvs/automake co automake"
    DIE=1
fi

# The separate $MACRODIR module was added late in the project, and
# since we need to do a cvs checkout to obtain it (cvs update won't do
# with modules), we run this check.
if ! test -d $MACRODIR
then
    cvs -z3 checkout -P $MACRODIR
    if ! test -d $MACRODIR
    then
	echo "Couldn't fetch $MACRODIR module!"
        echo
        echo "Directory ``$MACRODIR'' (a separate CVS module) seems to be missing."
        echo "You probably checked out $PROJECT before ``$MACRODIR'' was added."
        echo "Run 'cvs -d :pserver:cvs@cvs.sim.no:/export/cvsroot co $MACRODIR'"
        echo "to try again."
	DIE=1
	exit 1
    fi
fi

if test "$DIE" -eq 1; then
        exit 1
fi


echo "Running aclocal (generating aclocal.m4)..."
aclocal -I $MACRODIR

echo "Running autoheader (generating config.h.in)..."
autoheader

echo "Running automake (generating the Makefile.in files)..."
automake

echo "Running autoconf (generating the configure script)..."
autoconf

echo
echo "Done. Now run './configure' and 'make install' to build $PROJECT."
echo

