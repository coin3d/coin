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

# **************************************************************************

coin$(COIN_MAJOR_VERSION)_$(VERSION): distdir
	cp -p -R Coin-$(VERSION) coin$(COIN_MAJOR_VERSION)_$(VERSION)

coin$(COIN_MAJOR_VERSION)-lib_$(VERSION): distdir
	cp -p -R Coin-$(VERSION) coin$(COIN_MAJOR_VERSION)-lib_$(VERSION)

coin$(COIN_MAJOR_VERSION)-dev_$(VERSION): distdir
	cp -p -R Coin-$(VERSION) coin$(COIN_MAJOR_VERSION)-dev_$(VERSION)

coin$(COIN_MAJOR_VERSION)-doc_$(VERSION): distdir
	cp -p -R Coin-$(VERSION) coin$(COIN_MAJOR_VERSION)-doc_$(VERSION)

debian-control:
	test -d $(DEBIAN_DIST_DIR)/debian || mkdir $(DEBIAN_DIST_DIR)/debian;
	for f in packaging/debian/*.in; do \
	  file=`echo $$f | sed 's/.*\///g; s/\.in$$//g'`; \
	  ./config.status --file=$(DEBIAN_DIST_DIR)/debian/$$file:$$f; \
	done;
	for f in rules preinst prerm postinst postrm; do \
	  if test -f $(DEBIAN_DIST_DIR)/debian/$$f; then \
	    echo " chmod +x $(DEBIAN_DIST_DIR)/debian/$$f"; \
	    chmod +x $(DEBIAN_DIST_DIR)/debian/$$f; \
	  fi; \
	done;

debian-src-control: coin$(COIN_MAJOR_VERSION)_$(VERSION)
	distdir=coin$(COIN_MAJOR_VERSION)_$(VERSION); \
	$(MAKE) DEBIAN_DIST_DIR=$$distdir debian-control; \
	rm -f $distdir/debian/postinst;

debian-lib-control: coin$(COIN_MAJOR_VERSION)_$(VERSION)
	distdir=coin$(COIN_MAJOR_VERSION)-lib_$(VERSION); \
	$(MAKE) DEBIAN_DIST_DIR=$$distdir debian-control; \
	rm -f $distdir/debian/postinst;

debian-dev-control: coin$(COIN_MAJOR_VERSION)-dev_$(VERSION)
	distdir=coin$(COIN_MAJOR_VERSION)-dev_$(VERSION); \
	$(MAKE) DEBIAN_DIST_DIR=$$distdir debian-control;

debian-doc-control: coin$(COIN_MAJOR_VERSION)-doc_$(VERSION)
	distdir=coin$(COIN_MAJOR_VERSION)-doc_$(VERSION); \
	$(MAKE) DEBIAN_DIST_DIR=$$distdir debian-control; \
	rm -f $distdir/debian/postinst;

debian-src: coin$(COIN_MAJOR_VERSION)_$(VERSION) debian-src-control
debian-lib: coin$(COIN_MAJOR_VERSION)-lib_$(VERSION) debian-lib-control
debian-dev: coin$(COIN_MAJOR_VERSION)-dev_$(VERSION) debian-dev-control
debian-doc: coin$(COIN_MAJOR_VERSION)-doc_$(VERSION) debian-doc-control

debian-setup: debian-src debian-lib debian-dev debian-doc

debian-packages: debian-setup
	cd coin1_$(VERSION); \
	dpkg-buildpackage -S -uc -us;
	cd coin1-lib_$(VERSION); \
	dpkg-buildpackage -b -uc;
	cd coin1-dev_$(VERSION); \
	dpkg-buildpackage -b -uc;
	cd coin1-doc_$(VERSION); \
	dpkg-buildpackage -b -uc;

# EOF ######################################################################
