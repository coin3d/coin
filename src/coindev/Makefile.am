## Process this file with automake to generate Makefile.in.

lib_LTLIBRARIES = libcoindevLINKHACK.la

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

libcoindevLINKHACK_la_SOURCES = coindev.cpp
