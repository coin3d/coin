## Process this file with automake to generate Makefile.in.

COINLIB = libCoin@SUFFIX@.la

lib_LTLIBRARIES = $(COINLIB)

if HACKING_DYNAMIC_MODULES
libCoin@SUFFIX@_la_LIBADD = 
else
libCoin@SUFFIX@_la_LIBADD = \
	VRMLnodes/libVRMLnodes.la actions/libactions.la base/libbase.la \
	bundles/libbundles.la caches/libcaches.la elements/libelements.la \
	engines/libengines.la errors/liberrors.la events/libevents.la \
	fields/libfields.la misc/libmisc.la lists/liblists.la \
	nodekits/libnodekits.la nodes/libnodes.la projectors/libprojectors.la \
	sensors/libsensors.la details/libdetails.la draggers/libdraggers.la \
	manips/libmanips.la
endif

SUBDIRS = \
	VRMLnodes actions base bundles caches details draggers elements \
	engines errors events fields manips misc lists nodekits nodes \
	projectors sensors system .

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

libCoin@SUFFIX@_la_SOURCES = snprintf.c snprintf.h

## Installation information.

libCoin@SUFFIX@_la_LDFLAGS = \
	-no-undefined \
        -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libCoin@SUFFIX@incdir = $(includedir)/Inventor
libCoin@SUFFIX@inc_HEADERS = \
	$(top_builddir)/include/Inventor/SbBasic.h \
	$(top_srcdir)/include/Inventor/SbPList.h \
	$(top_srcdir)/include/Inventor/So.h \
	$(top_srcdir)/include/Inventor/SoDB.h \
	$(top_srcdir)/include/Inventor/SoFullPath.h \
	$(top_srcdir)/include/Inventor/SoNodeKitPath.h \
	$(top_srcdir)/include/Inventor/SoInput.h \
	$(top_srcdir)/include/Inventor/SoInteraction.h \
	$(top_srcdir)/include/Inventor/SoOffscreenRenderer.h \
	$(top_srcdir)/include/Inventor/SoOutput.h \
	$(top_srcdir)/include/Inventor/SoPath.h \
	$(top_srcdir)/include/Inventor/SoPickedPoint.h \
	$(top_srcdir)/include/Inventor/SoPrimitiveVertex.h \
	$(top_srcdir)/include/Inventor/SoSceneManager.h \
	$(top_srcdir)/include/Inventor/SoType.h \
	$(top_srcdir)/include/Inventor/Sb.h \
	$(top_srcdir)/include/Inventor/SbBox.h \
	$(top_srcdir)/include/Inventor/SbBSPTree.h \
	$(top_srcdir)/include/Inventor/SbLinear.h \
	$(top_srcdir)/include/Inventor/SoLists.h \
	$(top_srcdir)/include/Inventor/SbBox2f.h \
	$(top_srcdir)/include/Inventor/SbBox2s.h \
	$(top_srcdir)/include/Inventor/SbBox3f.h \
	$(top_srcdir)/include/Inventor/SbColor.h \
	$(top_srcdir)/include/Inventor/SbColor4f.h \
	$(top_srcdir)/include/Inventor/SbCylinder.h \
	$(top_srcdir)/include/Inventor/SbDict.h \
	$(top_srcdir)/include/Inventor/SbHeap.h \
	$(top_srcdir)/include/Inventor/SbImage.h \
	$(top_srcdir)/include/Inventor/SbLine.h \
	$(top_srcdir)/include/Inventor/SbMatrix.h \
	$(top_srcdir)/include/Inventor/SbName.h \
	$(top_srcdir)/include/Inventor/SbOctTree.h \
	$(top_srcdir)/include/Inventor/SbPlane.h \
	$(top_srcdir)/include/Inventor/SbRotation.h \
	$(top_srcdir)/include/Inventor/SbSphere.h \
	$(top_srcdir)/include/Inventor/SbString.h \
	$(top_srcdir)/include/Inventor/SbTesselator.h \
	$(top_srcdir)/include/Inventor/SbTime.h \
	$(top_srcdir)/include/Inventor/SbVec2f.h \
	$(top_srcdir)/include/Inventor/SbVec2s.h \
	$(top_srcdir)/include/Inventor/SbVec3f.h \
	$(top_srcdir)/include/Inventor/SbVec4f.h \
	$(top_srcdir)/include/Inventor/SbViewVolume.h \
	$(top_srcdir)/include/Inventor/SbViewportRegion.h \
	$(top_srcdir)/include/Inventor/SbXfBox3f.h

#### Additional "native" Make rules #########################################

## Rule for linking up a .DLL on MSWindows with MSVC++.
##
## Short explanation of what the shell-code does: tests if the C
## compiler is our MSVC++ wrapper script, and if so: properly link the
## DLL. If not, just make an empty coin{version}.dll file so the
## install-exec-hook knows when to _not_ install the .dll.

coin@COIN_MAJOR_VERSION@@SUFFIX@.dll: $(libCoin@SUFFIX@_la_OBJECTS) $(libCoin@SUFFIX@_la_DEPENDENCIES)
	@rm -f coin@COIN_MAJOR_VERSION@@SUFFIX@.dll && \
	if @BUILD_WITH_MSVC@; then \
	  echo "Linking DLL..."; \
	  $(CC) @DLLFLAGS@ /out:coin@COIN_MAJOR_VERSION@@SUFFIX@.dll \
	        $(LDFLAGS) \
	        `echo $(libCoin@SUFFIX@_la_OBJECTS) | sed 's%\.lo%.obj%g'` \
	        $(libCoin@SUFFIX@_la_LIBADD) $(LIBS); \
	else \
	  touch coin@COIN_MAJOR_VERSION@@SUFFIX@.dll; \
	fi

## This hook will be run at the end of a ``make install''.
##
## The single action taken is to install the coin.dll and coin.lib
## files (if they have been built properly).

install-exec-hook: coin@COIN_MAJOR_VERSION@@SUFFIX@.dll
	@file="coin@COIN_MAJOR_VERSION@@SUFFIX@.dll"; \
	if test -s $$file; then \
	  echo "Installing $$file"; \
	  $(INSTALL_DATA) $$file $(DESTDIR)$(bindir)/$$file; \
	  for e in lib pdb; do \
	    file="coin@COIN_MAJOR_VERSION@@SUFFIX@.$$e"; \
	    test -s $$file && \
	      $(INSTALL_DATA) $$file $(DESTDIR)$(libdir)/$$file; \
	  done \
	fi

uninstall: uninstall-am
	@file="$(DESTDIR)$(bindir)/coin@COIN_MAJOR_VERSION@@SUFFIX@.dll"; \
	test -f $$file && echo rm -f $$file && rm -f $$file; \
	for e in lib pdb; do \
	  file="$(DESTDIR)$(libdir)/coin@COIN_MAJOR_VERSION@@SUFFIX@.$$e"; \
	  test -f $$file && echo rm -f $$file && rm -f $$file; \
	done

