## Process this file with automake to generate Makefile.in.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libsensorsLINKHACK.la
else
noinst_LTLIBRARIES = libsensors.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
	SoAlarmSensor.cpp \
	SoAlarmSensor.h \
	SoDataSensor.cpp \
	SoDataSensor.h \
	SoDelayQueueSensor.cpp \
	SoDelayQueueSensor.h \
	SoFieldSensor.cpp \
	SoFieldSensor.h \
	SoIdleSensor.cpp \
	SoIdleSensor.h \
	SoNodeSensor.cpp \
	SoNodeSensor.h \
	SoOneShotSensor.cpp \
	SoOneShotSensor.h \
	SoPathSensor.cpp \
	SoPathSensor.h \
	SoSensor.cpp \
	SoSensor.h \
	SoSensorManager.cpp \
	SoSensorManager.h \
	SoTimerQueueSensor.cpp \
	SoTimerQueueSensor.h \
	SoTimerSensor.cpp \
	SoTimerSensor.h

libsensors_la_SOURCES = $(ALLSOURCES)
libsensorsLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libsensorsincdir = $(includedir)/Inventor/sensors
libsensorsLINKHACKincdir = $(includedir)/Inventor/sensors

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/sensors/SoSensors.h \
	$(top_srcdir)/include/Inventor/sensors/SoAlarmSensor.h \
	$(top_srcdir)/include/Inventor/sensors/SoDataSensor.h \
	$(top_srcdir)/include/Inventor/sensors/SoDelayQueueSensor.h \
	$(top_srcdir)/include/Inventor/sensors/SoFieldSensor.h \
	$(top_srcdir)/include/Inventor/sensors/SoIdleSensor.h \
	$(top_srcdir)/include/Inventor/sensors/SoNodeSensor.h \
	$(top_srcdir)/include/Inventor/sensors/SoOneShotSensor.h \
	$(top_srcdir)/include/Inventor/sensors/SoPathSensor.h \
	$(top_srcdir)/include/Inventor/sensors/SoSensor.h \
	$(top_srcdir)/include/Inventor/sensors/SoSensorManager.h \
	$(top_srcdir)/include/Inventor/sensors/SoTimerQueueSensor.h \
	$(top_srcdir)/include/Inventor/sensors/SoTimerSensor.h


libsensorsinc_HEADERS = $(ALLINSTHEADERS)
libsensorsLINKHACKinc_HEADERS = $(ALLINSTHEADERS)

## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libsensorsincHEADERS: $(libsensorsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libsensorsincdir)
	@list='$(libsensorsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libsensorsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libsensorsincdir)/$$f; \
	done

uninstall-libsensorsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libsensorsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libsensorsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libsensorsincdir)/$$f; \
	done


install-libsensorsLINKHACKincHEADERS: $(libsensorsLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libsensorsLINKHACKincdir)
	@list='$(libsensorsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libsensorsLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libsensorsLINKHACKincdir)/$$f; \
	done

uninstall-libsensorsLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libsensorsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libsensorsLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libsensorsLINKHACKincdir)/$$f; \
	done
