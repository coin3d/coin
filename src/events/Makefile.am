## Process this file with automake to generate Makefile.in.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libeventsLINKHACK.la
else
noinst_LTLIBRARIES = libevents.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
	SoButtonEvent.cpp \
	SoButtonEvent.h \
	SoEvent.cpp \
	SoEvent.h \
	SoKeyboardEvent.cpp \
	SoKeyboardEvent.h \
	SoLocation2Event.cpp \
	SoLocation2Event.h \
	SoMotion3Event.cpp \
	SoMotion3Event.h \
	SoMouseButtonEvent.cpp \
	SoMouseButtonEvent.h \
	SoSpaceballButtonEvent.cpp \
	SoSpaceballButtonEvent.h

libevents_la_SOURCES = $(ALLSOURCES)
libeventsLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libeventsincdir = $(includedir)/Inventor/events
libeventsLINKHACKincdir = $(includedir)/Inventor/events

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/events/SoSubEvent.h \
	$(top_srcdir)/include/Inventor/events/SoButtonEvent.h \
	$(top_srcdir)/include/Inventor/events/SoEvent.h \
	$(top_srcdir)/include/Inventor/events/SoEvents.h \
	$(top_srcdir)/include/Inventor/events/SoKeyboardEvent.h \
	$(top_srcdir)/include/Inventor/events/SoLocation2Event.h \
	$(top_srcdir)/include/Inventor/events/SoMotion3Event.h \
	$(top_srcdir)/include/Inventor/events/SoMouseButtonEvent.h \
	$(top_srcdir)/include/Inventor/events/SoSpaceballButtonEvent.h


libeventsinc_HEADERS = $(ALLINSTHEADERS)
libeventsLINKHACKinc_HEADERS = $(ALLINSTHEADERS)

## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libeventsincHEADERS: $(libeventsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libeventsincdir)
	@list='$(libeventsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libeventsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libeventsincdir)/$$f; \
	done

uninstall-libeventsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libeventsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libeventsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libeventsincdir)/$$f; \
	done


install-libeventsLINKHACKincHEADERS: $(libeventsLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libeventsLINKHACKincdir)
	@list='$(libeventsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libeventsLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libeventsLINKHACKincdir)/$$f; \
	done

uninstall-libeventsLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libeventsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libeventsLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libeventsLINKHACKincdir)/$$f; \
	done
