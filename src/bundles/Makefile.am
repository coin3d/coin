## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = libbundles.la

INCLUDES = -I$(top_srcdir)/include/

## All source files used when building this submodule, including all
## headers.

libbundles_la_SOURCES = \
	SoBundle.cpp \
	SoBundle.h \
	SoMaterialBundle.cpp \
	SoMaterialBundle.h \
	SoTextureCoordinateBundle.cpp \
	SoTextureCoordinateBundle.h

## Installation information.

libbundlesincdir = $(includedir)/Inventor/bundles

libbundlesinc_HEADERS = \
	$(top_srcdir)/include/Inventor/bundles/SoBundle.h \
	$(top_srcdir)/include/Inventor/bundles/SoMaterialBundle.h \
	$(top_srcdir)/include/Inventor/bundles/SoTextureCoordinateBundle.h


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libbundlesincHEADERS: $(libbundlesinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libbundlesincdir)
	@list='$(libbundlesinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libbundlesincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libbundlesincdir)/$$f; \
	done

uninstall-libbundlesincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libbundlesinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libbundlesincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libbundlesincdir)/$$f; \
	done
