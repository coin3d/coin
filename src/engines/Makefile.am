## Process this file with automake to generate Makefile.in.
## 19990904 mortene.

if HACKING_COMPACT_BUILD
ALLSOURCES = all.cpp
else
ALLSOURCES = \
	SoBoolOperation.cpp \
	$(top_srcdir)/include/Inventor/engines/SoBoolOperation.h \
	SoCalculator.cpp \
	$(top_srcdir)/include/Inventor/engines/SoCalculator.h \
	SoComposeMatrix.cpp \
	$(top_srcdir)/include/Inventor/engines/SoComposeMatrix.h \
	SoComposeRotation.cpp \
	$(top_srcdir)/include/Inventor/engines/SoComposeRotation.h \
	SoComposeRotationFromTo.cpp \
	$(top_srcdir)/include/Inventor/engines/SoComposeRotationFromTo.h \
	SoComposeVec2f.cpp \
	$(top_srcdir)/include/Inventor/engines/SoComposeVec2f.h \
	SoComposeVec3f.cpp \
	$(top_srcdir)/include/Inventor/engines/SoComposeVec3f.h \
	SoComposeVec4f.cpp \
	$(top_srcdir)/include/Inventor/engines/SoComposeVec4f.h \
	SoComputeBoundingBox.cpp \
	$(top_srcdir)/include/Inventor/engines/SoComputeBoundingBox.h \
	SoConcatenate.cpp \
	$(top_srcdir)/include/Inventor/engines/SoConcatenate.h \
	SoConvertAll.cpp \
	$(top_srcdir)/include/Inventor/engines/SoConvertAll.h \
	SoCounter.cpp \
	$(top_srcdir)/include/Inventor/engines/SoCounter.h \
	SoDecomposeMatrix.cpp \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeMatrix.h \
	SoDecomposeRotation.cpp \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeRotation.h \
	SoDecomposeVec2f.cpp \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeVec2f.h \
	SoDecomposeVec3f.cpp \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeVec3f.h \
	SoDecomposeVec4f.cpp \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeVec4f.h \
	SoElapsedTime.cpp \
	$(top_srcdir)/include/Inventor/engines/SoElapsedTime.h \
	SoEngine.cpp \
	$(top_srcdir)/include/Inventor/engines/SoEngine.h \
	SoEngineOutput.cpp \
	$(top_srcdir)/include/Inventor/engines/SoEngineOutput.h \
	SoFieldConverter.cpp \
	$(top_srcdir)/include/Inventor/engines/SoFieldConverter.h \
	SoGate.cpp \
	$(top_srcdir)/include/Inventor/engines/SoGate.h \
	SoInterpolate.cpp \
	$(top_srcdir)/include/Inventor/engines/SoInterpolate.h \
	SoInterpolateFloat.cpp \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateFloat.h \
	SoInterpolateRotation.cpp \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateRotation.h \
	SoInterpolateVec2f.cpp \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateVec2f.h \
	SoInterpolateVec3f.cpp \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateVec3f.h \
	SoInterpolateVec4f.cpp \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateVec4f.h \
	SoOnOff.cpp \
	$(top_srcdir)/include/Inventor/engines/SoOnOff.h \
	SoOneShot.cpp \
	$(top_srcdir)/include/Inventor/engines/SoOneShot.h \
        SoOutputData.cpp \
	$(top_srcdir)/include/Inventor/engines/SoOutputData.h \
	SoSelectOne.cpp \
	$(top_srcdir)/include/Inventor/engines/SoSelectOne.h \
	SoTimeCounter.cpp \
	$(top_srcdir)/include/Inventor/engines/SoTimeCounter.h \
	SoTransformVec3f.cpp \
	$(top_srcdir)/include/Inventor/engines/SoTransformVec3f.h \
	SoTriggerAny.cpp \
	$(top_srcdir)/include/Inventor/engines/SoTriggerAny.h \
	$(top_srcdir)/include/Inventor/engines/SoSubEngine.h \
	$(top_srcdir)/include/Inventor/engines/SoSubEngineP.h \
	evaluator.c \
	evaluator.h \
	evaluator_tab.c
