##$ TEMPLATE Make-LibtoolSilent()

# This macro contains fixes to avoid the default flush of output from
# the automake generated rules. 19991201 mortene. 

LIBTOOL = @LIBTOOL@ --silent
