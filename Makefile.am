## Process this file with automake to generate Makefile.in.

SUBDIRS = include data src build

dist_bin_SCRIPTS = \
	coin-config \
	cfg/m4/msvccc

m4datadir = $(datadir)/aclocal
dist_m4data_DATA = cfg/m4/coin.m4

cfgdatadir = $(datadir)/Coin/conf
dist_cfgdata_DATA = coin-@CONFIG@.cfg

noinst_HEADERS = \
	coindefs.h

coin-@CONFIG@.cfg: config.status $(srcdir)/coin.cfg.in
	@$(SHELL) config.status --file=$@:coin.cfg.in

EXTRA_DIST = \
	ChangeLog.1999 \
	ChangeLog.2000 \
	LICENSE.GPL \
	LICENSE.LGPL \
	README.AIX \
	README.IRIX \
	README.WIN32 \
	cfg/depcomp \
	cfg/ltconfig \
	cfg/install-header \
	docs/coin.doxygen.in

doxygen-doc:
	@@sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen

