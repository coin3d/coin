## Process this file with automake to generate Makefile.in.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libbundlesLINKHACK.la
else
noinst_LTLIBRARIES = libbundles.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
	SoBundle.cpp \
	SoBundle.h \
	SoMaterialBundle.cpp \
	SoMaterialBundle.h \
	SoTextureCoordinateBundle.cpp \
	SoTextureCoordinateBundle.h

libbundles_la_SOURCES = $(ALLSOURCES)
libbundlesLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libbundlesincdir = $(includedir)/Inventor/bundles
libbundlesLINKHACKincdir = $(includedir)/Inventor/bundles

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/bundles/SoBundle.h \
	$(top_srcdir)/include/Inventor/bundles/SoMaterialBundle.h \
	$(top_srcdir)/include/Inventor/bundles/SoTextureCoordinateBundle.h

libbundlesinc_HEADERS = $(ALLINSTHEADERS)
libbundlesLINKHACKinc_HEADERS = $(ALLINSTHEADERS)

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



install-libbundlesLINKHACKincHEADERS: $(libbundlesLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libbundlesLINKHACKincdir)
	@list='$(libbundlesLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libbundlesLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libbundlesLINKHACKincdir)/$$f; \
	done

uninstall-libbundlesLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libbundlesLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libbundlesLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libbundlesLINKHACKincdir)/$$f; \
	done
