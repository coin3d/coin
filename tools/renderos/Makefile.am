## Process this file with automake to generate Makefile.in.

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include
bin_PROGRAMS = renderos
renderos_SOURCES = renderos.cpp
renderos_LDADD = $(top_builddir)/src/libCoin.la @COIN_HACKING_LIBDIRS@ @COIN_EXTRA_LIBS@
