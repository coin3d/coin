## Process this file with automake to generate Makefile.in.

## FIXME: this file should eventually be rendered obsolete, as the
## inttypes.h should be moved into SbBasic.h.in, so we can decide and
## set the integer types from configure. Being compatible with TGS/SGI
## Inventor is _not_ a good idea here, as they expose the inttypes.h
## publicly, with the danger of making it "override" the inttypes.h
## file on the system. 19990912 mortene.
##
## I think the above comment can be ignored/removed. 20010712 larsa.

BUILT_SOURCES = \
	inttypes.h

inttypes.h: $(srcdir)/inttypes.h.in
	@( cd $(top_builddir); $(MAKE) include/Inventor/system/inttypes.h )

# header installation information

systemincdir = $(includedir)/Inventor/system
systeminc_HEADERS = \
	inttypes.h

