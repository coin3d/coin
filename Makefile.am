###########################################################################
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
MANDIR =
else
MANDIR = man
endif

SUBDIRS = . bin include $(SRCDIRS) $(MANDIR) $(HTMLDIRS)

BUILT_SOURCES = \
	include/Inventor/C/basic.h \
	include/Inventor/SbBasic.h \
	include/Inventor/system/gl.h \
	include/Inventor/system/inttypes.h \
	coin-@CONFIG@.cfg

CLEANFILES = \
	coin@COIN_MAJOR_VERSION@.dsp \
	coin@COIN_MAJOR_VERSION@.dsw \
	coin@COIN_MAJOR_VERSION@.ncb \
	coin@COIN_MAJOR_VERSION@.opt \
	coin@COIN_MAJOR_VERSION@.plg

DISTCLEANFILES = \
	coin-@CONFIG@.cfg \
	docs/coin.doxygen \
	docs/coin.doxygen.bak

if BUILD_LIBRARY
m4datadir = $(datadir)/aclocal
dist_m4data_DATA = cfg/coin.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = coin-@CONFIG@.cfg
endif

EXTRA_DIST = \
	FAQ \
	FAQ.legal \
	INSTALL.GNU \
	LICENSE.GPL \
	README.AIX \
	README.IRIX \
	README.MAC \
	README.SUN \
	README.VRML97 \
	README.WIN32 \
	RELNOTES \
	bootstrap \
	coin.cfg.in \
	Info.plist \
	docs/ChangeLog.1999 \
	docs/ChangeLog.2000 \
	docs/ChangeLog.2001 \
	docs/ChangeLog.2002 \
	docs/ChangeLog.2003 \
	docs/announcement-2_0_0.txt \
	docs/coin.doxygen.in \
	docs/oiki-launch.txt \
	cfg/ltconfig \
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
	packaging/macosx/coin.info.template.in \
	packaging/macosx/checklist.txt \
	packaging/macosx/Coin_Description.plist.in \
	packaging/macosx/Coin_Info.plist.in \
	packaging/macosx/Coin_Welcome.rtfd/TXT.rtf \
	packaging/macosx/CoinTools_Description.plist.in \
	packaging/macosx/CoinTools_Info.plist.in \
	packaging/macosx/CoinTools_Welcome.rtfd/TXT.rtf \
	packaging/macosx/inst-dmg-files/.DS_Store \
	packaging/macosx/inst-dmg-files/background.png \
	packaging/macosx/inst-dmg-files/Desktop_DB \
	packaging/macosx/inst-dmg-files/Desktop_DF \
	packaging/macosx/License.rtfd/TXT.rtf \
	packaging/macosx/makecoinpkg.sh.in \
	packaging/macosx/makecointoolspkg.sh.in \
	packaging/macosx/makedmg.sh.in \
	packaging/macosx/Makefile.in \
	packaging/macosx/makeinstdmg.sh.in \
	packaging/macosx/makenoinstdmg.sh.in \
	packaging/macosx/noinst-dmg-files/.DS_Store \
	packaging/macosx/noinst-dmg-files/background.png \
	packaging/macosx/noinst-dmg-files/Desktop_DB \
	packaging/macosx/noinst-dmg-files/Desktop_DF \
	packaging/macosx/README.txt.in \
	packaging/macosx/uninstall-coin.pl \
	packaging/redhat/coin.spec.in \
	packaging/windows/Makefile.in \
	packaging/windows/bfheader.nsi.in \
	packaging/windows/coin-sdk.html.in \
	packaging/windows/footer.nsi.in \
	packaging/windows/heading.nsi.in \
	packaging/windows/middle.nsi.in \
	packaging/windows/post.nsi.in \
	examples/bindings/glutiv.cpp

# FIXME: These files should really be added instead of the ones
#        containing '_' instead of ' '. If this is fixed, also
#        update packaging/macosx/make*dmg.sh.in (kintel 20040119)
#	packaging/macosx/inst-dmg-files/Desktop DB \
#	packaging/macosx/inst-dmg-files/Desktop DF \
#	packaging/macosx/noinst-dmg-files/Desktop DB \
#	packaging/macosx/noinst-dmg-files/Desktop DF \
#

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

