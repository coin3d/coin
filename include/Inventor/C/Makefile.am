if BUILD_WITH_THREADS
THREADSDIR = threads
else
THREADSDIR =
endif

SUBDIRS = base errors glue $(THREADSDIR)


BUILT_SOURCES = \
	basic.h

DISTCLEANFILES = \
	basic.h

EXTRA_DIST = \
	basic.h.in

basic.h: $(srcdir)/basic.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

if BUILD_LIBRARY
if MAC_FRAMEWORK
ccoinincdir = $(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Headers/C
else
ccoinincdir = $(includedir)/Inventor/C
endif
nodist_ccoininc_HEADERS = \
	basic.h

ccoininc_HEADERS = \
	tidbits.h

# FIXME: could get rid of this Automake override stuff by adding a new
# feature flag to Automake ("AM_CMP_BEFORE_INSTALL" or something.)
# 20021124 mortene.

install-ccoinincHEADERS: $(ccoininc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(ccoinincdir)
	@list='$(ccoininc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(ccoinincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

install-nodist_ccoinincHEADERS: $(nodist_ccoininc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(ccoinincdir)
	@list='$(nodist_ccoininc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(ccoinincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

endif
