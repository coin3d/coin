## Process this file with automake to generate Makefile.in.

SUBDIRS = include data src build

bin_SCRIPTS = coin-config

EXTRA_DIST = cfg/m4/coin.m4

m4datadir = $(datadir)/aclocal
m4data_DATA = cfg/m4/coin.m4

doxygen-doc:
	@@sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen

install-exec-hook: coin.cfg
	@target=coin.cfg; \
	@COIN_DEBUG_BUILD@ && target=coin-debug.cfg; \
	echo "Installing $$target"; \
	$(mkinstalldirs) $(DESTDIR)$(datadir)/Coin/conf && \
	  $(INSTALL_DATA) coin.cfg $(DESTDIR)$(datadir)/Coin/conf/$$target;

