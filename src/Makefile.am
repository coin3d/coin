## Process this file with automake to generate Makefile.in.

lib_LTLIBRARIES = libCoin.la

if HACKING_DYNAMIC_MODULES
TESTLIBDIR = coindev
libCoin_la_LIBADD = -lcoindevLINKHACK \
	-lVRMLnodesLINKHACK -lactionsLINKHACK -lcoinbaseLINKHACK \
	-lbundlesLINKHACK -lcachesLINKHACK -lelementsLINKHACK \
	-lenginesLINKHACK -lerrorsLINKHACK -leventsLINKHACK -lfieldsLINKHACK \
	-lmiscLINKHACK -llistsLINKHACK -lnodekitsLINKHACK -lnodesLINKHACK \
	-lprojectorsLINKHACK -lsensorsLINKHACK -ldetailsLINKHACK
else
TESTLIBDIR = 
libCoin_la_LIBADD = \
	VRMLnodes/libVRMLnodes.la actions/libactions.la base/libcoinbase.la \
	bundles/libbundles.la caches/libcaches.la elements/libelements.la \
	engines/libengines.la errors/liberrors.la events/libevents.la \
	fields/libfields.la misc/libmisc.la lists/liblists.la \
	nodekits/libnodekits.la nodes/libnodes.la projectors/libprojectors.la \
	sensors/libsensors.la details/libdetails.la
endif


SUBDIRS = $(TESTLIBDIR) \
	VRMLnodes actions base bundles caches details elements engines errors \
	events fields misc lists nodekits nodes projectors sensors system .

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files from the base directory used when building the
## library, including all headers.

libCoin_la_SOURCES = \
	SoDB.cpp \
	SoDB.h \
	SoFullPath.cpp \
	SoFullPath.h \
	SoImageInterface.cpp \
	SoImageInterface.h \
	SoInput.cpp \
	SoInput.h \
	SoInteraction.cpp \
	SoInteraction.h \
	SoOutput.cpp \
	SoOutput.h \
	SoPath.cpp \
	SoPath.h \
	SoPickedPoint.cpp \
	SoPickedPoint.h \
	SoPrimitiveVertex.cpp \
	SoPrimitiveVertex.h \
	SoSceneManager.cpp \
	SoSceneManager.h \
	SoType.cpp \
	SoType.h

## Installation information.

libCoinincdir = $(includedir)/Inventor

libCoin_la_LDFLAGS = \
        -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
        -release $(LT_RELEASE)

libCoininc_HEADERS = \
	$(top_srcdir)/include/Inventor/soconfig.h \
	$(top_srcdir)/include/Inventor/confdep.h \
	$(top_srcdir)/include/Inventor/SbPList.h \
	$(top_srcdir)/include/Inventor/So.h \
	$(top_srcdir)/include/Inventor/SoDB.h \
	$(top_srcdir)/include/Inventor/SoFullPath.h \
	$(top_srcdir)/include/Inventor/SoImageInterface.h \
	$(top_srcdir)/include/Inventor/SoInput.h \
	$(top_srcdir)/include/Inventor/SoInteraction.h \
	$(top_srcdir)/include/Inventor/SoOutput.h \
	$(top_srcdir)/include/Inventor/SoPath.h \
	$(top_srcdir)/include/Inventor/SoPickedPoint.h \
	$(top_srcdir)/include/Inventor/SoPrimitiveVertex.h \
	$(top_srcdir)/include/Inventor/SoSceneManager.h \
	$(top_srcdir)/include/Inventor/SoType.h


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libCoinincHEADERS: $(libCoininc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libCoinincdir)
	@list='$(libCoininc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libCoinincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libCoinincdir)/$$f; \
	done

uninstall-libCoinincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libCoininc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libCoinincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libCoinincdir)/$$f; \
	done
