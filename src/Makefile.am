# **************************************************************************
# Process this file with automake to generate Makefile.in.
# **************************************************************************

# **************************************************************************
# Extensions which might be present.

if BUILD_VRML97
VRML97DIR = extensions/vrml97/src
VRML97LIBADD = $(VRML97DIR)/libvrml97.la
VRML97LSTADD = $(VRML97DIR)/vrml97.lst
else
VRML97DIR =
VRML97LIBADD =
VRML97LSTADD =
endif

if BUILD_WITH_SUPERGLU
SUPERGLUDIR = extensions/superglu
SUPERGLULIBADD = $(SUPERGLUDIR)/lib${SUPERGLUPREFIX}GLU.la
SUPERGLULSTADD = $(SUPERGLUDIR)/${SUPERGLUPREFIX}GLU.lst
else
SUPERGLUDIR =
SUPERGLULIBADD =
SUPERGLULSTADD =
endif

if BUILD_WITH_THREADS
THREADSDIR = threads
THREADSLIBADD = threads/libthreads.la
THREADSLSTADD = threads/threads.lst
else
THREADSDIR =
THREADSLIBADD =
THREADSLSTADD =
endif

# **************************************************************************

SUBDIRS = \
	extensions actions base bundles caches details draggers elements \
	engines errors events fields manips misc lists nodekits nodes \
	projectors sensors $(THREADSDIR) $(VRML97DIR) .

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
	actions/libactions.la base/libbase.la \
	bundles/libbundles.la caches/libcaches.la elements/libelements.la \
	engines/libengines.la errors/liberrors.la events/libevents.la \
	fields/libfields.la misc/libmisc.la lists/liblists.la \
	nodekits/libnodekits.la nodes/libnodes.la projectors/libprojectors.la \
	sensors/libsensors.la details/libdetails.la draggers/libdraggers.la \
	manips/libmanips.la $(THREADSLIBADD) \
	$(VRML97LIBADD) $(SUPERGLULIBADD)
endif

libCoin@SUFFIX@_la_SOURCES = \
	tidbits.c tidbits.h

coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_LIBADD = \
	actions/actions.lst base/base.lst \
	bundles/bundles.lst caches/caches.lst elements/elements.lst \
	engines/engines.lst errors/errors.lst events/events.lst \
	fields/fields.lst misc/misc.lst lists/lists.lst \
	nodekits/nodekits.lst nodes/nodes.lst projectors/projectors.lst \
	sensors/sensors.lst details/details.lst draggers/draggers.lst \
	manips/manips.lst $(THREADSLSTADD) \
	$(VRML97LSTADD) $(SUPERGLULSTADD)

coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_SOURCES = \
	tidbits.c tidbits.h

## Installation information.

libCoin@SUFFIX@_la_LDFLAGS = \
	-no-undefined \
        -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

# header installation
libCoinincdir = $(includedir)/Inventor
libCoininc_HEADERS = \
	../include/Inventor/SbPList.h \
	../include/Inventor/So.h \
	../include/Inventor/SoDB.h \
	../include/Inventor/SoFullPath.h \
	../include/Inventor/SoNodeKitPath.h \
	../include/Inventor/SoInput.h \
	../include/Inventor/SoInteraction.h \
	../include/Inventor/SoOffscreenRenderer.h \
	../include/Inventor/SoOutput.h \
	../include/Inventor/SoPath.h \
	../include/Inventor/SoPickedPoint.h \
	../include/Inventor/SoPrimitiveVertex.h \
	../include/Inventor/SoSceneManager.h \
	../include/Inventor/SoType.h \
	../include/Inventor/Sb.h \
	../include/Inventor/SbBox.h \
	../include/Inventor/SbBSPTree.h \
	../include/Inventor/SbLinear.h \
	../include/Inventor/SbDPLinear.h \
	../include/Inventor/SoLists.h \
	../include/Inventor/SbBox2s.h \
	../include/Inventor/SbBox2f.h \
	../include/Inventor/SbBox2d.h \
	../include/Inventor/SbBox3f.h \
	../include/Inventor/SbClip.h \
	../include/Inventor/SbColor.h \
	../include/Inventor/SbColor4f.h \
	../include/Inventor/SbCylinder.h \
	../include/Inventor/SbDict.h \
	../include/Inventor/SbDPLine.h \
	../include/Inventor/SbDPLinear.h \
	../include/Inventor/SbDPMatrix.h \
	../include/Inventor/SbDPRotation.h \
	../include/Inventor/SbHeap.h \
	../include/Inventor/SbImage.h \
	../include/Inventor/SbLine.h \
	../include/Inventor/SbMatrix.h \
	../include/Inventor/SbName.h \
	../include/Inventor/SbOctTree.h \
	../include/Inventor/SbPlane.h \
	../include/Inventor/SbRotation.h \
	../include/Inventor/SbSphere.h \
	../include/Inventor/SbString.h \
	../include/Inventor/SbTesselator.h \
	../include/Inventor/SbTime.h \
	../include/Inventor/SbVec2s.h \
	../include/Inventor/SbVec2f.h \
	../include/Inventor/SbVec2d.h \
	../include/Inventor/SbVec3s.h \
	../include/Inventor/SbVec3f.h \
	../include/Inventor/SbVec3d.h \
	../include/Inventor/SbVec4f.h \
	../include/Inventor/SbVec4d.h \
	../include/Inventor/SbViewVolume.h \
	../include/Inventor/SbViewportRegion.h \
	../include/Inventor/SbXfBox3f.h

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
	  "$$baseincdir/threads" \
	  "$$baseincdir"; do \
	  if test -d "$$dir"; then \
	    echo " rmdir" "$$dir"; \
	    rmdir "$$dir"; \
	  fi \
	done; \
	:

