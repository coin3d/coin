## Process this file with automake to generate Makefile.in.

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include
noinst_PROGRAMS = lstextures
lstextures_SOURCES = lstextures.cpp texturefilenameelement.cpp
lstextures_LDADD = $(top_builddir)/src/libCoin.la @COIN_HACKING_LIBDIRS@ @COIN_EXTRA_LIBS@