docs/coin.doxygen: $(srcdir)/docs/coin.doxygen.in config.status
	@if test -d docs; then :; else mkdir docs; fi
	@./config.status --file=$@:$@.in
	@$(srcdir)/cfg/doxy4win.pl docs/coin.doxygen

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
if MAC_FRAMEWORK
install-exec-local:
install-data-local:
	@$(NORMAL_INSTALL)
	@echo "Setting up Mac OS X Compiler Framework"
	@if test -d "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)"; then :; else \
	  echo " $(mkinstalldirs) $(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)"; \
	  $(mkinstalldirs) "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)"; \
	  echo " $(mkinstalldirs) $(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Resources"; \
	  $(mkinstalldirs) "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Resources"; \
	fi
	@if test -e "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/Current"; then \
	  echo " rm -f $(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/Current"; \
	  rm -f "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/Current"; \
	fi; \
	echo " ln -s $(MAC_FRAMEWORK_VERSION) $(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/Current"; \
	ln -s $(MAC_FRAMEWORK_VERSION) "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/Current";
	@for symlink in Headers Resources Libraries; do \
	  if test -L "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/$$symlink"; then \
	    echo " rm -f $(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/$$symlink"; \
	    rm -f "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/$$symlink"; \
	  fi; \
	  echo " ln -s Versions/Current/$$symlink $(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/$$symlink"; \
	  ln -s Versions/Current/$$symlink "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/$$symlink" || exit 1; \
	done
else
install-data-local:
install-exec-local: $(srcdir)/cfg/wrapmsvc.exe
	@$(NORMAL_INSTALL)
	@if @BUILD_WITH_MSVC@; then \
	  echo " $(mkinstalldirs) $(DESTDIR)$(bindir)"; \
	  $(mkinstalldirs) $(DESTDIR)$(bindir); \
	  p=wrapmsvc.exe; \
	  echo " $(INSTALL_PROGRAM) $(srcdir)/cfg/$$p $(DESTDIR)$(bindir)/$$p"; \
	  $(INSTALL_PROGRAM) $(srcdir)/cfg/$$p $(DESTDIR)$(bindir)/$$p; \
	else :; fi
endif
endif

cvs-update:
	@cd $(top_srcdir) && cvs -z3 -q update -dP

# **************************************************************************
# misc rules for automatic debian packaging.  main: `debian-packages'

if DEB_PACKAGEABLE

coin-$(VERSION):
	if test -d coin-$(VERSION); then \
	  rm -rf coin-$(VERSION); \
	else :; fi;
	$(MAKE) distdir;
	mv Coin-$(VERSION) coin-$(VERSION)

coin_$(VERSION).orig.tar.gz:
	$(MAKE) dist
	mv Coin-$(VERSION).tar.gz coin_$(VERSION).orig.tar.gz

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
	distdir=coin-$(VERSION); \
	$(MAKE) DEBIAN_DIST_DIR=$$distdir debian-files;

debian-setup: coin-$(VERSION) debian-control

debian-packages: debian-setup coin_$(VERSION).orig.tar.gz
	@( cd coin-$(VERSION); dpkg-buildpackage )

endif

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

# **************************************************************************
# misc rules for automatic Mac OS X packaging.
# main: `macosx-packages'

if MACOSX_PACKAGEABLE

macosx-files:
	@if test -d macosx; then :; else mkdir macosx; fi
	@for file in $(top_srcdir)/packaging/macosx/*.in; do \
	  basename=`basename $$file .in`; \
          if ! test -f macosx/$$basename; then \
	    ./config.status --file=macosx/$$basename:$$file; \
          fi \
	done

macosx-packages: macosx-files
macosx-packages:
	@( cd macosx; $(MAKE) VERSION=$(VERSION) )

endif

# EOF ######################################################################
