## Process this file with automake to generate Makefile.in.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libnodekitsLINKHACK.la
else
noinst_LTLIBRARIES = libnodekits.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
	SoNodeKit.cpp \
	SoNodeKit.h

libnodekits_la_SOURCES = $(ALLSOURCES)
libnodekitsLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libnodekitsincdir = $(includedir)/Inventor/nodekits
libnodekitsLINKHACKincdir = $(includedir)/Inventor/nodekits

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/nodekits/SoNodeKit.h


libnodekitsinc_HEADERS = $(ALLINSTHEADERS)
libnodekitsLINKHACKinc_HEADERS = $(ALLINSTHEADERS)

## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libnodekitsincHEADERS: $(libnodekitsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libnodekitsincdir)
	@list='$(libnodekitsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libnodekitsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libnodekitsincdir)/$$f; \
	done

uninstall-libnodekitsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libnodekitsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libnodekitsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libnodekitsincdir)/$$f; \
	done


install-libnodekitsLINKHACKincHEADERS: $(libnodekitsLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libnodekitsLINKHACKincdir)
	@list='$(libnodekitsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libnodekitsLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libnodekitsLINKHACKincdir)/$$f; \
	done

uninstall-libnodekitsLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libnodekitsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libnodekitsLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libnodekitsLINKHACKincdir)/$$f; \
	done
