## Process this file with automake to generate Makefile.in.

SUBDIRS = C lock system

BUILT_SOURCES = \
	SbBasic.h

DISTCLEANFILES = \
	SbBasic.h

EXTRA_DIST = \
	SbBasic.h.in

SbBasic.h: $(srcdir)/SbBasic.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

if BUILD_LIBRARY
if MAC_FRAMEWORK
libCoinincdir = $(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Headers
else
libCoinincdir = $(includedir)/Inventor
endif
nodist_libCoininc_HEADERS = \
	SbBasic.h

install-nodist_libCoinincHEADERS: $(nodist_libCoininc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libCoinincdir)
	@list='$(nodist_libCoininc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(libCoinincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

endif

