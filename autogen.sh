#!/bin/sh

# Author: Morten Eriksen, <mortene@sim.no>. Loosely based on Ralph
# Levien's script for Gnome.

DIE=0

PROJECT=Coin

# FIXME: check for minimum version number? 19990822 mortene.
(autoconf --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have autoconf installed to generate"
	echo "configure information and Makefiles for $PROJECT."
        echo "Get ftp://ftp.gnu.org/pub/gnu/autoconf-*.tar.gz"
        DIE=1
}

# FIXME: check for minimum version number? 19990822 mortene.
(libtool --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have libtool installed to generate"
	echo "configure information and Makefiles for $PROJECT."
        echo "Get ftp://ftp.gnu.org/pub/gnu/libtool-*.tar.gz"
        DIE=1
}

# FIXME: check for minimum version number? 19990822 mortene.
(automake --version) < /dev/null > /dev/null 2>&1 || {
        echo
        echo "You must have automake installed to generate"
	echo "configure information and Makefiles for $PROJECT."
        echo "Get ftp://ftp.gnu.org/pub/gnu/automake-*.tar.gz"
        DIE=1
}

# FIXME: check for more tools? 19990822 mortene.

if test "$DIE" -eq 1; then
        exit 1
fi

echo "Running aclocal"
aclocal

echo "Running autoheader"
autoheader

echo
echo "Running automake..."
echo "(NB: if you're compiling without g++, you probably need to"
echo "run automake with the --include-deps argument to avoid setting"
echo "up dependency tracking. 19991006 mortene.)"
automake

echo "Running autoconf"
autoconf

echo "Now type './configure' and 'make' to compile $PROJECT."
