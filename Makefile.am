## Process this file with automake to generate Makefile.in.

SUBDIRS = include data src build

bin_SCRIPTS = coin-config
BUILT_SOURCES = coin-config
coin-config: coin-config.in

EXTRA_DIST = conf-macros/coin.m4

m4datadir = $(datadir)/aclocal
m4data_DATA = conf-macros/coin.m4

doxygen-doc:
	@@sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen

