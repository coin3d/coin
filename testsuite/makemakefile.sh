#!/bin/sh
# **************************************************************************

makefile=$1
filter=""

shift
while test $# -gt 0; do
  case $1 in
  filter=*)
    filter=`echo $1 | cut -d= -f2-`
    ;;
  *)
    ;;
  esac
  shift
done

if test x"$makefile" = x""; then
  echo "Usage: $0 Makefile [filter=...]"
  exit
fi

# **************************************************************************

exec 5>$makefile

srcdir=..

filelist=""
extractlist=""
objlist=""

for file in `cd $srcdir; find src -type f -print | LC_ALL="C" sort`; do
  # filter the find results
  case $file in
  */.svn/*)
    continue
    ;;
  *.cpp | *.c | *.icc | *.ic)
    ;;
  *)
    continue
    ;;
  esac

  case $file in
  *${filter}*)
    infile=$srcdir/$file
    if test `grep -c "^#if.*COIN_TEST_SUITE" $infile` = 0; then
      continue
    fi
    filelist="$filelist $file"
    ;;
  esac
done

# **************************************************************************

for path in $filelist; do
  token=$(echo $path | tr -d '/\\' | sed -e 's/^\.*//g')

  case $token in
  *.cpp)
    token=`basename $token .cpp`
    class=`basename $path .cpp`
    ;;
  *.c)
    token=`basename $token .c`
    class=`basename $path .c`
    ;;
  *.icc)
    token=`basename $token .icc`
    class=`basename $path .icc`
    ;;
  *.ic)
    token=`basename $token .ic`
    class=`basename $path .ic`
    ;;
  esac

  case $token in
  srcInventorWin* | libInventorWin*)
    token=`echo $token | cut -c15-`
    ;;
  srcInventorQt* | libInventorQt*)
    token=`echo $token | cut -c14-`
    ;;
  srcInventor* | libInventor*)
    token=`echo $token | cut -c12-`
    ;;
  src* | lib*)
    token=`echo $token | cut -c4-`
    ;;
  *)
    ;;
  esac

  objlist="$objlist ${token}.\$(OBJEXT)"
  extractlist="$extractlist ${token}.cpp"
done

# **************************************************************************

cat <<"EODATA" >&5

@MACOSX_TRUE@macosx_boost_add = 
@MACOSX_FALSE@macosx_boost_add =

srcdir = @srcdir@
top_srcdir = @top_srcdir@
top_builddir = ..
CXX = @CXX@
LDFLAGS = @LDFLAGS@ $(macosx_boost_add)

prefix = @prefix@
OBJEXT = @OBJEXT@
EXEEXT = @EXEEXT@

@MAC_FRAMEWORK_FALSE@FRAMEWORKLIBADD =
@MAC_FRAMEWORK_TRUE@FRAMEWORKLIBADD = -framework Inventor

TS_INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/include/Inventor/annex -I$(top_builddir)/include -I$(top_builddir)/include/Inventor/annex -I$(top_srcdir)/testsuite
TS_CPPFLAGS = $(TS_INCLUDES) -g @COIN_TESTSUITE_EXTRA_CPPFLAGS@ @COIN_EXTRA_CPPFLAGS@ @COIN_EXTRA_CXXFLAGS@
TS_LDFLAGS = @COIN_TESTSUITE_EXTRA_LDFLAGS@ -L$(top_builddir)/src -L$(top_builddir)/src/.libs $(LDFLAGS)
TS_LIBS = $(FRAMEWORKLIBADD) @COIN_HACKING_LIBDIRS@ @COIN_EXTRA_LIBS@

EMPTY =

TEST_SUITE_OBJECTS = \
	TestSuiteInit.$(OBJEXT) \
	TestSuiteUtils.$(OBJEXT) \
	TestSuiteMisc.$(OBJEXT) \
EODATA

if test x"$filter" = x""; then
cat <<"EODATA" >&5
	StandardTests.$(OBJEXT) \
EODATA
fi

for obj in $objlist; do
  echo >&5 "	$obj \\"
done
echo >&5 "	\$(EMPTY)"
echo >&5 ""

echo >&5 "TEST_SUITE_BUILT_FILES = \\"
for extractfile in $extractlist; do
  echo >&5 "	$extractfile \\"
done
echo >&5 "	\$(EMPTY)"
echo >&5 ""