endif

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/engines/SoSubEngine.h \
	$(top_srcdir)/include/Inventor/engines/SoEngines.h \
	$(top_srcdir)/include/Inventor/engines/SoBoolOperation.h \
	$(top_srcdir)/include/Inventor/engines/SoCalculator.h \
	$(top_srcdir)/include/Inventor/engines/SoCompose.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeMatrix.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeRotation.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeRotationFromTo.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeVec2f.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeVec3f.h \
	$(top_srcdir)/include/Inventor/engines/SoComposeVec4f.h \
	$(top_srcdir)/include/Inventor/engines/SoComputeBoundingBox.h \
	$(top_srcdir)/include/Inventor/engines/SoConcatenate.h \
	$(top_srcdir)/include/Inventor/engines/SoCounter.h \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeMatrix.h \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeRotation.h \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeVec2f.h \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeVec3f.h \
	$(top_srcdir)/include/Inventor/engines/SoDecomposeVec4f.h \
	$(top_srcdir)/include/Inventor/engines/SoElapsedTime.h \
	$(top_srcdir)/include/Inventor/engines/SoEngine.h \
	$(top_srcdir)/include/Inventor/engines/SoEngineOutput.h \
	$(top_srcdir)/include/Inventor/engines/SoFieldConverter.h \
	$(top_srcdir)/include/Inventor/engines/SoGate.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolate.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateFloat.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateRotation.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateVec2f.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateVec3f.h \
	$(top_srcdir)/include/Inventor/engines/SoInterpolateVec4f.h \
	$(top_srcdir)/include/Inventor/engines/SoOnOff.h \
	$(top_srcdir)/include/Inventor/engines/SoOneShot.h \
	$(top_srcdir)/include/Inventor/engines/SoOutputData.h \
	$(top_srcdir)/include/Inventor/engines/SoSelectOne.h \
	$(top_srcdir)/include/Inventor/engines/SoTimeCounter.h \
	$(top_srcdir)/include/Inventor/engines/SoTransformVec3f.h \
	$(top_srcdir)/include/Inventor/engines/SoTriggerAny.h


# Don't use the explicit rules, they will just confuse users on
# systems without flex and/or bison. 20000606 mortene.

#BUILT_SOURCES = \
#	$(top_srcdir)/src/engines/lex.so_eval.c \
#	$(top_srcdir)/src/engines/evaluator.tab.c

#$(top_srcdir)/src/engines/lex.so_eval.c: $(top_srcdir)/src/engines/evaluator.l
#	flex -o$(top_srcdir)/src/engines/lex.so_eval.c -L $(top_srcdir)/src/engines/evaluator.l

#$(top_srcdir)/src/engines/evaluator_tab.c: $(top_srcdir)/src/engines/lex.so_eval.c $(top_srcdir)/src/engines/evaluator.y
#	bison -p so_eval -o $(top_srcdir)/src/engines/evaluator_tab.c -l $(top_srcdir)/src/engines/evaluator.y



##$ BEGIN TEMPLATE Make-Common(engines)

# Everything below is autogenerated from a macro, so don't modify
# within the Makefile.am itself. 19991201 mortene.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libengines@SUFFIX@LINKHACK.la
else
noinst_LTLIBRARIES = libengines.la
endif

# Remove any linklibraries detected by configure, as they are not
# needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/include -I$(top_builddir)/include

libengines_la_SOURCES = $(ALLSOURCES)
libengines@SUFFIX@LINKHACK_la_SOURCES = $(ALLSOURCES)

libenginesincdir = $(includedir)/Inventor/engines
libenginesinc_HEADERS = $(ALLINSTHEADERS)

##$ END TEMPLATE Make-Common
