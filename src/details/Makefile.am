## Process this file with automake to generate Makefile.in.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libdetailsLINKHACK.la
else
noinst_LTLIBRARIES = libdetails.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
	SoDetail.cpp \
	SoDetail.h \
	SoDetails.h \
	SoConeDetail.cpp \
	SoConeDetail.h \
	SoCubeDetail.cpp \
	SoCubeDetail.h \
	SoCylinderDetail.cpp \
	SoCylinderDetail.h \
	SoFaceDetail.cpp \
	SoFaceDetail.h \
	SoLineDetail.cpp \
	SoLineDetail.h \
	SoNodeKitDetail.cpp \
	SoNodeKitDetail.h \
	SoPointDetail.cpp \
	SoPointDetail.h \
	SoTextDetail.cpp \
	SoTextDetail.h

libdetails_la_SOURCES = $(ALLSOURCES)
libdetailsLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libdetailsincdir = $(includedir)/Inventor/details
libdetailsLINKHACKincdir = $(includedir)/Inventor/details

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/details/SoDetail.h \
	$(top_srcdir)/include/Inventor/details/SoDetails.h \
	$(top_srcdir)/include/Inventor/details/SoConeDetail.h \
	$(top_srcdir)/include/Inventor/details/SoCubeDetail.h \
	$(top_srcdir)/include/Inventor/details/SoCylinderDetail.h \
	$(top_srcdir)/include/Inventor/details/SoFaceDetail.h \
	$(top_srcdir)/include/Inventor/details/SoLineDetail.h \
	$(top_srcdir)/include/Inventor/details/SoNodeKitDetail.h \
	$(top_srcdir)/include/Inventor/details/SoPointDetail.h \
	$(top_srcdir)/include/Inventor/details/SoTextDetail.h


libdetailsinc_HEADERS = $(ALLINSTHEADERS)
libdetailsLINKHACKinc_HEADERS = $(ALLINSTHEADERS)

## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libdetailsincHEADERS: $(libdetailsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libdetailsincdir)
	@list='$(libdetailsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libdetailsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libdetailsincdir)/$$f; \
	done

uninstall-libdetailsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libdetailsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libdetailsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libdetailsincdir)/$$f; \
	done


install-libdetailsLINKHACKincHEADERS: $(libdetailsLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libdetailsLINKHACKincdir)
	@list='$(libdetailsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libdetailsLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libdetailsLINKHACKincdir)/$$f; \
	done

uninstall-libdetailsLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libdetailsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libdetailsLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libdetailsLINKHACKincdir)/$$f; \
	done
