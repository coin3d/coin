## Process this file with automake to generate Makefile.in.

SUBDIRS = \
	VRMLnodes actions bundles caches elements engines errors events \
	fields misc lists nodekits nodes projectors sensors system .

lib_LTLIBRARIES = libCoin.la

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files from the base directory used when building the
## library, including all headers.

libCoin_la_SOURCES = \
	SbBSPTree.cpp \
	SbBSPTree.h \
	SbBox2f.cpp \
	SbBox2f.h \
	SbBox2s.cpp \
	SbBox2s.h \
	SbBox3f.cpp \
	SbBox3f.h \
	SbColor.cpp \
	SbColor.h \
	SbColor4f.cpp \
	SbColor4f.h \
	SbCylinder.cpp \
	SbCylinder.h \
	SbDict.cpp \
	SbDict.h \
	SbLine.cpp \
	SbLine.h \
	SbMatrix.cpp \
	SbMatrix.h \
	SbName.cpp \
	SbName.h \
	SbPlane.cpp \
	SbPlane.h \
	SbRotation.cpp \
	SbRotation.h \
	SbSphere.cpp \
	SbSphere.h \
	SbString.cpp \
	SbString.h \
	SbTime.cpp \
	SbTime.h \
	SbVec2f.cpp \
	SbVec2f.h \
	SbVec2s.cpp \
	SbVec2s.h \
	SbVec3f.cpp \
	SbVec3f.h \
	SbVec4f.cpp \
	SbVec4f.h \
	SbViewVolume.cpp \
	SbViewVolume.h \
	SbViewportRegion.cpp \
	SbViewportRegion.h \
	SbXfBox3f.cpp \
	SbXfBox3f.h \
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

libCoin_la_LIBADD = \
	VRMLnodes/libVRMLnodes.la actions/libactions.la \
	bundles/libbundles.la caches/libcaches.la elements/libelements.la \
	engines/libengines.la errors/liberrors.la events/libevents.la \
	fields/libfields.la misc/libmisc.la lists/liblists.la \
	nodekits/libnodekits.la nodes/libnodes.la projectors/libprojectors.la \
	sensors/libsensors.la

## Installation information.

libCoinincdir = $(includedir)/Inventor

libCoin_la_LDFLAGS = \
        -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
        -release $(LT_RELEASE)

libCoininc_HEADERS = \
	$(top_srcdir)/include/Inventor/soconfig.h \
	$(top_srcdir)/include/Inventor/confdep.h \
	$(top_srcdir)/include/Inventor/SbBox.h \
	$(top_srcdir)/include/Inventor/SbLinear.h \
	$(top_srcdir)/include/Inventor/SoLists.h \
	$(top_srcdir)/include/Inventor/SbBox2f.h \
	$(top_srcdir)/include/Inventor/SbBox2s.h \
	$(top_srcdir)/include/Inventor/SbBox3f.h \
	$(top_srcdir)/include/Inventor/SbColor.h \
	$(top_srcdir)/include/Inventor/SbColor4f.h \
	$(top_srcdir)/include/Inventor/SbCylinder.h \
	$(top_srcdir)/include/Inventor/SbDict.h \
	$(top_srcdir)/include/Inventor/SbLine.h \
	$(top_srcdir)/include/Inventor/SbMatrix.h \
	$(top_srcdir)/include/Inventor/SbName.h \
	$(top_srcdir)/include/Inventor/SbPlane.h \
	$(top_srcdir)/include/Inventor/SbRotation.h \
	$(top_srcdir)/include/Inventor/SbSphere.h \
	$(top_srcdir)/include/Inventor/SbString.h \
	$(top_srcdir)/include/Inventor/SbTime.h \
	$(top_srcdir)/include/Inventor/SbVec2f.h \
	$(top_srcdir)/include/Inventor/SbVec2s.h \
	$(top_srcdir)/include/Inventor/SbVec3f.h \
	$(top_srcdir)/include/Inventor/SbVec4f.h \
	$(top_srcdir)/include/Inventor/SbViewVolume.h \
	$(top_srcdir)/include/Inventor/SbViewportRegion.h \
	$(top_srcdir)/include/Inventor/SbXfBox3f.h \
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
