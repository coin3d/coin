## Process this file with automake to generate Makefile.in.
## 19990904 mortene.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libbaseLINKHACK.la
else
noinst_LTLIBRARIES = libbase.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
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
	SbHeap.cpp \
	SbHeap.h \
	SbLine.cpp \
	SbLine.h \
	SbMatrix.cpp \
	SbMatrix.h \
	SbName.cpp \
	SbName.h \
	SbOctTree.cpp \
	SbOctTree.h \
	SbPlane.cpp \
	SbPlane.h \
	SbRotation.cpp \
	SbRotation.h \
	SbSphere.cpp \
	SbSphere.h \
	SbString.cpp \
	SbString.h \
	SbTesselator.cpp \
	SbTesselator.h \
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
	SbXfBox3f.h

libbase_la_SOURCES = $(ALLSOURCES)
libbaseLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libbaseincdir = $(includedir)/Inventor
libbaseLINKHACKincdir = $(includedir)/Inventor

ALLINSTHEADERS = \
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


libbaseinc_HEADERS = $(ALLINSTHEADERS)
libbaseLINKHACKinc_HEADERS = $(ALLINSTHEADERS)


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libbaseincHEADERS: $(libbaseinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libbaseincdir)
	@list='$(libbaseinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libbaseincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libbaseincdir)/$$f; \
	done

uninstall-libbaseincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libbaseinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libbaseincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libbaseincdir)/$$f; \
	done


install-libbaseLINKHACKincHEADERS: $(libbaseLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libbaseLINKHACKincdir)
	@list='$(libbaseLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libbaseLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libbaseLINKHACKincdir)/$$f; \
	done

uninstall-libbaseLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libbaseLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libbaseLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libbaseLINKHACKincdir)/$$f; \
	done
