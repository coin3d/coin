coin.iwz:
	@perl -w $(top_srcdir)/build/geniwz.pl @VERSION@ \
            $(libdir)/coin@COIN_MAJOR_VERSION@.dll \
            $(libdir)/coin@COIN_MAJOR_VERSION@.lib \
            `find $(includedir)/Inventor -name "*.h" | egrep -v -e '/Xt/' -e '/Qt/' -e '/Win/' -e '/Gtk/'` \
            > coin.iwz

ispkg: coin.iwz
	@IsxBuild.exe coin.iwz -E -O
