## Process this file with automake to generate Makefile.in.

SUBDIRS = include data src build

bin_SCRIPTS = coin-config

EXTRA_DIST = cfg/m4/coin.m4

m4datadir = $(datadir)/aclocal
m4data_DATA = cfg/m4/coin.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = coin-@CONFIG@.cfg

coin-@CONFIG@.cfg: $(top_srcdir)/coin.cfg.in
	@./config.status --file=$@:coin.cfg.in

doxygen-doc:
	@@sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen

