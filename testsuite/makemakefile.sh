#!/bin/sh
# **************************************************************************

makefile=$1

if test x"$makefile" = x""; then
  echo "Usage: $0 Makefile"
  exit
fi

# **************************************************************************

exec 5>$makefile

srcdir=..

filelist=""
extractlist=""
objlist=""

for file in `cd $srcdir; find src -type f -print`; do
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

  infile=$srcdir/$file

  if test `grep -c "^#if.*COIN_TEST_SUITE" $infile` = 0; then
    continue
  fi

  filelist="$filelist $file"
done

# **************************************************************************

for path in $filelist; do
  token=`echo $path | tr -d '/\\' | sed -e 's/^\.*//g'`

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

srcdir = @srcdir@
top_srcdir = @top_srcdir@
top_builddir = ..
CXX = @CXX@

prefix = @prefix@
OBJEXT = @OBJEXT@
EXEEXT = @EXEEXT@

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/include/Inventor/annex -I$(top_builddir)/include -I$(top_builddir)/include/Inventor/annex
CPPFLAGS = $(INCLUDES) -g @COIN_TESTSUITE_EXTRA_CPPFLAGS@ @COIN_EXTRA_CPPFLAGS@ @COIN_EXTRA_CXXFLAGS@
LDFLAGS = @COIN_TESTSUITE_EXTRA_LDFLAGS@ -L$(top_builddir)/src
LIBS = @COIN_EXTRA_LIBS@

EMPTY =

TEST_SUITE_OBJECTS = \
	TestSuiteInit.$(OBJEXT) \
EODATA

for obj in $objlist; do
  echo >&5 "	$obj \\"
done
echo >&5 "	$(EMPTY)"
echo >&5 ""

echo >&5 "TEST_SUITE_BUILT_FILES = \\"
for extractfile in $extractlist; do
  echo >&5 "	$extractfile \\"
done
echo >&5 "	$(EMPTY)"
echo >&5 ""

cat <<"EODATA" >&5
all: testsuite$(EXEEXT)
	LD_LIBRARY_PATH=$(top_builddir)/src:$$LD_LIBRARY_PATH \
	PATH=$(top_builddir)/src:$$PATH \
	./testsuite --log_level=warning --show_progress=yes \
	  --detect_memory_leaks=no

verbose: testsuite$(EXEEXT)
	LD_LIBRARY_PATH=$(top_builddir)/src:$$LD_LIBRARY_PATH \
	PATH=$(top_builddir)/src:$$PATH \
	./testsuite --log_level=all --show_progress=no \
	  --detect_memory_leaks=no

clean:
	rm -f testsuite$(EXEEXT) *.pdb
	rm -f $(TEST_SUITE_OBJECTS)
	rm -f $(TEST_SUITE_BUILT_FILES)

makefile-update:
	( cd $(srcdir); ./makemakefile.sh Makefile.in )
	( cd $(top_builddir); ./config.status testsuite/Makefile )

testsuite$(EXEEXT): $(TEST_SUITE_OBJECTS) Makefile
	$(CXX) -o $@ $(TEST_SUITE_OBJECTS) $(LDFLAGS) $(LIBS)

TestSuiteInit.$(OBJEXT): $(srcdir)/TestSuiteInit.cpp Makefile
	$(CXX) $(CPPFLAGS) -c $(srcdir)/TestSuiteInit.cpp

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
    echo >&5 "$objectfile: $extractfile Makefile"
    echo >&5 "	\$(CXX) \$(CPPFLAGS) -g -c $extractfile"
    echo >&5 ""
  fi

  e=`echo "$e" | cut -d' ' -f2-`
  s=`echo "$s" | cut -d' ' -f2-`
  o=`echo "$o" | cut -d' ' -f2-`
done

exec 5>/dev/null
