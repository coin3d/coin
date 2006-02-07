# **************************************************************************

CLEANFILES = \
	coin@COIN_MAJOR_VERSION@@SUFFIX@.pdb \
	coin@COIN_MAJOR_VERSION@@SUFFIX@.dll \
	coin@COIN_MAJOR_VERSION@@SUFFIX@.exp

# **************************************************************************
# Extensions which might be present.

if BUILD_VRML97
VRML97DIR = vrml97
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

# **************************************************************************

SUBDIRS = \
	extensions actions base bundles caches details draggers elements \
	engines errors events fields fonts glue io manips misc lists nodekits \
	nodes shapenodes projectors sensors upgraders \
	3ds collision hardcopy threads mpeg shaders $(VRML97DIR) foreignfiles .

DEFAULT_INCLUDES = -I$(top_builddir)/include -I$(top_srcdir)/include

# **************************************************************************

if BUILD_WITH_MSVC
lib_LIBRARIES = coin@COIN_MAJOR_VERSION@@SUFFIX@.lib
else
lib_LTLIBRARIES = libCoin@SUFFIX@.la
endif

if MAC_FRAMEWORK
# hack to change libdir value when installing Mac OS X compiler framework
lib@nop@dir = $(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Libraries
endif

# **************************************************************************

if HACKING_DYNAMIC_MODULES
libCoin@SUFFIX@_la_LIBADD = 
else
libCoin@SUFFIX@_la_LIBADD = \
	actions/libactions.la base/libbase.la \
	bundles/libbundles.la caches/libcaches.la elements/libelements.la \
	elements/GL/libglelements.la engines/libengines.la \
	errors/liberrors.la events/libevents.la fields/libfields.la \
	fonts/libfonts.la glue/libglue.la io/libio.la misc/libmisc.la \
	lists/liblists.la nodekits/libnodekits.la nodes/libnodes.la \
	shapenodes/libshapenodes.la projectors/libprojectors.la \
	sensors/libsensors.la details/libdetails.la draggers/libdraggers.la \
	manips/libmanips.la upgraders/libupgraders.la \
	3ds/libformat3ds.la collision/libcollision.la hardcopy/libhardcopy.la \
	threads/libthreads.la mpeg/libmpeg.la shaders/libshaders.la \
	foreignfiles/libforeignfiles.la \
	$(VRML97LIBADD) $(SUPERGLULIBADD)
endif

# **************************************************************************
# Source code files for local build & dist.

RegularSources = \
	tidbits.c \
	share/gl/CoinGLPerformance.cpp \
	coin-dummy.cpp

PrivateHeaders = \
	share/gl/CoinGLPerformance.h

libCoin@SUFFIX@_la_SOURCES = $(RegularSources)
coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_SOURCES = $(RegularSources)

EXTRA_libCoin@SUFFIX@_la_SOURCES = $(PrivateHeaders)
EXTRA_coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_SOURCES = $(PrivateHeaders)

# **************************************************************************

coin@COIN_MAJOR_VERSION@@SUFFIX@_lib_LIBADD = \
	actions/actions.lst base/base.lst \
	bundles/bundles.lst caches/caches.lst elements/elements.lst \
	elements/GL/glelements.lst engines/engines.lst \
	errors/errors.lst events/events.lst fields/fields.lst \
	fonts/fonts.lst glue/glue.lst io/io.lst misc/misc.lst lists/lists.lst \
	nodekits/nodekits.lst nodes/nodes.lst shapenodes/shapenodes.lst \
	projectors/projectors.lst sensors/sensors.lst \
	details/details.lst draggers/draggers.lst manips/manips.lst \
	upgraders/upgraders.lst \
	3ds/format3ds.lst collision/collision.lst hardcopy/hardcopy.lst \
	threads/threads.lst mpeg/mpeg.lst shaders/shaders.lst \
	foreignfiles/foreignfiles.lst \
	$(VRML97LSTADD) $(SUPERGLULSTADD)

# **************************************************************************
# Installation information.

# On Mac OS 10.2 or later, create two-level namespace binaries and use
# prebinding. Also pad the install_name to make it possible to change
# it later (e.g. to include the library in an app bundle).  On Mac OS
# 10.3 or later, use -undefined dynamic_lookup to delay symbol lookup
# to runtime (needed to make enable_hacking work)

if MACOSX_10_2FF
if MACOSX_USE_DYNAMIC_LOOKUP
# Must export deployment target environment, since it is required by
# the linker to allow the -undefined dynamic_lookup flag.
export MACOSX_DEPLOYMENT_TARGET=@MACOSX_DEPLOYMENT_TARGET@
LibtoolLinkArgs = -no-undefined -Wl,-twolevel_namespace -Wl,-undefined,dynamic_lookup -Wl,-headerpad_max_install_names 
else
LibtoolLinkArgs = -no-undefined -Wl,-twolevel_namespace -Wl,-undefined,error -Wl,-prebind -Wl,-seg1addr,0x3f000000 -Wl,-headerpad_max_install_names 
endif
else
LibtoolLinkArgs = -no-undefined
endif

libCoin@SUFFIX@_la_LDFLAGS = \
	$(LibtoolLinkArgs) \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

# header installation
if MAC_FRAMEWORK
libCoinincdir = $(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Headers
else
libCoinincdir = $(includedir)/Inventor
endif
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
	../include/Inventor/SbBox3s.h \
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
	../include/Inventor/SbDPPlane.h \
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
	../include/Inventor/SbDPViewVolume.h \
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
	    $(INSTALL_HEADER) "$$src" "$$dst" || exit 1; \
	  fi \
	done

if MAC_FRAMEWORK
install-data-local:
	cd $(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/Current && ln -sf Libraries/libCoin.dylib $(MAC_FRAMEWORK_NAME) && cd -; \
	cd $(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework && ln -sf Versions/Current/$(MAC_FRAMEWORK_NAME) $(MAC_FRAMEWORK_NAME) && install_name_tool -id $(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/$(MAC_FRAMEWORK_NAME) $(MAC_FRAMEWORK_NAME) && cd -;
else
install-data-local:
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	for ext in lib; do \
	  file="$$basename.$$ext"; \
	  if test -f "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(libdir)"; \
	    echo " $(INSTALL_DATA) $$file $(DESTDIR)$(libdir)/$$file"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(libdir)/$$file" || exit 1; \
	  fi \
	done;
endif

if BUILD_WITH_MSVC
install-exec-local: coin@COIN_MAJOR_VERSION@@SUFFIX@.lib
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	for ext in dll pdb; do \
	  file="$$basename.$$ext"; \
	  if test -f "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(bindir)"; \
	    echo " $(INSTALL_PROGRAM) $$file $(DESTDIR)$(bindir)/$$file"; \
	    $(INSTALL_PROGRAM) "$$file" "$(DESTDIR)$(bindir)/$$file" || exit 1; \
	  fi \
	done
else
install-exec-local:
endif

uninstall-local:
	@basename="coin@COIN_MAJOR_VERSION@@SUFFIX@"; \
	for file in \
	  "$(DESTDIR)$(bindir)/$$basename.dll" \
	  "$(DESTDIR)$(bindir)/$$basename.pdb" \
	  "$(DESTDIR)$(libdir)/$$basename.lib" \
	  "$(DESTDIR)$(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/$(MAKE_FRAMEWORK_NAME)"; do \
	  if test -f "$$file"; then \
	    echo " rm -f" "$$file"; \
	    rm -f "$$file"; \
	  fi \
	done; \
	baseincdir="$(DESTDIR)$(libCoinincdir)"; \
	for dir in \
	  "$$baseincdir/3ds" \
	  "$$baseincdir/actions" \
	  "$$baseincdir/base" \
	  "$$baseincdir/bundles" \
	  "$$baseincdir/caches" \
	  "$$baseincdir/collision" \
	  "$$baseincdir/details" \
	  "$$baseincdir/draggers" \
	  "$$baseincdir/elements" \
	  "$$baseincdir/engines" \
	  "$$baseincdir/errors" \
	  "$$baseincdir/events" \
	  "$$baseincdir/fields" \
	  "$$baseincdir/fonts" \
	  "$$baseincdir/glue" \
	  "$$baseincdir/hardcopy" \
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
	  "$$baseincdir/upgraders" \
	  "$$baseincdir/VRMLnodes" \
	  "$$baseincdir/MPEG" \
	  "$$baseincdir/C/base" \
	  "$$baseincdir/C/errors" \
	  "$$baseincdir/C/glue" \
	  "$$baseincdir/C/threads" \
	  "$$baseincdir/C" \
	  "$$baseincdir"; do \
	  if test -d "$$dir"; then \
	    echo " rmdir $$dir"; \
	    rmdir "$$dir"; \
	  fi \
	done; \
	:

