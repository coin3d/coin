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

## Rule for creating a Windows library.

coin@COIN_MAJOR_VERSION@@SUFFIX@.lib: Makefile $(libCoin@SUFFIX@_la_OBJECTS) $(libCoin@SUFFIX@_la_DEPENDENCIES)
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	rm -f "$$basename.dll" "$$basename.lib"; \
	if @BUILD_WITH_MSVC@; then \
	  echo "Linking $$basename.lib..."; \
	  echo $(CC) $(LDFLAGS) $(LIBFLAGS) \
	    `echo $(libCoin@SUFFIX@_la_OBJECTS) | sed 's%\.lo%.obj%g'` \
	    $(libCoin@SUFFIX@_la_LIBADD) $(LIBS); \
	  $(CC) $(LDFLAGS) $(LIBFLAGS) \
	    `echo $(libCoin@SUFFIX@_la_OBJECTS) | sed 's%\.lo%.obj%g'` \
	    $(libCoin@SUFFIX@_la_LIBADD) $(LIBS); \
	else \
	  touch "$$basename.dll" "$$basename.lib"; \
	fi

# **************************************************************************

all-local: coin@COIN_MAJOR_VERSION@@SUFFIX@.lib

install-exec-local:
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	if test -s "$$basename.dll"; then \
	  $(mkinstalldirs) "$(DESTDIR)$(bindir)"; \
	  echo " $(INSTALL_PROGRAM)" "$$basename.dll" "$(DESTDIR)$(bindir)/$$basename.dll"; \
	  $(INSTALL_PROGRAM) "$$basename.dll" "$(DESTDIR)$(bindir)/$$basename.dll"; \
	fi

install-data-local:
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	for ext in lib pdb; do \
	  file="$$basename.$$ext"; \
	  if test -s "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(libdir)"; \
	    echo " $(INSTALL_DATA)" "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	  fi \
	done

uninstall-local:
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	for file in \
	  "$(DESTDIR)$(bindir)/$$basename.dll" \
	  "$(DESTDIR)$(libdir)/$$basename.pdb" \
	  "$(DESTDIR)$(libdir)/$$basename.lib"; do \
	  if test -f "$$file"; then \
	    echo " rm -f" "$$file"; \
	    rm -f "$$file"; \
	  fi \
	done

