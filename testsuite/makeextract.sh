#!/bin/sh
# **************************************************************************

srcdir=$1
srcdirpath=$2

if test x"$srcdirpath" = x""; then
  echo "Usage: $0 basedir sourcefile"
  exit
fi

# **************************************************************************

token=`echo $srcdirpath | tr -d '/\\' | sed -e 's/^\.*//g'`

case $token in
*.cpp)
  token=`basename $token .cpp`
  class=`basename $srcdirpath .cpp`
  ;;
*.c)
  token=`basename $token .c`
  class=`basename $srcdirpath .c`
  ;;
*.icc)
  token=`basename $token .icc`
  class=`basename $srcdirpath .icc`
  ;;
*.ic)
  token=`basename $token .ic`
  class=`basename $srcdirpath .ic`
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

exec 5>${token}.cpp

cat $srcdir/$srcdirpath | grep "^#include" | head -1 >&5

echo "" >&5
echo "#define BOOST_TEST_NO_LIB 1" >&5
echo "#include <boost/test/unit_test.hpp>" >&5
echo "" >&5
echo "#include <assert.h>" >&5
echo "#include <stdio.h>" >&5
echo "#include <iostream>" >&5
echo "" >&5
echo "#include <TestSuiteUtils.h>" >&5
echo "#include <TestSuiteMisc.h>" >&5
echo "" >&5
echo "using namespace SIM::Coin3D::Coin;" >&5
echo "using namespace SIM::Coin3D::Coin::TestSuite;" >&5
echo "" >&5
echo "BOOST_AUTO_TEST_SUITE(${class}_TestSuite);" >&5
echo "" >&5

# FIXME: to get compile errors to point to the correct place (the original
# source files, not the extract files), we should insert #file-directives
# into the extract files for each COIN_TEST_SUITE block.  That would
# slow this extraction process down a bit though, but should be worth it.
cat $srcdir/$srcdirpath | \
  sed -n -e '/^#if.*COIN_TEST_SUITE/,/^#endif.*COIN_TEST_SUITE/ p' | \
  egrep -v "^#.*COIN_TEST_SUITE" >&5

echo "" >&5
echo "BOOST_AUTO_TEST_SUITE_END();" >&5

exec 5>/dev/null