cat <<"EODATA" >&5
all: testsuite$(EXEEXT)
	LD_LIBRARY_PATH=$(top_builddir)/src/.libs:$$LD_LIBRARY_PATH \
	DYLD_LIBRARY_PATH=$(top_builddir)/src/.libs:$$DYLD_LIBRARY_PATH \
	PATH=$(top_builddir)/src:$$PATH \
	./testsuite --log_level=warning --show_progress=yes \
	  --detect_memory_leaks=0

verbose: testsuite$(EXEEXT)
	LD_LIBRARY_PATH=$(top_builddir)/src/.libs:$$LD_LIBRARY_PATH \
	DYLD_LIBRARY_PATH=$(top_builddir)/src/.libs:$$DYLD_LIBRARY_PATH \
	PATH=$(top_builddir)/src:$$PATH \
	./testsuite --log_level=all --show_progress=no \
	  --detect_memory_leaks=0

debug: testsuite$(EXEEXT)
	optionsfile=/tmp/opts.$$; \
	echo set args --log_level=all --show_progress=no --detect_memory_leaks=0 >$optionsfile; \
	echo run >>$optionsfile; \
	LD_LIBRARY_PATH=$(top_builddir)/src/.libs:$$LD_LIBRARY_PATH \
	DYLD_LIBRARY_PATH=$(top_builddir)/src/.libs:$$DYLD_LIBRARY_PATH \
	PATH=$(top_builddir)/src:$$PATH \
	gdb ./testsuite -x $optionsfile; \
	rm -f $optionsfile;

clean:
	rm -f testsuite$(EXEEXT) *.pdb
	rm -f $(TEST_SUITE_OBJECTS)
	rm -f $(TEST_SUITE_BUILT_FILES)

makefile-update:
	( cd $(srcdir); ./makemakefile.sh Makefile.in filter="$(filter)" )
	( cd $(top_builddir); ./config.status testsuite/Makefile )

testsuite$(EXEEXT): $(TEST_SUITE_OBJECTS)
	$(CXX) -o $@ $(AM_LDFLAGS) $(TEST_SUITE_OBJECTS) $(TS_LDFLAGS) $(LIBS) $(TS_LIBS)

TestSuiteInit.$(OBJEXT): $(srcdir)/TestSuiteInit.cpp $(srcdir)/TestSuiteUtils.h
	$(CXX) $(CPPFLAGS) $(TS_CPPFLAGS) -c $(srcdir)/TestSuiteInit.cpp

TestSuiteUtils.$(OBJEXT): $(srcdir)/TestSuiteUtils.cpp $(srcdir)/TestSuiteUtils.h
	$(CXX) $(CPPFLAGS) $(TS_CPPFLAGS) -c $(srcdir)/TestSuiteUtils.cpp

TestSuiteMisc.$(OBJEXT): $(srcdir)/TestSuiteMisc.cpp $(srcdir)/TestSuiteMisc.h
	$(CXX) $(CPPFLAGS) $(TS_CPPFLAGS) -c $(srcdir)/TestSuiteMisc.cpp

StandardTests.$(OBJEXT): $(srcdir)/StandardTests.cpp $(srcdir)/TestSuiteUtils.h
	$(CXX) $(CPPFLAGS) $(TS_CPPFLAGS) -c $(srcdir)/StandardTests.cpp

EODATA

e="$extractlist ";
s="$filelist ";
o="$objlist ";
while test x"$e" != x""; do
  extractfile=`echo "$e" | cut -d' ' -f1`
  sourcefile=`echo "$s" | cut -d' ' -f1`
  objectfile=`echo "$o" | cut -d' ' -f1`

  if test x"$extractfile" != x""; then
    echo >&5 "$extractfile: \$(top_srcdir)/$sourcefile \$(srcdir)/makeextract.sh"
    echo >&5 "	\$(srcdir)/makeextract.sh \$(top_srcdir) $sourcefile"
    echo >&5 ""
    echo >&5 "$objectfile: $extractfile \$(srcdir)/TestSuiteUtils.h \$(srcdir)/TestSuiteMisc.h"
    echo >&5 "	\$(CXX) \$(CPPFLAGS) \$(TS_CPPFLAGS) -g -c $extractfile"
    echo >&5 ""
  fi

  e=`echo "$e" | cut -d' ' -f2-`
  s=`echo "$s" | cut -d' ' -f2-`
  o=`echo "$o" | cut -d' ' -f2-`
done

exec 5>/dev/null
