## Process this file with automake to generate Makefile.in.

##$ BEGIN TEMPLATE Make-CompileSilent()

# This macro contains fixes to avoid the default flush of output from
# the automake generated rules. 19991201 mortene. 


# FIXME: the non-dependency rules won't be overridden without make
# emitting ugly warnings. 19991201 mortene.

#.cpp.o:
#	@echo Compiling $< | sed -e 's/\/.*\///'
#	@$(CXXCOMPILE) -c $<
#
#.cpp.lo:
#	@echo Compiling $< | sed -e 's/\/.*\///'
#	@$(LTCXXCOMPILE) -c $<
#

%.o: %.cpp
	@echo Compiling $< | sed -e 's/\/.*\///'; \
	$(CXXCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-cp .deps/$(*F).pp .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm .deps/$(*F).pp

%.lo: %.cpp
	@echo Compiling $< | sed -e 's/\/.*\///'; \
	$(LTCXXCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-sed -e 's/^\([^:]*\)\.o[ 	]*:/\1.lo \1.o :/' \
	  < .deps/$(*F).pp > .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm -f .deps/$(*F).pp

##$ END TEMPLATE Make-CompileSilent
##$ BEGIN TEMPLATE Make-LibtoolSilent()

# This macro contains fixes to avoid the default flush of output from
# the automake generated rules. 19991201 mortene. 

LIBTOOL = @LIBTOOL@ --silent
##$ END TEMPLATE Make-LibtoolSilent

lib_LTLIBRARIES = libCoin.la

if HACKING_DYNAMIC_MODULES
TESTLIBDIR = coindev
libCoin_la_LIBADD = -lcoindevLINKHACK \
	-lVRMLnodesLINKHACK -lactionsLINKHACK -lbaseLINKHACK \
	-lbundlesLINKHACK -lcachesLINKHACK -lelementsLINKHACK \
	-lenginesLINKHACK -lerrorsLINKHACK -leventsLINKHACK -lfieldsLINKHACK \
	-lmiscLINKHACK -llistsLINKHACK -lnodekitsLINKHACK -lnodesLINKHACK \
	-lprojectorsLINKHACK -lsensorsLINKHACK -ldetailsLINKHACK
else
TESTLIBDIR = 
libCoin_la_LIBADD = \
	VRMLnodes/libVRMLnodes.la actions/libactions.la base/libbase.la \
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

## The build process won't complete the final link stage unless we've
## got at least one sourcefile specified here. So it seems there's a
## bug (or missing feature) in Automake, which we fool by using the
## empty sourcefile dummy.cpp. 19991108 mortene.

libCoin_la_SOURCES = dummy.cpp

## Installation information.

libCoinincdir = $(includedir)/Inventor

libCoin_la_LDFLAGS = \
        -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
        -release $(LT_RELEASE)

libCoininc_HEADERS = \
	$(top_srcdir)/include/Inventor/soconfig.h \
	$(top_srcdir)/include/Inventor/confdep.h \
	$(top_builddir)/include/Inventor/SbBasic.h \
	$(top_srcdir)/include/Inventor/SbPList.h \
	$(top_srcdir)/include/Inventor/So.h \
	$(top_srcdir)/include/Inventor/SoDB.h \
	$(top_srcdir)/include/Inventor/SoFullPath.h \
	$(top_srcdir)/include/Inventor/SoNodeKitPath.h \
	$(top_srcdir)/include/Inventor/SoInput.h \
	$(top_srcdir)/include/Inventor/SoInteraction.h \
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


##$ BEGIN TEMPLATE Make-Install(Coin)

# FIXME: this is a temporary workaround for a bug in Automake. The
# bug is fixed in the latest Automake CVS, but not in any "official"
# release (?). Last checked with Automake 1.4a. 19990912 mortene.

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

##$ END TEMPLATE Make-Install
