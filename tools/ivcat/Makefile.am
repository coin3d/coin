## Process this file with automake to generate Makefile.in.

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include
bin_PROGRAMS = ivcat
ivcat_SOURCES = ivcat.cpp
ivcat_LDADD = $(top_builddir)/src/libCoin.la @COIN_HACKING_LIBDIRS@ @COIN_EXTRA_LIBS@
