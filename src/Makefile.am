# **************************************************************************
# Process this file with automake to generate Makefile.in.

if BUILD_VRML97
VRML97DIR = vrml97
VRML97LIBADD = $(VRML97DIR)/libvrml97.la
VRML97LSTADD = $(VRML97DIR)/vrml97.lst
else
VRML97DIR =
VRML97LIBADD =
VRML97LSTADD =
endif

SUBDIRS = \
	VRMLnodes actions base bundles caches details draggers elements \
	engines errors events fields manips misc lists nodekits nodes \
	projectors sensors $(VRML97DIR) .

INCLUDES = -I$(top_srcdir)/include

if BUILD_WITH_MSVC
lib_LIBRARIES = coin@COIN_MAJOR_VERSION@@SUFFIX@.lib
else
lib_LTLIBRARIES = libCoin@SUFFIX@.la
endif

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
	manips/libmanips.la $(VRML97LIBADD)
endif

libCoin@SUFFIX@_la_SOURCES = \
	snprintf.c snprintf.h

coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_LIBADD = \
	VRMLnodes/VRMLnodes.lst actions/actions.lst base/base.lst \
	bundles/bundles.lst caches/caches.lst elements/elements.lst \
	engines/engines.lst errors/errors.lst events/events.lst \
	fields/fields.lst misc/misc.lst lists/lists.lst \
	nodekits/nodekits.lst nodes/nodes.lst projectors/projectors.lst \
	sensors/sensors.lst details/details.lst draggers/draggers.lst \
	manips/manips.lst $(VRML97LSTADD)

coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_SOURCES = \
	snprintf.c snprintf.h

## Installation information.

libCoin@SUFFIX@_la_LDFLAGS = \
	-no-undefined \
        -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

# header installation
libCoinincdir = $(includedir)/Inventor
libCoininc_HEADERS = \
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

# **************************************************************************

# Custom rule for linking a Visual C++ (MS Windows) library.

coin@COIN_MAJOR_VERSION@@SUFFIX@.lib: Makefile \
  $(coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_OBJECTS) \
  $(coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_DEPENDENCIES)
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	rm -f "$$basename.dll" "$$basename.lib"; \
	echo "Linking $$basename.lib..."; \
	echo $(CC) $(LDFLAGS) $(LIBFLAGS) \
	  $(coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_OBJECTS) \
	  $(coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_LIBADD) $(LIBS); \
	$(CC) $(LDFLAGS) $(LIBFLAGS) \
	  $(coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_OBJECTS) \
	  $(coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_LIBADD) $(LIBS);

# **************************************************************************

install-libCoinincHEADERS: $(libCoininc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libCoinincdir)
	@list='$(libCoininc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(libCoinincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

install-exec-local:
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	for ext in dll pdb; do \
	  file="$$basename.$$ext"; \
	  if test -f "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(bindir)"; \
	    echo " $(INSTALL_PROGRAM)" "$$file" "$(DESTDIR)$(bindir)/$$file"; \
	    $(INSTALL_PROGRAM) "$$file" "$(DESTDIR)$(bindir)/$$file"; \
	  fi \
	done

install-data-local:
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	for ext in lib; do \
	  file="$$basename.$$ext"; \
	  if test -f "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(libdir)"; \
	    echo " $(INSTALL_DATA)" "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	  fi \
	done;

uninstall-local:
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	for file in \
	  "$(DESTDIR)$(bindir)/$$basename.dll" \
	  "$(DESTDIR)$(bindir)/$$basename.pdb" \
	  "$(DESTDIR)$(libdir)/$$basename.lib"; do \
	  if test -f "$$file"; then \
	    echo " rm -f" "$$file"; \
	    rm -f "$$file"; \
	  fi \
	done; \
	baseincdir="$(DESTDIR)$(includedir)/Inventor"; \
	for dir in \
	  "$$baseincdir/VRMLNodes" \
	  "$$baseincdir/actions" \
	  "$$baseincdir/base" \
	  "$$baseincdir/bundles" \
	  "$$baseincdir/caches" \
	  "$$baseincdir/details" \
	  "$$baseincdir/draggers" \
	  "$$baseincdir/elements" \
	  "$$baseincdir/engines" \
	  "$$baseincdir/errors" \
	  "$$baseincdir/events" \
	  "$$baseincdir/fields" \
	  "$$baseincdir/lists" \
	  "$$baseincdir/lock" \
	  "$$baseincdir/manips" \
	  "$$baseincdir/misc" \
	  "$$baseincdir/nodekits" \
	  "$$baseincdir/nodes" \
	  "$$baseincdir/projectors" \
	  "$$baseincdir/sensors" \
	  "$$baseincdir/system" \
	  "$$baseincdir"; do \
	  if test -d "$$dir"; then \
	    echo " rmdir" "$$dir"; \
	    rmdir "$$dir"; \
	  fi \
	done;

