## Process this file with automake to generate Makefile.in.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libenginesLINKHACK.la
else
noinst_LTLIBRARIES = libengines.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
	SoBoolOperation.cpp \
	SoBoolOperation.h \
	SoCalculator.cpp \
	SoCalculator.h \
	SoComposeMatrix.cpp \
	SoComposeMatrix.h \
	SoComposeRotation.cpp \
	SoComposeRotation.h \
	SoComposeVec2f.cpp \
	SoComposeVec2f.h \
	SoComposeVec3f.cpp \
	SoComposeVec3f.h \
	SoComposeVec4f.cpp \
	SoComposeVec4f.h \
	SoConcatenate.cpp \
	SoConcatenate.h \
	SoConvertAll.cpp \
	SoConvertAll.h \
	SoCounter.cpp \
	SoCounter.h \
	SoDecomposeMatrix.cpp \
	SoDecomposeMatrix.h \
	SoDecomposeRotation.cpp \
	SoDecomposeRotation.h \
	SoDecomposeVec2f.cpp \
	SoDecomposeVec2f.h \
	SoDecomposeVec3f.cpp \
	SoDecomposeVec3f.h \
	SoDecomposeVec4f.cpp \
	SoDecomposeVec4f.h \
	SoElapsedTime.cpp \
	SoElapsedTime.h \
	SoEngine.cpp \
	SoEngine.h \
	SoEngineOutput.cpp \
	SoEngineOutput.h \
	SoExpressionParser.cpp \
	SoExpressionParser.h \
	SoFieldConverter.cpp \
	SoFieldConverter.h \
	SoGate.cpp \
	SoGate.h \
	SoInterpolateAbs.cpp \
	SoInterpolateAbs.h \
	SoInterpolateFloat.cpp \
	SoInterpolateFloat.h \
	SoInterpolateRotation.cpp \
	SoInterpolateRotation.h \
	SoInterpolateVec2f.cpp \
	SoInterpolateVec2f.h \
	SoInterpolateVec3f.cpp \
	SoInterpolateVec3f.h \
	SoInterpolateVec4f.cpp \
	SoInterpolateVec4f.h \
	SoOnOff.cpp \
	SoOnOff.h \
	SoOneShot.cpp \
	SoOneShot.h \
	SoSelectOne.cpp \
	SoSelectOne.h \
	SoTriggerAny.cpp \
	SoTriggerAny.h

libengines_la_SOURCES = $(ALLSOURCES)
libenginesLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libenginesincdir = $(includedir)/Inventor/engines
libenginesLINKHACKincdir = $(includedir)/Inventor/engines

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/engines/SoBoolOperation.h \
	$(top_srcdir)/include/Inventor/engines/SoCalculator.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeMatrix.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeRotation.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeVec2f.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeVec3f.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeVec4f.h \
	$(top_srcdir)/include/Inventor/engines/SoConcatenate.h \
	$(top_srcdir)/include/Inventor/engines/SoConvertAll.h \
	$(top_srcdir)/include/Inventor/engines/SoCounter.h \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeMatrix.h \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeRotation.h \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeVec2f.h \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeVec3f.h \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeVec4f.h \
	$(top_srcdir)/include/Inventor/engines/SoElapsedTime.h \
	$(top_srcdir)/include/Inventor/engines/SoEngine.h \
	$(top_srcdir)/include/Inventor/engines/SoEngineOutput.h \
	$(top_srcdir)/include/Inventor/engines/SoExpressionParser.h \
	$(top_srcdir)/include/Inventor/engines/SoFieldConverter.h \
	$(top_srcdir)/include/Inventor/engines/SoGate.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolate.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateAbs.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateFloat.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateRotation.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateVec2f.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateVec3f.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateVec4f.h \
	$(top_srcdir)/include/Inventor/engines/SoOnOff.h \
	$(top_srcdir)/include/Inventor/engines/SoOneShot.h \
	$(top_srcdir)/include/Inventor/engines/SoSelectOne.h \
	$(top_srcdir)/include/Inventor/engines/SoTriggerAny.h


libenginesinc_HEADERS = $(ALLINSTHEADERS)
libenginesLINKHACKinc_HEADERS = $(ALLINSTHEADERS)

## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libenginesincHEADERS: $(libenginesinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libenginesincdir)
	@list='$(libenginesinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libenginesincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libenginesincdir)/$$f; \
	done

uninstall-libenginesincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libenginesinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libenginesincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libenginesincdir)/$$f; \
	done


install-libenginesLINKHACKincHEADERS: $(libenginesLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libenginesLINKHACKincdir)
	@list='$(libenginesLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libenginesLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libenginesLINKHACKincdir)/$$f; \
	done

uninstall-libenginesLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libenginesLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libenginesLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libenginesLINKHACKincdir)/$$f; \
	done
