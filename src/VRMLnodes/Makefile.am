## Process this file with automake to generate Makefile.in.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libVRMLnodesLINKHACK.la
else
noinst_LTLIBRARIES = libVRMLnodes.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
	SoVRMLInterpOutput.cpp \
	SoVRMLInterpOutput.h \
	SoVRMLInterpolator.cpp \
	SoVRMLInterpolator.h

libVRMLnodes_la_SOURCES = $(ALLSOURCES)
libVRMLnodesLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libVRMLnodesincdir = $(includedir)/Inventor/VRMLnodes
libVRMLnodesLINKHACKincdir = $(includedir)/Inventor/VRMLnodes

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/VRMLnodes/SoVRMLInterpOutput.h \
	$(top_srcdir)/include/Inventor/VRMLnodes/SoVRMLInterpolator.h

libVRMLnodesinc_HEADERS = $(ALLINSTHEADERS)
libVRMLnodesLINKHACKinc_HEADERS = $(ALLINSTHEADERS)


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libVRMLnodesincHEADERS: $(libVRMLnodesinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libVRMLnodesincdir)
	@list='$(libVRMLnodesinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libVRMLnodesincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libVRMLnodesincdir)/$$f; \
	done

uninstall-libVRMLnodesincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libVRMLnodesinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libVRMLnodesincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libVRMLnodesincdir)/$$f; \
	done


install-libVRMLnodesLINKHACKincHEADERS: $(libVRMLnodesLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libVRMLnodesLINKHACKincdir)
	@list='$(libVRMLnodesLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libVRMLnodesLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libVRMLnodesLINKHACKincdir)/$$f; \
	done

uninstall-libVRMLnodesLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libVRMLnodesLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libVRMLnodesLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libVRMLnodesLINKHACKincdir)/$$f; \
	done
