## Process this file with automake to generate Makefile.in.

SUBDIRS = . include data src build

dist_bin_SCRIPTS = \
	coin-config

BUILT_SOURCES = \
	include/Inventor/SbBasic.h \
	include/Inventor/system/inttypes.h \
	coin-@CONFIG@.cfg

DISTCLEANFILES = \
	coin-@CONFIG@.cfg

m4datadir = $(datadir)/aclocal
dist_m4data_DATA = cfg/m4/coin.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = coin-@CONFIG@.cfg

EXTRA_DIST = \
	ChangeLog.1999 \
	ChangeLog.2000 \
	LICENSE.GPL \
	LICENSE.LGPL \
	README.AIX \
	README.IRIX \
	README.WIN32 \
	coin.cfg.in \
	docs/coin.doxygen.in \
	cfg/doxy4win.pl \
	cfg/ltconfig \
	cfg/m4/wrapmsvc.cpp \
	cfg/m4/wrapmsvc.exe

coin-@CONFIG@.cfg: config.status $(srcdir)/coin.cfg.in
	@$(SHELL) config.status --file=$@:coin.cfg.in

# sources needed to generate docs must be created by this rule
built-sources: $(BUILT_SOURCES)

include/Inventor/SbBasic.h: $(srcdir)/include/Inventor/SbBasic.h.in
	@./config.status --header=$@

include/Inventor/system/inttypes.h: $(srcdir)/include/Inventor/system/inttypes.h.in
	@./config.status --header=$@

doxygen-doc:
	@@sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen

doxygen-docs:
	@@sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen

install-exec-local: $(srcdir)/cfg/m4/wrapmsvc.exe
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@if @BUILD_WITH_MSVC@; then \
	  p=wrapmsvc.exe; \
	  echo " $(INSTALL_PROGRAM) $(srcdir)/cfg/m4/$$p $(DESTDIR)$(bindir)/$$p"; \
	  $(INSTALL_PROGRAM) $(srcdir)/cfg/m4/$$p $(DESTDIR)$(bindir)/$$p; \
	fi

# EOF ######################################################################
