##$ TEMPLATE Make-LinkSilent(-xxx-)

# This macro contains fixes to avoid the default flush of output from
# the automake generated rules. 19991201 mortene. 

lib-xxx-LINKHACK.la: $(lib-xxx-LINKHACK_la_OBJECTS) $(lib-xxx-LINKHACK_la_DEPENDENCIES)
	@echo Linking lib-xxx-LINKHACK.la
	@$(CXXLINK) -rpath $(libdir) $(lib-xxx-LINKHACK_la_LDFLAGS) $(lib-xxx-LINKHACK_la_OBJECTS) $(lib-xxx-LINKHACK_la_LIBADD) $(LIBS)

lib-xxx-.la: $(lib-xxx-_la_OBJECTS) $(lib-xxx-_la_DEPENDENCIES)
	@echo Linking lib-xxx-.la
	@$(CXXLINK)  $(lib-xxx-_la_LDFLAGS) $(lib-xxx-_la_OBJECTS) $(lib-xxx-_la_LIBADD) $(LIBS)
