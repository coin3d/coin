#! /usr/bin/make -f

.PHONY: all

all:
	( cd include/Inventor; $(MAKE) -f templates.make )
	( cd include/Inventor/C/base; $(MAKE) -f templates.make )
	( cd src/base; $(MAKE) -f templates.make )

clean:
	( cd include/Inventor; $(MAKE) -f templates.make clean )
	( cd include/Inventor/C/base; $(MAKE) -f templates.make clean )
	( cd src/base; $(MAKE) -f templates.make clean )


