## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = libsensors.la

INCLUDES = -I$(top_srcdir)/include/

## All source files used when building this submodule, including all
## headers.

libsensors_la_SOURCES = \
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

## Installation information.

libsensorsincdir = $(includedir)/Inventor/sensors

libsensorsinc_HEADERS = \
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
