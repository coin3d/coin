############################################################################
##

ACLOCAL_AMFLAGS = -I cfg/m4

############################################################################
##

if BUILD_LIBRARY
SRCDIRS = data src
else
SRCDIRS =
endif

if BUILD_HTMLPAGES
HTMLDIRS = html
else
HTMLDIRS =
endif

if MAC_FRAMEWORK
BINDIR =
MANDIR =
else
BINDIR = bin
MANDIR = man
endif

SUBDIRS = . $(BINDIR) include $(SRCDIRS) $(MANDIR) $(HTMLDIRS)

BUILT_SOURCES = \
	include/Inventor/C/basic.h \
	include/Inventor/SbBasic.h \
	include/Inventor/system/gl.h \
	include/Inventor/system/inttypes.h \
	coin-@CONFIG@.cfg

DISTCLEANFILES = \
	coin-@CONFIG@.cfg

if BUILD_LIBRARY
m4datadir = $(datadir)/aclocal
dist_m4data_DATA = cfg/m4/coin.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = coin-@CONFIG@.cfg
endif

EXTRA_DIST = \
	LICENSE.GPL \
	README.AIX \
	README.SUN \
	README.IRIX \
	README.MAC \
	README.WIN32 \
	coin.cfg.in \
	docs/ChangeLog.1999 \
	docs/ChangeLog.2000 \
	docs/ChangeLog.2001 \
	docs/coin.doxygen.in \
	cfg/m4/doxy4win.pl \
	cfg/m4/errors.txt \
	cfg/ltconfig \
	cfg/m4/wrapmsvc.cpp \
	cfg/m4/wrapmsvc.exe \
	packaging/debian/changelog.in \
	packaging/debian/control.in \
	packaging/debian/copyright.in \
	packaging/debian/rules.in \
	packaging/debian/libcoin40.files.in \
	packaging/debian/libcoin40.README.Debian.in \
	packaging/debian/libcoin40-dev.files.in \
	packaging/debian/libcoin40-dev.README.Debian.in \
	packaging/debian/libcoin40-doc.files.in \
	packaging/debian/libcoin40-doc.README.Debian.in \
	packaging/debian/libcoin40-runtime.files.in \
	packaging/debian/libcoin40-runtime.README.Debian.in \
	packaging/redhat/coin.spec.in

coin-@CONFIG@.cfg: config.status $(srcdir)/coin.cfg.in
	@$(SHELL) config.status --file=$@:coin.cfg.in

# sources needed to generate docs must be created by this rule
built-sources: $(BUILT_SOURCES)

include/Inventor/C/basic.h: $(srcdir)/include/Inventor/C/basic.h.in config.status
	@./config.status --header=$@

include/Inventor/SbBasic.h: $(srcdir)/include/Inventor/SbBasic.h.in config.status
	@./config.status --header=$@

include/Inventor/system/gl.h: $(srcdir)/include/Inventor/system/gl.h.in config.status
	@./config.status --file=include/Inventor/system/gl.h.new:include/Inventor/system/gl.h.in; \
	if cmp -s include/Inventor/system/gl.h include/Inventor/system/gl.h.new; then \
	  echo "include/Inventor/system/gl.h is unchanged"; \
	  rm -f include/Inventor/system/gl.h.new; \
	else \
	  mv include/Inventor/system/gl.h.new include/Inventor/system/gl.h; \
	fi

include/Inventor/system/inttypes.h: $(srcdir)/include/Inventor/system/inttypes.h.in config.status
	@./config.status --header=$@

doxygen-doc: built-sources
	if test x"@COIN_DOC_HTML@" = x"YES"; then \
	  mkdir -p "@coin_html_dir@"; \
	else :; fi
	if test x"@COIN_DOC_MAN@" = x"YES"; then \
	  mkdir -p "@coin_man_dir@"; \
	else :; fi
	@sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen

doxygen-docs: built-sources
	if test x"@COIN_DOC_HTML@" = x"YES"; then \
	  mkdir -p "@coin_html_dir@"; \
	else :; fi
	if test x"@COIN_DOC_MAN@" = x"YES"; then \
	  mkdir -p "@coin_man_dir@"; \
	else :; fi
	@sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen

if BUILD_LIBRARY
install-exec-local: $(srcdir)/cfg/m4/wrapmsvc.exe
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@if @BUILD_WITH_MSVC@; then \
	  p=wrapmsvc.exe; \
	  echo " $(INSTALL_PROGRAM) $(srcdir)/cfg/m4/$$p $(DESTDIR)$(bindir)/$$p"; \
	  $(INSTALL_PROGRAM) $(srcdir)/cfg/m4/$$p $(DESTDIR)$(bindir)/$$p; \
	fi
endif

cvs-update:
	@cd $(top_srcdir) && cvs -z3 -q update -dP

# **************************************************************************
# misc rules for automatic debian packaging.  main: `debian-packages'

libcoin$(COIN_MAJOR_VERSION)-$(VERSION):
	if test -d libcoin$(COIN_MAJOR_VERSION)-$(VERSION); then \
	  rm -rf libcoin$(COIN_MAJOR_VERSION)-$(VERSION); \
	else :; fi;
	$(MAKE) distdir;
	mv Coin-$(VERSION) libcoin$(COIN_MAJOR_VERSION)-$(VERSION)

debian-files: $(DEBIAN_DIST_DIR)
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

debian-control:
	distdir=libcoin$(COIN_MAJOR_VERSION)-$(VERSION); \
	$(MAKE) DEBIAN_DIST_DIR=$$distdir debian-files;

debian-setup: libcoin$(COIN_MAJOR_VERSION)-$(VERSION) debian-control

debian-packages: debian-setup
	cd libcoin$(COIN_MAJOR_VERSION)-$(VERSION); \
	./debian/rules source;
	cd libcoin$(COIN_MAJOR_VERSION)-$(VERSION); \
	./debian/rules binary;

# **************************************************************************
# misc rules for automatic Microsoft Windows packaging.
# main: `windows-packages'

if WIN_PACKAGEABLE

windows-files:
	@if test -d windows; then :; else mkdir windows; fi
	@for file in $(top_srcdir)/packaging/windows/*.in; do \
	  basename=`echo $$file | sed 's/^.*\///g; s/\.in$$//'`; \
	  ./config.status --file=windows/$$basename:$$file; \
	done

windows-packages: windows-files
	@( cd windows; $(MAKE) VERSION=$(VERSION) )

endif

# EOF ######################################################################
