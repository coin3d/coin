##$ TEMPLATE Make-CompileSilent()

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

