## Process this file with automake to generate Makefile.in.

SUBDIRS = include data src build

bin_SCRIPTS = coin-config
BUILT_SOURCES = coin-config
coin-config: coin-config.in

EXTRA_DIST = cfg/m4/coin.m4

m4datadir = $(datadir)/aclocal
m4data_DATA = cfg/m4/coin.m4

doxygen-doc:
	@@sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen

coin-installer.exe: build/coin.nsi
	@sed 's,$$,
,' <$(srcdir)/COPYING >COPYING.DOS; \
	makensis build\\coin.nsi; \
	rm COPYING.DOS

