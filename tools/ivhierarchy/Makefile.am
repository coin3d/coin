## Process this file with automake to generate Makefile.in.

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include
bin_PROGRAMS = ivhierarchy
ivhierarchy_SOURCES = ivhierarchy.cpp
ivhierarchy_LDADD = $(top_builddir)/src/libCoin.la @COIN_HACKING_LIBDIRS@ @COIN_EXTRA_LIBS@
