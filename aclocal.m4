dnl aclocal.m4 generated automatically by aclocal 1.4a-SIM-20000531

dnl Copyright (C) 1994, 1995-9, 2000 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

# Usage:
#   SIM_COMPILER_INLINE_FOR( [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND ]])
#
# Description:
#   See if the compiler supports for(;;){} loops inside inlined
#   constructors.
#
#   This smokes out the useless HPUX 10.20 CC compiler.
#

AC_DEFUN([SIM_COMPILER_INLINE_FOR], [
AC_PREREQ([2.14])

AC_CACHE_CHECK(
  [whether the C++ compiler handles inlined loops],
  sim_cv_c_inlinefor,
  [cat > sim_ac_test.h <<EOF
class TestClass {
public:
  TestClass(int);
};

inline TestClass::TestClass(int) { for (int i=0; i<1; i++) i=0; }
EOF
  AC_TRY_COMPILE([#include "sim_ac_test.h"],
                 [TestClass t(0);],
                 [sim_cv_c_inlinefor=yes],
                 [sim_cv_c_inlinefor=no])
])

rm -rf sim_ac_test.h

if test x"$sim_cv_c_inlinefor" = x"yes"; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
])


# Usage:
#   SIM_AC_CHECK_VAR_FUNCTIONNAME
#
# Side-Effects:
#   config.h:
#     HAVE_VAR___PRETTY_FUNCTION__   (1 if exists)
#     HAVE_VAR___FUNCTION__          (always 0 if __PRETTY_FUNCTION__ exists)
#
# Authors:
#   Lars J. Aas <larsa@sim.no>
#

AC_DEFUN([SIM_AC_CHECK_VAR_FUNCTIONNAME], [
AC_CACHE_CHECK([for function name variable],
  sim_cv_var_functionname, [
  AC_TRY_COMPILE(
    [#include <stdio.h>],
    [(void)printf("%s\n",__PRETTY_FUNCTION__)],
    [sim_cv_var_functionname=__PRETTY_FUNCTION__],
    [sim_cv_var_functionname=none])
  if test x"$sim_cv_pretty_function" = x"none"; then
    AC_TRY_COMPILE(
      [#include <stdio.h>],
      [(void)printf("%s\n",__FUNCTION__)],
      [sim_cv_var_functionname=__FUNCTION__],
      [sim_cv_var_functionname=none])
  fi])

if test x"$sim_cv_var_functionname" = x"__PRETTY_FUNCTION__"; then
  AC_DEFINE([HAVE_VAR___PRETTY_FUNCTION__], 1,
    [Define this to true if the __PRETTY_FUNCTION__ variable contains the current function name])
fi

if test x"$sim_cv_var_functionname" = x"__FUNCTION__"; then
  AC_DEFINE([HAVE_VAR___FUNCTION__], 1,
    [Define this to true if the __FUNCTION__ variable contains the current function name])
fi
])


# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 2

# AC_PROVIDE_IFELSE(MACRO-NAME, IF-PROVIDED, IF-NOT-PROVIDED)
# -----------------------------------------------------------
# If MACRO-NAME is provided do IF-PROVIDED, else IF-NOT-PROVIDED.
# The purpose of this macro is to provide the user with a means to
# check macros which are provided without letting her know how the
# information is coded.
# If this macro is not defined by Autoconf, define it here.
ifdef([AC_PROVIDE_IFELSE],
      [],
      [define([AC_PROVIDE_IFELSE],
              [ifdef([AC_PROVIDE_$1],
                     [$2], [$3])])])


# AM_INIT_AUTOMAKE(PACKAGE,VERSION, [NO-DEFINE])
# ----------------------------------------------
AC_DEFUN(AM_INIT_AUTOMAKE,
[dnl We require 2.13 because we rely on SHELL being computed by configure.
AC_PREREQ([2.13])dnl
AC_REQUIRE([AC_PROG_INSTALL])dnl
# test to see if srcdir already configured
if test "`CDPATH=: && cd $srcdir && pwd`" != "`pwd`" &&
   test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi

# Define the identity of the package.
PACKAGE=$1
AC_SUBST(PACKAGE)dnl
VERSION=$2
AC_SUBST(VERSION)dnl
ifelse([$3],,
[AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package])])

# Some tools Automake needs.
AC_REQUIRE([AM_SANITY_CHECK])dnl
AC_REQUIRE([AC_ARG_PROGRAM])dnl
AM_MISSING_PROG(ACLOCAL, aclocal)
AM_MISSING_PROG(AUTOCONF, autoconf)
AM_MISSING_PROG(AUTOMAKE, automake)
AM_MISSING_PROG(AUTOHEADER, autoheader)
AM_MISSING_PROG(MAKEINFO, makeinfo)
AM_MISSING_PROG(AMTAR, tar)
AM_MISSING_INSTALL_SH
dnl We need awk for the "check" target.  The system "awk" is bad on
dnl some platforms.
AC_REQUIRE([AC_PROG_AWK])dnl
AC_REQUIRE([AC_PROG_MAKE_SET])dnl
AC_REQUIRE([AM_DEP_TRACK])dnl
AC_REQUIRE([AM_SET_DEPDIR])dnl
AC_PROVIDE_IFELSE([AC_PROG_CC],
                  [AM_DEPENDENCIES(CC)],
                  [define([AC_PROG_CC],
                          defn([AC_PROG_CC])[AM_DEPENDENCIES(CC)])])dnl
AC_PROVIDE_IFELSE([AC_PROG_CXX],
                  [AM_DEPENDENCIES(CXX)],
                  [define([AC_PROG_CXX],
                          defn([AC_PROG_CXX])[AM_DEPENDENCIES(CXX)])])dnl
])

#
# Check to make sure that the build environment is sane.
#

AC_DEFUN(AM_SANITY_CHECK,
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "[$]*" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   if test "[$]*" != "X $srcdir/configure conftestfile" \
      && test "[$]*" != "X conftestfile $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
rm -f conftest*
AC_MSG_RESULT(yes)])

dnl AM_MISSING_PROG(NAME, PROGRAM)
AC_DEFUN(AM_MISSING_PROG, [
AC_REQUIRE([AM_MISSING_HAS_RUN])
$1=${$1-"${am_missing_run}$2"}
AC_SUBST($1)])

dnl Like AM_MISSING_PROG, but only looks for install-sh.
dnl AM_MISSING_INSTALL_SH()
AC_DEFUN(AM_MISSING_INSTALL_SH, [
AC_REQUIRE([AM_MISSING_HAS_RUN])
if test -z "$install_sh"; then
   install_sh="$ac_aux_dir/install-sh"
   test -f "$install_sh" || install_sh="$ac_aux_dir/install.sh"
   test -f "$install_sh" || install_sh="${am_missing_run}${ac_auxdir}/install-sh"
   dnl FIXME: an evil hack: we remove the SHELL invocation from
   dnl install_sh because automake adds it back in.  Sigh.
   install_sh="`echo $install_sh | sed -e 's/\${SHELL}//'`"
fi
AC_SUBST(install_sh)])

dnl AM_MISSING_HAS_RUN.
dnl Define MISSING if not defined so far and test if it supports --run.
dnl If it does, set am_missing_run to use it, otherwise, to nothing.
AC_DEFUN([AM_MISSING_HAS_RUN], [
test x"${MISSING+set}" = xset || \
  MISSING="\${SHELL} `CDPATH=: && cd $ac_aux_dir && pwd`/missing"
dnl Use eval to expand $SHELL
if eval "$MISSING --run :"; then
  am_missing_run="$MISSING --run "
else
  am_missing_run=
  am_backtick='`'
  AC_MSG_WARN([${am_backtick}missing' script is too old or missing])
fi
])

dnl See how the compiler implements dependency checking.
dnl Usage:
dnl AM_DEPENDENCIES(NAME)
dnl NAME is "CC", "CXX" or "OBJC".

dnl We try a few techniques and use that to set a single cache variable.

AC_DEFUN(AM_DEPENDENCIES,[
AC_REQUIRE([AM_SET_DEPDIR])
AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])
ifelse([$1],CC,[
AC_REQUIRE([AC_PROG_CC])
AC_REQUIRE([AC_PROG_CPP])
depcc="$CC"
depcpp="$CPP"],[$1],CXX,[
AC_REQUIRE([AC_PROG_CXX])
AC_REQUIRE([AC_PROG_CXXCPP])
depcc="$CXX"
depcpp="$CXXCPP"],[$1],OBJC,[
am_cv_OBJC_dependencies_compiler_type=gcc],[
AC_REQUIRE([AC_PROG_][$1])
depcc="$[$1]"
depcpp=""])
AC_MSG_CHECKING([dependency style of $depcc])
AC_CACHE_VAL(am_cv_[$1]_dependencies_compiler_type,[
if test -z "$AMDEP"; then
  echo '#include "conftest.h"' > conftest.c
  echo 'int i;' > conftest.h

  am_cv_[$1]_dependencies_compiler_type=none
  for depmode in `sed -n 's/^#*\([a-zA-Z0-9]*\))$/\1/p' < "$am_depcomp"`; do
    case "$depmode" in
    nosideeffect)
      # after this tag, mechanisms are not by side-effect, so they'll
      # only be used when explicitly requested
      if test "x$enable_dependency_tracking" = xyes; then
	continue
      else
	break
      fi
      ;;
    none) break ;;
    esac
    if depmode="$depmode" \
       source=conftest.c object=conftest.o \
       depfile=conftest.Po tmpdepfile=conftest.TPo \
       $SHELL $am_depcomp $depcc -c conftest.c 2>/dev/null &&
       grep conftest.h conftest.Po > /dev/null 2>&1; then
      am_cv_[$1]_dependencies_compiler_type="$depmode"
      break
    fi
  done

  rm -f conftest.*
else
  am_cv_[$1]_dependencies_compiler_type=none
fi
])
AC_MSG_RESULT($am_cv_[$1]_dependencies_compiler_type)
[$1]DEPMODE="depmode=$am_cv_[$1]_dependencies_compiler_type"
AC_SUBST([$1]DEPMODE)
])

dnl Choose a directory name for dependency files.
dnl This macro is AC_REQUIREd in AM_DEPENDENCIES

AC_DEFUN(AM_SET_DEPDIR,[
if test -d .deps || mkdir .deps 2> /dev/null || test -d .deps; then
  DEPDIR=.deps
else
  DEPDIR=_deps
fi
AC_SUBST(DEPDIR)
])

AC_DEFUN(AM_DEP_TRACK,[
AC_ARG_ENABLE(dependency-tracking,
[  --disable-dependency-tracking Speeds up one-time builds
  --enable-dependency-tracking  Do not reject slow dependency extractors])
if test "x$enable_dependency_tracking" = xno; then
  AMDEP="#"
else
  am_depcomp="$ac_aux_dir/depcomp"
  if test ! -f "$am_depcomp"; then
    AMDEP="#"
  else
    AMDEP=
  fi
fi
AC_SUBST(AMDEP)
if test -z "$AMDEP"; then
  AMDEPBACKSLASH='\'
else
  AMDEPBACKSLASH=
fi
pushdef([subst], defn([AC_SUBST]))
subst(AMDEPBACKSLASH)
popdef([subst])
])

dnl Generate code to set up dependency tracking.
dnl This macro should only be invoked once -- use via AC_REQUIRE.
dnl Usage:
dnl AM_OUTPUT_DEPENDENCY_COMMANDS

dnl
dnl This code is only required when automatic dependency tracking
dnl is enabled.  FIXME.  This creates each `.P' file that we will
dnl need in order to bootstrap the dependency handling code.
AC_DEFUN(AM_OUTPUT_DEPENDENCY_COMMANDS,[
AC_OUTPUT_COMMANDS([
test x"$AMDEP" != x"" ||
for mf in $CONFIG_FILES; do
  case "$mf" in
  Makefile) dirpart=.;;
  */Makefile) dirpart=`echo "$mf" | sed -e 's|/[^/]*$||'`;;
  *) continue;;
  esac
  grep '^DEP_FILES *= *[^ #]' < "$mf" > /dev/null || continue
  # Extract the definition of DEP_FILES from the Makefile without
  # running `make'.
  DEPDIR=`tr -d "
" < "$mf" | sed -n -e '/^DEPDIR = / s///p'`
  test -z "$DEPDIR" && continue
  # When using ansi2knr, U may be empty or an underscore; expand it
  U=`sed -n -e '/^U = / s///p' < "$mf"`
  test -d "$dirpart/$DEPDIR" || mkdir "$dirpart/$DEPDIR"
  # We invoke sed twice because it is the simplest approach to
  # changing $(DEPDIR) to its actual value in the expansion.
  for file in `tr -d "
" < "$mf" | sed -n -e '
    /^DEP_FILES = .*\\\\$/ {
      s/^DEP_FILES = //
      :loop
	s/\\\\$//
	p
	n
	/\\\\$/ b loop
      p
    }
    /^DEP_FILES = / s/^DEP_FILES = //p' | \
       sed -e 's/\$(DEPDIR)/'"$DEPDIR"'/g' -e 's/\$U/'"$U"'/g'`; do
    # Make sure the directory exists.
    test -f "$dirpart/$file" && continue
    fdir=`echo "$file" | sed -e 's|/[^/]*$||'`
    $ac_aux_dir/mkinstalldirs "$dirpart/$fdir" > /dev/null 2>&1
    # echo "creating $dirpart/$file"
    echo '# dummy' > "$dirpart/$file"
  done
done
], [AMDEP="$AMDEP"
ac_aux_dir="$ac_aux_dir"])])

# Like AC_CONFIG_HEADER, but automatically create stamp file.

AC_DEFUN(AM_CONFIG_HEADER,
[AC_PREREQ([2.12])
AC_CONFIG_HEADER([$1])
dnl When config.status generates a header, we must update the stamp-h file.
dnl This file resides in the same directory as the config header
dnl that is generated.  We must strip everything past the first ":",
dnl and everything past the last "/".
AC_OUTPUT_COMMANDS(changequote(<<,>>)dnl
ifelse(patsubst(<<$1>>, <<[^ ]>>, <<>>), <<>>,
<<test -z "<<$>>CONFIG_HEADERS" || echo timestamp > patsubst(<<$1>>, <<^\([^:]*/\)?.*>>, <<\1>>)stamp-h<<>>dnl>>,
<<am_indx=1
for am_file in <<$1>>; do
  case " <<$>>CONFIG_HEADERS " in
  *" <<$>>am_file "*<<)>>
    echo timestamp > `echo <<$>>am_file | sed -e 's%:.*%%' -e 's%[^/]*$%%'`stamp-h$am_indx
    ;;
  esac
  am_indx=`expr "<<$>>am_indx" + 1`
done<<>>dnl>>)
changequote([,]))])


# serial 40 AC_PROG_LIBTOOL
AC_DEFUN(AC_PROG_LIBTOOL,
[AC_REQUIRE([AC_LIBTOOL_SETUP])dnl

# Save cache, so that ltconfig can load it
AC_CACHE_SAVE

# Actually configure libtool.  ac_aux_dir is where install-sh is found.
CC="$CC" CFLAGS="$CFLAGS" CPPFLAGS="$CPPFLAGS" \
LD="$LD" LDFLAGS="$LDFLAGS" LIBS="$LIBS" \
LN_S="$LN_S" NM="$NM" RANLIB="$RANLIB" \
DLLTOOL="$DLLTOOL" AS="$AS" OBJDUMP="$OBJDUMP" \
${CONFIG_SHELL-/bin/sh} $ac_aux_dir/ltconfig --no-reexec \
$libtool_flags --no-verify $ac_aux_dir/ltmain.sh $lt_target \
|| AC_MSG_ERROR([libtool configure failed])

# Reload cache, that may have been modified by ltconfig
AC_CACHE_LOAD

# This can be used to rebuild libtool when needed
LIBTOOL_DEPS="$ac_aux_dir/ltconfig $ac_aux_dir/ltmain.sh"

# Always use our own libtool.
LIBTOOL='$(SHELL) $(top_builddir)/libtool'
AC_SUBST(LIBTOOL)dnl

# Redirect the config.log output again, so that the ltconfig log is not
# clobbered by the next message.
exec 5>>./config.log
])

AC_DEFUN(AC_LIBTOOL_SETUP,
[AC_PREREQ(2.13)dnl
AC_REQUIRE([AC_ENABLE_SHARED])dnl
AC_REQUIRE([AC_ENABLE_STATIC])dnl
AC_REQUIRE([AC_ENABLE_FAST_INSTALL])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
AC_REQUIRE([AC_PROG_RANLIB])dnl
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_LD])dnl
AC_REQUIRE([AC_PROG_NM])dnl
AC_REQUIRE([AC_PROG_LN_S])dnl
dnl

case "$target" in
NONE) lt_target="$host" ;;
*) lt_target="$target" ;;
esac

# Check for any special flags to pass to ltconfig.
libtool_flags="--cache-file=$cache_file"
test "$enable_shared" = no && libtool_flags="$libtool_flags --disable-shared"
test "$enable_static" = no && libtool_flags="$libtool_flags --disable-static"
test "$enable_fast_install" = no && libtool_flags="$libtool_flags --disable-fast-install"
test "$ac_cv_prog_gcc" = yes && libtool_flags="$libtool_flags --with-gcc"
test "$ac_cv_prog_gnu_ld" = yes && libtool_flags="$libtool_flags --with-gnu-ld"
ifdef([AC_PROVIDE_AC_LIBTOOL_DLOPEN],
[libtool_flags="$libtool_flags --enable-dlopen"])
ifdef([AC_PROVIDE_AC_LIBTOOL_WIN32_DLL],
[libtool_flags="$libtool_flags --enable-win32-dll"])
AC_ARG_ENABLE(libtool-lock,
  [  --disable-libtool-lock  avoid locking (might break parallel builds)])
test "x$enable_libtool_lock" = xno && libtool_flags="$libtool_flags --disable-lock"
test x"$silent" = xyes && libtool_flags="$libtool_flags --silent"

# Some flags need to be propagated to the compiler or linker for good
# libtool support.
case "$lt_target" in
*-*-irix6*)
  # Find out which ABI we are using.
  echo '[#]line __oline__ "configure"' > conftest.$ac_ext
  if AC_TRY_EVAL(ac_compile); then
    case "`/usr/bin/file conftest.o`" in
    *32-bit*)
      LD="${LD-ld} -32"
      ;;
    *N32*)
      LD="${LD-ld} -n32"
      ;;
    *64-bit*)
      LD="${LD-ld} -64"
      ;;
    esac
  fi
  rm -rf conftest*
  ;;

*-*-sco3.2v5*)
  # On SCO OpenServer 5, we need -belf to get full-featured binaries.
  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -belf"
  AC_CACHE_CHECK([whether the C compiler needs -belf], lt_cv_cc_needs_belf,
    [AC_TRY_LINK([],[],[lt_cv_cc_needs_belf=yes],[lt_cv_cc_needs_belf=no])])
  if test x"$lt_cv_cc_needs_belf" != x"yes"; then
    # this is probably gcc 2.8.0, egcs 1.0 or newer; no need for -belf
    CFLAGS="$SAVE_CFLAGS"
  fi
  ;;

ifdef([AC_PROVIDE_AC_LIBTOOL_WIN32_DLL],
[*-*-cygwin* | *-*-mingw*)
  AC_CHECK_TOOL(DLLTOOL, dlltool, false)
  AC_CHECK_TOOL(AS, as, false)
  AC_CHECK_TOOL(OBJDUMP, objdump, false)
  ;;
])
esac
])

# AC_LIBTOOL_DLOPEN - enable checks for dlopen support
AC_DEFUN(AC_LIBTOOL_DLOPEN, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])])

# AC_LIBTOOL_WIN32_DLL - declare package support for building win32 dll's
AC_DEFUN(AC_LIBTOOL_WIN32_DLL, [AC_BEFORE([$0], [AC_LIBTOOL_SETUP])])

# AC_ENABLE_SHARED - implement the --enable-shared flag
# Usage: AC_ENABLE_SHARED[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_SHARED, [dnl
define([AC_ENABLE_SHARED_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(shared,
changequote(<<, >>)dnl
<<  --enable-shared[=PKGS]  build shared libraries [default=>>AC_ENABLE_SHARED_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_shared=yes ;;
no) enable_shared=no ;;
*)
  enable_shared=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_shared=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_shared=AC_ENABLE_SHARED_DEFAULT)dnl
])

# AC_DISABLE_SHARED - set the default shared flag to --disable-shared
AC_DEFUN(AC_DISABLE_SHARED, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_SHARED(no)])

# AC_ENABLE_STATIC - implement the --enable-static flag
# Usage: AC_ENABLE_STATIC[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_STATIC, [dnl
define([AC_ENABLE_STATIC_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(static,
changequote(<<, >>)dnl
<<  --enable-static[=PKGS]  build static libraries [default=>>AC_ENABLE_STATIC_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_static=yes ;;
no) enable_static=no ;;
*)
  enable_static=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_static=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_static=AC_ENABLE_STATIC_DEFAULT)dnl
])

# AC_DISABLE_STATIC - set the default static flag to --disable-static
AC_DEFUN(AC_DISABLE_STATIC, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_STATIC(no)])


# AC_ENABLE_FAST_INSTALL - implement the --enable-fast-install flag
# Usage: AC_ENABLE_FAST_INSTALL[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_FAST_INSTALL, [dnl
define([AC_ENABLE_FAST_INSTALL_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(fast-install,
changequote(<<, >>)dnl
<<  --enable-fast-install[=PKGS]  optimize for fast installation [default=>>AC_ENABLE_FAST_INSTALL_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_fast_install=yes ;;
no) enable_fast_install=no ;;
*)
  enable_fast_install=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_fast_install=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_fast_install=AC_ENABLE_FAST_INSTALL_DEFAULT)dnl
])

# AC_ENABLE_FAST_INSTALL - set the default to --disable-fast-install
AC_DEFUN(AC_DISABLE_FAST_INSTALL, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_FAST_INSTALL(no)])

# AC_PROG_LD - find the path to the GNU or non-GNU linker
AC_DEFUN(AC_PROG_LD,
[AC_ARG_WITH(gnu-ld,
[  --with-gnu-ld           assume the C compiler uses GNU ld [default=no]],
test "$withval" = no || with_gnu_ld=yes, with_gnu_ld=no)
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
ac_prog=ld
if test "$ac_cv_prog_gcc" = yes; then
  # Check if gcc -print-prog-name=ld gives a path.
  AC_MSG_CHECKING([for ld used by GCC])
  ac_prog=`($CC -print-prog-name=ld) 2>&5`
  case "$ac_prog" in
    # Accept absolute paths.
changequote(,)dnl
    [\\/]* | [A-Za-z]:[\\/]*)
      re_direlt='/[^/][^/]*/\.\./'
changequote([,])dnl
      # Canonicalize the path of ld
      ac_prog=`echo $ac_prog| sed 's%\\\\%/%g'`
      while echo $ac_prog | grep "$re_direlt" > /dev/null 2>&1; do
	ac_prog=`echo $ac_prog| sed "s%$re_direlt%/%"`
      done
      test -z "$LD" && LD="$ac_prog"
      ;;
  "")
    # If it fails, then pretend we aren't using GCC.
    ac_prog=ld
    ;;
  *)
    # If it is relative, then search for the first ld in PATH.
    with_gnu_ld=unknown
    ;;
  esac
elif test "$with_gnu_ld" = yes; then
  AC_MSG_CHECKING([for GNU ld])
else
  AC_MSG_CHECKING([for non-GNU ld])
fi
AC_CACHE_VAL(ac_cv_path_LD,
[if test -z "$LD"; then
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}${PATH_SEPARATOR-:}"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/$ac_prog" || test -f "$ac_dir/$ac_prog$ac_exeext"; then
      ac_cv_path_LD="$ac_dir/$ac_prog"
      # Check to see if the program is GNU ld.  I'd rather use --version,
      # but apparently some GNU ld's only accept -v.
      # Break only if it was the GNU/non-GNU ld that we prefer.
      if "$ac_cv_path_LD" -v 2>&1 < /dev/null | egrep '(GNU|with BFD)' > /dev/null; then
	test "$with_gnu_ld" != no && break
      else
	test "$with_gnu_ld" != yes && break
      fi
    fi
  done
  IFS="$ac_save_ifs"
else
  ac_cv_path_LD="$LD" # Let the user override the test with a path.
fi])
LD="$ac_cv_path_LD"
if test -n "$LD"; then
  AC_MSG_RESULT($LD)
else
  AC_MSG_RESULT(no)
fi
test -z "$LD" && AC_MSG_ERROR([no acceptable ld found in \$PATH])
AC_PROG_LD_GNU
])

AC_DEFUN(AC_PROG_LD_GNU,
[AC_CACHE_CHECK([if the linker ($LD) is GNU ld], ac_cv_prog_gnu_ld,
[# I'd rather use --version here, but apparently some GNU ld's only accept -v.
if $LD -v 2>&1 </dev/null | egrep '(GNU|with BFD)' 1>&5; then
  ac_cv_prog_gnu_ld=yes
else
  ac_cv_prog_gnu_ld=no
fi])
])

# AC_PROG_NM - find the path to a BSD-compatible name lister
AC_DEFUN(AC_PROG_NM,
[AC_MSG_CHECKING([for BSD-compatible nm])
AC_CACHE_VAL(ac_cv_path_NM,
[if test -n "$NM"; then
  # Let the user override the test.
  ac_cv_path_NM="$NM"
else
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}${PATH_SEPARATOR-:}"
  for ac_dir in $PATH /usr/ccs/bin /usr/ucb /bin; do
    test -z "$ac_dir" && ac_dir=.
    if test -f $ac_dir/nm || test -f $ac_dir/nm$ac_exeext ; then
      # Check to see if the nm accepts a BSD-compat flag.
      # Adding the `sed 1q' prevents false positives on HP-UX, which says:
      #   nm: unknown option "B" ignored
      if ($ac_dir/nm -B /dev/null 2>&1 | sed '1q'; exit 0) | egrep /dev/null >/dev/null; then
	ac_cv_path_NM="$ac_dir/nm -B"
	break
      elif ($ac_dir/nm -p /dev/null 2>&1 | sed '1q'; exit 0) | egrep /dev/null >/dev/null; then
	ac_cv_path_NM="$ac_dir/nm -p"
	break
      else
	ac_cv_path_NM=${ac_cv_path_NM="$ac_dir/nm"} # keep the first match, but
	continue # so that we can try to find one that supports BSD flags
      fi
    fi
  done
  IFS="$ac_save_ifs"
  test -z "$ac_cv_path_NM" && ac_cv_path_NM=nm
fi])
NM="$ac_cv_path_NM"
AC_MSG_RESULT([$NM])
])

# AC_CHECK_LIBM - check for math library
AC_DEFUN(AC_CHECK_LIBM,
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
LIBM=
case "$lt_target" in
*-*-beos* | *-*-cygwin*)
  # These system don't have libm
  ;;
*-ncr-sysv4.3*)
  AC_CHECK_LIB(mw, _mwvalidcheckl, LIBM="-lmw")
  AC_CHECK_LIB(m, main, LIBM="$LIBM -lm")
  ;;
*)
  AC_CHECK_LIB(m, main, LIBM="-lm")
  ;;
esac
])

# AC_LIBLTDL_CONVENIENCE[(dir)] - sets LIBLTDL to the link flags for
# the libltdl convenience library and INCLTDL to the include flags for
# the libltdl header and adds --enable-ltdl-convenience to the
# configure arguments.  Note that LIBLTDL and INCLTDL are not
# AC_SUBSTed, nor is AC_CONFIG_SUBDIRS called.  If DIR is not
# provided, it is assumed to be `libltdl'.  LIBLTDL will be prefixed
# with '${top_builddir}/' and INCLTDL will be prefixed with
# '${top_srcdir}/' (note the single quotes!).  If your package is not
# flat and you're not using automake, define top_builddir and
# top_srcdir appropriately in the Makefiles.
AC_DEFUN(AC_LIBLTDL_CONVENIENCE, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
  case "$enable_ltdl_convenience" in
  no) AC_MSG_ERROR([this package needs a convenience libltdl]) ;;
  "") enable_ltdl_convenience=yes
      ac_configure_args="$ac_configure_args --enable-ltdl-convenience" ;;
  esac
  LIBLTDL='${top_builddir}/'ifelse($#,1,[$1],['libltdl'])/libltdlc.la
  INCLTDL='-I${top_srcdir}/'ifelse($#,1,[$1],['libltdl'])
])

# AC_LIBLTDL_INSTALLABLE[(dir)] - sets LIBLTDL to the link flags for
# the libltdl installable library and INCLTDL to the include flags for
# the libltdl header and adds --enable-ltdl-install to the configure
# arguments.  Note that LIBLTDL and INCLTDL are not AC_SUBSTed, nor is
# AC_CONFIG_SUBDIRS called.  If DIR is not provided and an installed
# libltdl is not found, it is assumed to be `libltdl'.  LIBLTDL will
# be prefixed with '${top_builddir}/' and INCLTDL will be prefixed
# with '${top_srcdir}/' (note the single quotes!).  If your package is
# not flat and you're not using automake, define top_builddir and
# top_srcdir appropriately in the Makefiles.
# In the future, this macro may have to be called after AC_PROG_LIBTOOL.
AC_DEFUN(AC_LIBLTDL_INSTALLABLE, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
  AC_CHECK_LIB(ltdl, main,
  [test x"$enable_ltdl_install" != xyes && enable_ltdl_install=no],
  [if test x"$enable_ltdl_install" = xno; then
     AC_MSG_WARN([libltdl not installed, but installation disabled])
   else
     enable_ltdl_install=yes
   fi
  ])
  if test x"$enable_ltdl_install" = x"yes"; then
    ac_configure_args="$ac_configure_args --enable-ltdl-install"
    LIBLTDL='${top_builddir}/'ifelse($#,1,[$1],['libltdl'])/libltdl.la
    INCLTDL='-I${top_srcdir}/'ifelse($#,1,[$1],['libltdl'])
  else
    ac_configure_args="$ac_configure_args --enable-ltdl-install=no"
    LIBLTDL="-lltdl"
    INCLTDL=
  fi
])

dnl old names
AC_DEFUN(AM_PROG_LIBTOOL, [indir([AC_PROG_LIBTOOL])])dnl
AC_DEFUN(AM_ENABLE_SHARED, [indir([AC_ENABLE_SHARED], $@)])dnl
AC_DEFUN(AM_ENABLE_STATIC, [indir([AC_ENABLE_STATIC], $@)])dnl
AC_DEFUN(AM_DISABLE_SHARED, [indir([AC_DISABLE_SHARED], $@)])dnl
AC_DEFUN(AM_DISABLE_STATIC, [indir([AC_DISABLE_STATIC], $@)])dnl
AC_DEFUN(AM_PROG_LD, [indir([AC_PROG_LD])])dnl
AC_DEFUN(AM_PROG_NM, [indir([AC_PROG_NM])])dnl

dnl This is just to silence aclocal about the macro not being used
ifelse([AC_DISABLE_FAST_INSTALL])dnl

# Add --enable-maintainer-mode option to configure.
# From Jim Meyering

# serial 1

AC_DEFUN(AM_MAINTAINER_MODE,
[AC_MSG_CHECKING([whether to enable maintainer-specific portions of Makefiles])
  dnl maintainer-mode is disabled by default
  AC_ARG_ENABLE(maintainer-mode,
[  --enable-maintainer-mode enable make rules and dependencies not useful
                          (and sometimes confusing) to the casual installer],
      USE_MAINTAINER_MODE=$enableval,
      USE_MAINTAINER_MODE=no)
  AC_MSG_RESULT($USE_MAINTAINER_MODE)
  AM_CONDITIONAL(MAINTAINER_MODE, test $USE_MAINTAINER_MODE = yes)
  MAINT=$MAINTAINER_MODE_TRUE
  AC_SUBST(MAINT)dnl
]
)

# Define a conditional.

AC_DEFUN(AM_CONDITIONAL,
[AC_SUBST($1_TRUE)
AC_SUBST($1_FALSE)
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi])

# Usage:
#   SIM_AC_CHECK_LINKSTYLE
#
# Description:
#
#   Detect how to link against external libraries; UNIX-style
#   ("-llibname") or MSWin-style ("libname.lib"). As a side-effect of
#   running this macro, the shell variable sim_ac_linking_style will be
#   set to either "mswin" or "unix".
#
# Author:
#   Marius B. Monsen <mariusbu@sim.no>

AC_DEFUN([SIM_AC_CHECK_LINKSTYLE], [

sim_ac_save_ldflags=$LDFLAGS
LDFLAGS="$LDFLAGS version.lib"

AC_CACHE_CHECK(
  [if linking should be done "MSWin-style"],
  sim_cv_mswin_linking,
  AC_TRY_COMPILE([#include <windows.h>
#include <version.h>],
                 [(void)GetFileVersionInfoSize(0L, 0L);],
                 [sim_cv_mswin_linking=yes],
                 [sim_cv_mswin_linking=no])
)

LDFLAGS=$sim_ac_save_ldflags

if test x"$sim_cv_mswin_linking" = x"yes"; then
  sim_ac_linking_style=mswin
else
  sim_ac_linking_style=unix
fi
])

# Usage:
#  SIM_AC_DOXYGEN_TOOL([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
# Description:
#   This macro locates the doxygen executable. If it is found, the
#   variable $sim_ac_doxygen_exe is set to the full path- and
#   executable name (if not found, it is set to "false") and
#   $sim_ac_doxygen_avail is set to the version number (if not
#   found, it is set to "no").
#
# Author: Morten Eriksen, <mortene@sim.no>.

AC_DEFUN(SIM_AC_DOXYGEN_TOOL, [
AC_ARG_WITH(
  [doxygen],
  AC_HELP_STRING([--with-doxygen=DIR],
                 [DIR is the directory where the doxygen executable resides]),
  [],
  [with_doxygen=yes])

sim_ac_doxygen_avail=no

if test x"$with_doxygen" != xno; then
  sim_ac_path=$PATH
  if test x"$with_doxygen" != xyes; then
    sim_ac_path=${with_doxygen}:$PATH
  fi

  AC_PATH_PROG(sim_ac_doxygen_exe, doxygen, false, $sim_ac_path)
  if test x"$sim_ac_doxygen_exe" = xfalse; then
    ifelse([$2], , :, [$2])
  else
    sim_ac_doxygen_avail=`$sim_ac_doxygen_exe -help 2> /dev/null | head -1 | sed 's%[[^ ]]\+ [[^ ]]\+ %%'`
    $1
  fi
fi
])

# Usage:
#   SIM_COMPILE_DEBUG( ACTION-IF-DEBUG, ACTION-IF-NOT-DEBUG )
#
# Description:
#   Let the user decide if compilation should be done in "debug mode".
#   If compilation is not done in debug mode, all assert()'s in the code
#   will be disabled.
#
#   Also sets enable_debug variable to either "yes" or "no", so the
#   configure.in writer can add package-specific actions. Default is "yes".
#   This was also extended to enable the developer to set up the two first
#   macro arguments following the well-known ACTION-IF / ACTION-IF-NOT
#   concept.
#
#   Note: this macro must be placed after either AC_PROG_CC or AC_PROG_CXX
#   in the configure.in script.
#
# Authors:
#   Morten Eriksen, <mortene@sim.no>
#   Lars J. Aas, <larsa@sim.no>
#
# TODO:
# * [larsa:20000220] Set up ATTRIBUTE-LIST for developer-configurable
#   default-value.
#

AC_DEFUN([SIM_COMPILE_DEBUG], [
AC_PREREQ([2.13])

AC_ARG_ENABLE(
  [debug],
  AC_HELP_STRING([--enable-debug], [compile in debug mode [default=yes]]),
  [case "${enableval}" in
    yes) enable_debug=yes ;;
    no)  enable_debug=no ;;
    *) AC_MSG_ERROR(bad value \"${enableval}\" for --enable-debug) ;;
  esac],
  [enable_debug=yes])

if test x"$enable_debug" = x"yes"; then
  ifelse([$1], , :, [$1])
else
  CFLAGS="$CFLAGS -DNDEBUG"
  CXXFLAGS="$CXXFLAGS -DNDEBUG"
  $2
fi
])


# Usage:
#   SIM_DEBUGSYMBOLS
#
# Description:
#   Let the user decide if debug symbol information should be compiled
#   in. The compiled libraries/executables will use a lot less space
#   if stripped for their symbol information.
# 
#   Note: this macro must be placed after either AC_PROG_CC or AC_PROG_CXX
#   in the configure.in script.
# 
# Author: Morten Eriksen, <mortene@sim.no>.
# 
# TODO:
#   * [mortene:19991114] make this work with compilers other than gcc/g++
# 

AC_DEFUN([SIM_DEBUGSYMBOLS], [
AC_PREREQ([2.13])
AC_ARG_ENABLE(
  [symbols],
  AC_HELP_STRING([--enable-symbols],
                 [(GCC only) include symbol debug information [default=yes]]),
  [case "${enableval}" in
    yes) enable_symbols=yes ;;
    no)  enable_symbols=no ;;
    *) AC_MSG_ERROR(bad value \"${enableval}\" for --enable-symbols) ;;
  esac],
  [enable_symbols=yes])

if test x"$enable_symbols" = x"no"; then
  if test x"$GXX" = x"yes" || x"$GCC" = x"yes"; then
    CFLAGS="`echo $CFLAGS | sed 's/-g//'`"
    CXXFLAGS="`echo $CXXFLAGS | sed 's/-g//'`"
  else
    AC_MSG_WARN([--disable-symbols only has effect when using GNU gcc or g++])
  fi
fi
])


# Usage:
#   SIM_AC_RTTI_SUPPORT
#
# Description:
#   Let the user decide if RTTI should be compiled in. The compiled
#   libraries/executables will use a lot less space if they don't
#   contain RTTI.
# 
#   Note: this macro must be placed after AC_PROG_CXX in the
#   configure.in script.
# 
# Author: Morten Eriksen, <mortene@sim.no>.

AC_DEFUN([SIM_AC_RTTI_SUPPORT], [
AC_PREREQ([2.13])
AC_ARG_ENABLE(
  [rtti],
  AC_HELP_STRING([--enable-rtti], [(g++ only) compile with RTTI [default=yes]]),
  [case "${enableval}" in
    yes) enable_rtti=yes ;;
    no)  enable_rtti=no ;;
    *) AC_MSG_ERROR(bad value \"${enableval}\" for --enable-rtti) ;;
  esac],
  [enable_rtti=yes])

if test x"$enable_rtti" = x"no"; then
  if test x"$GXX" = x"yes"; then
    CXXFLAGS="$CXXFLAGS -fno-rtti"
  else
    AC_MSG_WARN([--enable-rtti only has effect when using GNU g++])
  fi
fi
])

# Usage:
#   SIM_CHECK_EXCEPTION_HANDLING
#
# Description:
#   Let the user decide if C++ exception handling should be compiled
#   in. The compiled libraries/executables will use a lot less space
#   if they have exception handling support.
#
#   Note: this macro must be placed after AC_PROG_CXX in the
#   configure.in script.
#
#   Author: Morten Eriksen, <mortene@sim.no>.
#
# TODO:
#   * [mortene:19991114] make this work with compilers other than gcc/g++
#

AC_DEFUN([SIM_EXCEPTION_HANDLING], [
AC_PREREQ([2.13])
AC_ARG_ENABLE(
  [exceptions],
  AC_HELP_STRING([--enable-exceptions],
                 [(g++ only) compile with exceptions [default=no]]),
  [case "${enableval}" in
    yes) enable_exceptions=yes ;;
    no)  enable_exceptions=no ;;
    *) AC_MSG_ERROR(bad value \"${enableval}\" for --enable-exceptions) ;;
  esac],
  [enable_exceptions=no])

if test x"$enable_exceptions" = x"no"; then
  if test "x$GXX" = "xyes"; then
    unset _exception_flag
    dnl This is for GCC >= 2.8
    SIM_AC_CXX_COMPILER_OPTION([-fno-exceptions], [_exception_flag=-fno-exceptions])
    if test x"${_exception_flag+set}" != x"set"; then
      dnl For GCC versions < 2.8
      SIM_AC_CXX_COMPILER_OPTION([-fno-handle-exceptions],
                                 [_exception_flag=-fno-handle-exceptions])
    fi
    if test x"${_exception_flag+set}" != x"set"; then
      AC_MSG_WARN([couldn't find a valid option for avoiding exception handling])
    else
      CXXFLAGS="$CXXFLAGS $_exception_flag"
    fi
  fi
else
  if test x"$GXX" != x"yes"; then
    AC_MSG_WARN([--enable-exceptions only has effect when using GNU g++])
  fi
fi
])


#   Use this file to store miscellaneous macros related to checking
#   compiler features.

# Usage:
#   SIM_AC_CC_COMPILER_OPTION(OPTION-TO-TEST, ACTION-IF-TRUE [, ACTION-IF-FALSE])
#   SIM_AC_CXX_COMPILER_OPTION(OPTION-TO-TEST, ACTION-IF-TRUE [, ACTION-IF-FALSE])
#
# Description:
#
#   Check whether the current C or C++ compiler can handle a
#   particular command-line option.
#
#
# Author: Morten Eriksen, <mortene@sim.no>.
#
#   * [mortene:19991218] improve macros by catching and analyzing
#     stderr (at least to see if there was any output there)?
#

AC_DEFUN([SIM_AC_COMPILER_OPTION], [
sim_ac_save_cppflags=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $1"
AC_TRY_COMPILE([], [], [sim_ac_accept_result=yes], [sim_ac_accept_result=no])
AC_MSG_RESULT([$sim_ac_accept_result])
CPPFLAGS=$sim_ac_save_cppflags
# This need to go last, in case CPPFLAGS is modified in $2 or $3.
if test $sim_ac_accept_result = yes; then
  ifelse($2, , :, $2)
else
  ifelse($3, , :, $3)
fi
])

AC_DEFUN([SIM_AC_CC_COMPILER_OPTION], [
AC_LANG_SAVE
AC_LANG_C
AC_MSG_CHECKING([whether $CC accepts $1])
SIM_AC_COMPILER_OPTION($1, $2, $3)
AC_LANG_RESTORE
])

AC_DEFUN([SIM_AC_CXX_COMPILER_OPTION], [
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_MSG_CHECKING([whether $CXX accepts $1])
SIM_AC_COMPILER_OPTION($1, $2, $3)
AC_LANG_RESTORE
])

# Usage:
#   SIM_PROFILING_SUPPORT
#
# Description:
#   Let the user decide if profiling code should be compiled
#   in. The compiled libraries/executables will use a lot less space
#   if they don't contain profiling code information, and they will also
#   execute faster.
#
#   Note: this macro must be placed after either AC_PROG_CC or AC_PROG_CXX
#   in the configure.in script.
#
# Author: Morten Eriksen, <mortene@sim.no>.
#
# TODO:
#   * [mortene:19991114] make this work with compilers other than gcc/g++
#

AC_DEFUN([SIM_PROFILING_SUPPORT], [
AC_PREREQ([2.13])
AC_ARG_ENABLE(
  [profile],
  AC_HELP_STRING([--enable-profile],
                 [(GCC only) turn on inclusion of profiling code [default=no]]),
  [case "${enableval}" in
    yes) enable_profile=yes ;;
    no)  enable_profile=no ;;
    *) AC_MSG_ERROR(bad value \"${enableval}\" for --enable-profile) ;;
  esac],
  [enable_profile=no])

if test x"$enable_profile" = x"yes"; then
  if test x"$GXX" = x"yes" || test x"$GCC" = x"yes"; then
    CFLAGS="$CFLAGS -pg"
    CXXFLAGS="$CXXFLAGS -pg"
    LDFLAGS="$LDFLAGS -pg"
  else
    AC_MSG_WARN([--enable-profile only has effect when using GNU gcc or g++])
  fi
fi
])


# Usage:
#   SIM_COMPILER_WARNINGS
#
# Description:
#   Take care of making a sensible selection of warning messages
#   to turn on or off.
# 
#   Note: this macro must be placed after either AC_PROG_CC or AC_PROG_CXX
#   in the configure.in script.
# 
# Author: Morten Eriksen, <mortene@sim.no>.
# 
# TODO:
#   * [mortene:19991114] find out how to get GCC's
#     -Werror-implicit-function-declaration option to work as expected
#
#   * [mortene:20000606] there are a few assumptions here which doesn't
#     necessarily hold water: both the C and C++ compiler doesn't have
#     to be "compatible", i.e. the C compiler could be gcc, while the
#     C++ compiler could be a native compiler, for instance. So some
#     restructuring should be done.
# 
#   * [larsa:20000607] don't check all -woff options to SGI MIPSpro CC,
#     just put all of them on the same line, to check if the syntax is ok.

AC_DEFUN([SIM_COMPILER_WARNINGS], [
AC_ARG_ENABLE(
  [warnings],
  AC_HELP_STRING([--enable-warnings],
                 [turn on warnings when compiling [default=yes]]),
  [case "${enableval}" in
    yes) enable_warnings=yes ;;
    no)  enable_warnings=no ;;
    *) AC_MSG_ERROR(bad value \"${enableval}\" for --enable-warnings) ;;
  esac],
  [enable_warnings=yes])

if test x"$enable_warnings" = x"yes"; then
  if test x"$GXX" = x"yes" || test x"$GCC" = x"yes"; then
    sim_ac_common_gcc_warnings="-W -Wall -Wno-unused"
    # -fno-multichar can be different for gcc and egcs c++, for instance,
    # so we need to do separate checks.
    if test x"$CC" = x"$CXX"; then
      CPPFLAGS="$CPPFLAGS $sim_ac_common_gcc_warnings"
      SIM_AC_CXX_COMPILER_OPTION([-Wno-multichar], [CPPFLAGS="$CPPFLAGS -Wno-multichar"])
    else
      CFLAGS="$CFLAGS $sim_ac_common_gcc_warnings"
      SIM_AC_CC_COMPILER_OPTION([-Wno-multichar], [CFLAGS="$CFLAGS -Wno-multichar"])
      CXXFLAGS="$CXXFLAGS $sim_ac_common_gcc_warnings"
      SIM_AC_CXX_COMPILER_OPTION([-Wno-multichar], [CXXFLAGS="$CXXFLAGS -Wno-multichar"])
    fi
  else
    case $host in
    *-*-irix*) 
      if test x"$CC" = xcc || test x"$CXX" = xCC; then
        _warn_flags=
        _woffs=""
        ### Turn on all warnings ######################################
        SIM_AC_CC_COMPILER_OPTION(-fullwarn, CPPFLAGS="$CPPFLAGS -fullwarn")

        ### Turn off specific (bogus) warnings ########################

        ## SGI MipsPro v?.?? (our compiler on IRIX 6.2) ##############
        # 3115: ``type qualifiers are meaningless in this declaration''.
        # 3262: unused variables.
        ## SGI MipsPro v7.30 #########################################
	# 1174: "The function was declared but never referenced."
        # 1209: "The controlling expression is constant." (kill warning on
        #       if (0), assert(FALSE), etc).
        # 1355: Kill warnings on extra semicolons (which happens with some
        #       of the Coin macros).
        # 1375: Non-virtual destructors in base classes.
        # 3201: Unused argument to a function.
        # 3303: "Meaningless type qualifier on return type" (happens with the
        #       SoField macros in Coin because of use of const in the macros).
        # 1110: "Statement is not reachable" (the Lex/Flex generated code in
        #       Coin/src/engines has lots of shitty code which needs this).

        sim_ac_bogus_warnings="-woff 3115,3262,1174,1209,1355,1375,3201,3303,1110"
        SIM_AC_CC_COMPILER_OPTION($sim_ac_bogus_warnings,
                                  CPPFLAGS="$CPPFLAGS $sim_ac_bogus_warnings")
      fi
    ;;
    esac
  fi
else
  if test x"$GXX" != x"yes" && test x"$GCC" != x"yes"; then
    AC_MSG_WARN([--enable-warnings only has effect when using GNU gcc or g++])
  fi
fi
])


# Usage:
#  SIM_CHECK_SNPRINTF
#
# Description:
#   Find out which of these "safe" and non-standard functions are
#   available on the system: snprintf(), vsnprintf(), _snprintf()
#   and _vsnprintf().
#
#   The variables sim_ac_snprintf_avail, sim_ac_vsnprintf_avail,
#   sim_ac__snprintf_avail and sim_ac__vsnprintf_avail are set to either
#   "yes" or "no" according to their availability, and HAVE_SNPRINTF
#   etc will be defined properly.
#
# Authors:
#   Morten Eriksen, <mortene@sim.no>.
#

AC_DEFUN([SIM_CHECK_NPRINTF], [
AC_PREREQ([2.14])

sim_ac_snprintf_avail=no
sim_ac__snprintf_avail=no
sim_ac_vsnprintf_avail=no
sim_ac__vsnprintf_avail=no

AC_CACHE_CHECK(
  [whether snprintf() is available],
  sim_cv_func_snprintf,
  [AC_TRY_LINK([#include <stdio.h>],
               [(void)snprintf(0L, 0, 0L);],
               [sim_cv_func_snprintf=yes],
               [sim_cv_func_snprintf=no])])

sim_ac_snprintf_avail=$sim_cv_func_snprintf


AC_CACHE_CHECK(
  [whether vsnprintf() is available],
  sim_cv_func_vsnprintf,
  [AC_TRY_LINK([#include <stdio.h>],
               [(void)vsnprintf(0L, 0, 0L, 0L);],
               [sim_cv_func_vsnprintf=yes],
               [sim_cv_func_vsnprintf=no])])

sim_ac_vsnprintf_avail=$sim_cv_func_vsnprintf

# We're not interested in _snprintf() unless snprintf() is unavailable.
if test x"$sim_ac_snprintf_avail" = x"no"; then
  AC_CACHE_CHECK(
    [whether _snprintf() is available],
    sim_cv_func__snprintf,
    [AC_TRY_LINK([#include <stdio.h>],
                 [(void)_snprintf(0L, 0, 0L);],
                 [sim_cv_func__snprintf=yes],
                 [sim_cv_func__snprintf=no])])
  sim_ac__snprintf_avail=$sim_cv_func__snprintf
fi

# We're not interested in _vsnprintf() unless vsnprintf() is unavailable.
if test x"$sim_ac_vsnprintf_avail" = xno; then
  AC_CACHE_CHECK(
    [whether _vsnprintf() is available],
    sim_cv_func__vsnprintf,
    [AC_TRY_LINK([#include <stdio.h>],
                 [(void)_vsnprintf(0L, 0, 0L, 0L);],
                 [sim_cv_func__vsnprintf=yes],
                 [sim_cv_func__vsnprintf=no])])
  sim_ac__vsnprintf_avail=$sim_cv_func__vsnprintf
fi

test x"$sim_ac_snprintf_avail" = x"yes" && AC_DEFINE(HAVE_SNPRINTF)
test x"$sim_ac_vsnprintf_avail" = x"yes" && AC_DEFINE(HAVE_VSNPRINTF)
test x"$sim_ac__snprintf_avail" = x"yes" && AC_DEFINE(HAVE__SNPRINTF)
test x"$sim_ac__vsnprintf_avail" = x"yes" && AC_DEFINE(HAVE__VSNPRINTF)
])


# Usage:
#   SIM_AC_CHECK_MACRO_QUOTE([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
# Description:
#   Find out how to quote strings within macros.
#
#   The variable sim_ac_quote_hash will be set to "yes" if we
#   can do ANSI C string quoting within macros using the hash
#   symbol.
#
#   The variable sim_ac_quote_apostrophes will be set to "yes"
#   if we can do string quoting within macros using apostrophes.
#   Note that the string quoting check with apostrophes will
#   not be performed if the hash quoting works.
#
#   ACTION-IF-FOUND will be performed if any method works,
#   otherwise ACTION-IF-NOT-FOUND will be run.
#
# Author: Morten Eriksen, <mortene@sim.no>.
#

AC_DEFUN([SIM_AC_CHECK_MACRO_QUOTE], [

sim_ac_quote_hash=no
sim_ac_quote_apostrophes=
AC_CACHE_CHECK(
  [whether quoting in macros can be done with hash symbol],
  sim_cv_quote_hash,
  [AC_TRY_RUN([#include <string.h>
               #define TEST_QUOTE(str) #str
               int main(void) { return strcmp("sim", TEST_QUOTE(sim)); }],
              [sim_cv_quote_hash=yes],
              [sim_cv_quote_hash=no],
              [AC_MSG_WARN([can't check macroquoting when crosscompiling, assumes ANSI C])
              sim_cv_quote_hash=yes])])

if test x"$sim_cv_quote_hash" = x"yes"; then
  sim_ac_quote_hash=yes
  $1
else
  AC_CACHE_CHECK(
    [whether quoting in macros can be done with apostrophes],
    sim_cv_quote_apostrophes,
    [AC_TRY_RUN([#include <string.h>
                 #define TEST_QUOTE(str) "str"
                 int main(void) { return strcmp("sim", TEST_QUOTE(sim)); }],
                [sim_cv_quote_apostrophes=yes],
                [sim_cv_quote_apostrophes=no],
                [sim_cv_quote_apostrophes=yes])])
  if test x"$sim_cv_quote_apostrophes" = x"yes"; then
    sim_ac_quote_apostrophes=yes
    $1
  else
    sim_ac_quote_apostrophes=no
    $2
  fi
fi
])

# Usage:
#   SIM_AC_CHECK_SIMAGE( ACTION-IF-FOUND, ACTION-IF-NOT-FOUND, ATTRIBUTE-LIST )
#
# Description:
#   This macro locates the simage development system.  If it is found, the
#   set of variables listed below are set up as described and made available
#   to the configure script.
#
# ATTRIBUTE-LIST Options:
#   [no]default              whether --with-simage is default or not
#                            (default on)
#   [no]searchprefix         whether $exec_prefix is to be searched
#                            (default off)
#
# Autoconf Variables:
#   $sim_ac_simage_avail     yes | no
#   $sim_ac_simage_cppflags  (extra flags the compiler needs for simage)
#   $sim_ac_simage_ldflags   (extra flags the linker needs for simage)
#   $sim_ac_simage_libs      (link libraries the linker needs for simage)
#   $CPPFLAGS                $CPPFLAGS $sim_ac_simage_cppflags
#   $LDFLAGS                 $LDFLAGS $sim_ac_simage_ldflags
#   $LIBS                    $sim_ac_simage_libs $LIBS
#
# Automake Conditionals:
#   HAVE_LIBSIMAGE           (code disabled)
#
# Config.h Defines:
#   HAVE_LIBSIMAGE           (code disabled)
#   HAVE_SIMAGE_H            (code disabled)
#
# Authors:
#   Morten Eriksen, <mortene@sim.no>
#   Lars J. Aas, <larsa@sim.no>
#
# TODO:
# * [mortene:20000122] make sure this work on MSWin (with Cygwin)
# * [larsa:20000220] find a less strict AC_PREREQ setting

AC_DEFUN([SIM_AC_CHECK_SIMAGE], [

SIM_PARSE_MODIFIER_LIST([$3],[
  sim4_simage_with          yes
  sim4_simage_searchprefix  no
],[
  default         sim4_simage_with          yes
  nodefault       sim4_simage_with          no
  searchprefix    sim4_simage_searchprefix  yes
  nosearchprefix  sim4_simage_searchprefix  no
])

AC_ARG_WITH(simage, AC_HELP_STRING([--with-simage=DIR], changequote({,}){use simage for loading texture files [default=}sim4_simage_with{]}changequote([,])), , [with_simage=sim4_simage_with])

sim_ac_simage_avail=no

if test "x$with_simage" != "xno"; then
  sim_ac_path=$PATH
  if test "x$with_simage" != "xyes"; then
    sim_ac_path=${with_simage}/bin:$PATH
    ifelse(sim4_simage_searchprefix, yes,
    [if test "x$exec_prefix" != "xNONE"; then
      sim_ac_path=$sim_ac_path:$sim_ac_path/bin
    fi], :)
  fi

  AC_PATH_PROG(sim_ac_simage_conf_cmd, simage-config, false, $sim_ac_path)
  if test "x$sim_ac_simage_conf_cmd" = "xfalse"; then
    AC_MSG_WARN(could not find 'simage-config' in $sim_ac_path)
  fi

  sim_ac_simage_cppflags=`$sim_ac_simage_conf_cmd --cppflags`
  sim_ac_simage_ldflags=`$sim_ac_simage_conf_cmd --ldflags`
  sim_ac_simage_libs=`$sim_ac_simage_conf_cmd --libs`

  AC_CACHE_CHECK([whether the simage library is available],
    sim_cv_lib_simage_avail, [
    sim_ac_save_cppflags=$CPPFLAGS
    sim_ac_save_ldflags=$LDFLAGS
    sim_ac_save_libs=$LIBS
    CPPFLAGS="$CPPFLAGS $sim_ac_simage_cppflags"
    LDFLAGS="$LDFLAGS $sim_ac_simage_ldflags"
    LIBS="$sim_ac_simage_libs $LIBS"
    AC_TRY_LINK([#include <simage.h>],
                [(void)simage_read_image(0L, 0L, 0L, 0L);],
                sim_cv_lib_simage_avail=yes,
                sim_cv_lib_simage_avail=no)
    CPPFLAGS=$sim_ac_save_cppflags
    LDFLAGS=$sim_ac_save_ldflags
    LIBS=$sim_ac_save_libs
  ])

  if test x"$sim_cv_lib_simage_avail" = xyes; then
    sim_ac_simage_avail=yes
    CPPFLAGS="$CPPFLAGS $sim_ac_simage_cppflags"
    LDFLAGS="$LDFLAGS $sim_ac_simage_ldflags"
    LIBS="$sim_ac_simage_libs $LIBS"
#   AM_CONDITIONAL(HAVE_LIBSIMAGE, true)
#   AC_DEFINE(HAVE_SIMAGE_H, 1,
#     [Define this if you have simage.h])
#   AC_DEFINE(HAVE_LIBSIMAGE, 1,
#     [Define this if you are going to use libsimage])
    $1
  else
#   AM_CONDITIONAL(HAVE_LIBSIMAGE, false)
    ifelse([$2], , :, [$2])
  fi
else
  ifelse([$2], , :, [$2])
fi
])


dnl ************************************************************************
dnl Usage:
dnl   SIM_AC_PARSE_MODIFIER_LIST( MODIFIER-LIST-STRING, MODIFIER-VARIABLES, 
dnl       MODIFIER-LIST, opt ACTION-ON-SUCCESS, opt ACTION-ON-FAILURE )
dnl
dnl Description:
dnl   This macro makes it easy to let macros have a MODIFIER-LIST argument
dnl   which can add some flexibility to the macro by letting the developer
dnl   configure some of the macro beaviour from the invocation in the
dnl   configure.in file.
dnl
dnl   Everything is handled on the m4-level, which means things are handled
dnl   at autoconf-run-time, not configure-run-time.  This lets you discover
dnl   problems at an earlier stage, which is nice.  It also lets you insert
dnl   the modifier values into e.g. help strings, something you can't do
dnl   on the shell level.
dnl
dnl   MODIFIER-LIST-STRING is the string of modifiers used in the
dnl   macro invocation.
dnl
dnl   MODIFIER-VARIABLES is a list of variables and their default values.
dnl   The variables and values are recognized as words matching [[^\s-]*]
dnl   separated by whitespace, and they must of course come in pairs.
dnl
dnl   MODIFIER-LIST is a description-list of all the valid modifiers that
dnl   can be used in the MODIFIER-LIST-STRING argument.  They must come in
dnl   tuples of three and three words (same word-definition as above) where
dnl   the first word is the modifier, the second word is the variable
dnl   that is to be set by the modifier, and last the value the modifier
dnl   variable should be set to.
dnl
dnl   ACTION-ON-SUCCESS is the expansion of the macro if all the modifiers
dnl   in MODIFIER-LIST-STRING pass through without problem.  The default
dnl   expansion is nothing.
dnl
dnl   ACTION-ON-FAILURE is the expansion of the macro if some of the
dnl   modifiers in MODIFIER-LIST-STRING doesn't pass through.  The default
dnl   expansion is nothing, but warnings are printed to stderr on the
dnl   modifiers causing the problem.
dnl
dnl Sample Usage:
dnl   [to come later]
dnl
dnl Authors:
dnl   Lars J. Aas <larsa@sim.no> (idea, design, coding)
dnl   Akim Demaille <akim@epita.fr> (hints, tips, corrections)
dnl
dnl TODO:
dnl * [larsa:20000222] more warnings on potential problems
dnl

define([m4_noquote],
[changequote(-=<{,}>=-)$1-=<{}>=-changequote([,])])

AC_DEFUN([SIM_AC_PML_WARNING],
[errprint([SIM_PARSE_MODIFIER_LIST: $1
  (file "]__file__[", line ]__line__[)
])])

define([TAB],[	])
define([LF],[
])

dnl * this is an unquoted string compaction - words in string must expand to
dnl * nothing before compaction starts...
AC_DEFUN([SIM_AC_PML_STRING_COMPACT],
[patsubst(patsubst([$1],m4_noquote([[TAB LF]+]),[ ]),[^ \| $],[])])

AC_DEFUN([SIM_AC_PML_STRING_WORDCOUNT_COMPACT],
[m4_eval((1+len(patsubst([$1],[[^ ]+],[_])))/2)])

AC_DEFUN([SIM_AC_PML_STRING_WORDCOUNT],
[SIM_AC_PML_STRING_WORDCOUNT_COMPACT([SIM_AC_PML_STRING_COMPACT([$1])])])

AC_DEFUN([SIM_AC_PML_DEFINE_VARIABLE],
[define([$1],[$2])])

AC_DEFUN([SIM_AC_PML_DEFINE_VARIABLES],
[ifelse(SIM_AC_PML_STRING_WORDCOUNT_COMPACT([$1]), 2,
        [patsubst([$1],[^\([^ ]+\) \([^ ]+\)],
                  [SIM_AC_PML_DEFINE_VARIABLE([\1],[\2])])],
        [patsubst([$1],[^\([^ ]+\) \([^ ]+\) \(.*\)],
                  [SIM_AC_PML_DEFINE_VARIABLE([\1],[\2])SIM_AC_PML_DEFINE_VARIABLES([\3])])])])

AC_DEFUN([SIM_AC_PML_PUSHDEF_MODIFIER],
[ifelse(defn([$2]), [],
        [SIM_AC_PML_ERROR([invalid variable in argument 3: "$2"])],
        [pushdef([$1],[define([$2],[$3])])])])

AC_DEFUN([SIM_AC_PML_PUSHDEF_MODIFIERS],
[ifelse(SIM_AC_PML_STRING_WORDCOUNT_COMPACT([$1]), 3,
        [patsubst([$1],[^\([^ ]+\) \([^ ]+\) \([^ ]+\)],
                  [SIM_AC_PML_PUSHDEF_MODIFIER([\1],[\2],[\3])])],
        [patsubst([$1],[^\([^ ]+\) \([^ ]+\) \([^ ]+\) \(.*\)],
                  [SIM_AC_PML_PUSHDEF_MODIFIER([\1],[\2],[\3])SIM_AC_PML_PUSHDEF_MODIFIERS([\4])])])])

AC_DEFUN([SIM_AC_PML_POPDEF_MODIFIER],
[popdef([$1])])

AC_DEFUN([SIM_AC_PML_POPDEF_MODIFIERS],
[ifelse(SIM_AC_PML_STRING_WORDCOUNT_COMPACT([$1]), 3,
        [patsubst([$1],[^\([^ ]+\) \([^ ]+\) \([^ ]+\)],
                  [SIM_AC_PML_POPDEF_MODIFIER([\1])])],
        [patsubst([$1],[^\([^ ]+\) \([^ ]+\) \([^ ]+\) \(.*\)],
                  [SIM_AC_PML_POPDEF_MODIFIER([\1])SIM_AC_PML_POPDEF_MODIFIERS([\4])])])])

AC_DEFUN([SIM_AC_PML_PARSE_MODIFIER_LIST],
[pushdef([wordcount],SIM_AC_PML_STRING_WORDCOUNT([$2]))]dnl
[ifelse(m4_eval(((wordcount % 2) == 0) && (wordcount > 0)), 1,
        [],
        [SIM_AC_PML_WARNING([invalid word count ]wordcount[ for argument 2: "]SIM_AC_PML_STRING_COMPACT([$2])")])]dnl
[popdef([wordcount])]dnl
[pushdef([wordcount],SIM_AC_PML_STRING_WORDCOUNT([$3]))]dnl
[ifelse(m4_eval(((wordcount % 3) == 0) && (wordcount > 0)), 1,
        [],
        [SIM_AC_PML_WARNING([invalid word count ]wordcount[ for argument 3: "$3"])])]dnl
[popdef([wordcount])]dnl
[SIM_AC_PML_DEFINE_VARIABLES([$2])]dnl
[SIM_AC_PML_PUSHDEF_MODIFIERS([$3])]dnl
[ifelse(SIM_AC_PML_STRING_COMPACT([$1]), [],
        [ifelse([$4], [], [], [$4])],
        [ifelse([$5], [],
                [SIM_AC_PML_WARNING([modifier(s) parse error: "]SIM_AC_PML_STRING_COMPACT([$1])")],
                [$5])])]dnl
[SIM_AC_PML_POPDEF_MODIFIERS([$3])])

AC_DEFUN([SIM_AC_PARSE_MODIFIER_LIST],
[SIM_AC_PML_PARSE_MODIFIER_LIST(
        SIM_AC_PML_STRING_COMPACT([$1]),
        SIM_AC_PML_STRING_COMPACT([$2]),
        SIM_AC_PML_STRING_COMPACT([$3]),
        [$4],
        [$5])])

dnl * to be deleted after migrating dependant macros to ac_sim_...
AC_DEFUN([SIM_PARSE_MODIFIER_LIST],
[SIM_AC_PML_PARSE_MODIFIER_LIST(
        SIM_AC_PML_STRING_COMPACT([$1]),
        SIM_AC_PML_STRING_COMPACT([$2]),
        SIM_AC_PML_STRING_COMPACT([$3]),
        [$4],
        [$5])])


# Usage:
#   SIM_AC_CHECK_MATHLIB([ACTION-IF-OK[, ACTION-IF-NOT-OK]])
#
# Description:
#   Check if linker needs to explicitly link with the library with
#   math functions. Sets environment variable $sim_ac_libm to the
#   necessary linklibrary, plus includes this library in the LIBS
#   env variable.
#
# Notes:
#   There is a macro AC_CHECK_LIBM in the libtool distribution, but it
#   does at least not work with SGI MIPSpro CC v7.30.
#
# Authors:
#   Lars Jørgen Aas, <larsa@sim.no>
#   Morten Eriksen, <mortene@sim.no>

AC_DEFUN([SIM_AC_CHECK_MATHLIB], [

sim_ac_store_libs=$LIBS
sim_ac_libm=

AC_CACHE_CHECK(
  [for math library functions],
  [sim_cv_lib_math],
  [sim_cv_lib_math=UNDEFINED
  # BeOS and Cygwin platforms has implicit math library linking,
  # and ncr-sysv4.3 might use -lmw (according to AC_CHECK_LIBM in
  # libtool.m4).
  for sim_ac_math_chk in "" -lm -lmw; do
    if test x"$sim_cv_lib_math" = xUNDEFINED; then
      LIBS="$sim_ac_store_libs $sim_ac_math_chk"
      AC_TRY_LINK([#include <math.h>],
                  [fmod(1.0, 1.0); pow(1.0, 1.0); exp(1.0); sin(1.0)],
                  [sim_cv_lib_math=$sim_ac_math_chk])
    fi
  done
  ])

if test x"$sim_cv_lib_math" != xUNDEFINED; then
  sim_ac_libm=$sim_cv_lib_math
  LIBS="$sim_ac_libm $sim_ac_store_libs"
  $1
else
  LIBS=$sim_ac_store_libs
  $2
fi
])

# Usage:
#  SIM_CHECK_X11([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
#  Try to find the X11 development system. If it is found, these
#  shell variables are set:
#
#    $sim_ac_x11_cppflags (extra flags the compiler needs for X11)
#    $sim_ac_x11_ldflags  (extra flags the linker needs for X11)
#    $sim_ac_x11_libs     (link libraries the linker needs for X11)
#
#  The CPPFLAGS, LDFLAGS and LIBS flags will also be modified accordingly.
#  In addition, the variable $sim_ac_x11_avail is set to "yes" if
#  the X11 development system is found.
#
#
# Author: Morten Eriksen, <mortene@sim.no>.

AC_DEFUN([SIM_CHECK_X11], [

sim_ac_x11_avail=no

AC_PATH_XTRA

if test x"$no_x" != xyes; then
  #  *** DEBUG ***
  #  Keep this around, as it can be handy when testing on new systems.
  # echo "X_CFLAGS: $X_CFLAGS"
  # echo "X_PRE_LIBS: $X_PRE_LIBS"
  # echo "X_LIBS: $X_LIBS"
  # echo "X_EXTRA_LIBS: $X_EXTRA_LIBS"
  # echo
  # echo "CFLAGS: $CFLAGS"
  # echo "CPPFLAGS: $CPPFLAGS"
  # echo "CXXFLAGS: $CXXFLAGS"
  # echo "LDFLAGS: $LDFLAGS"
  # echo "LIBS: $LIBS"
  # exit 0

  sim_ac_x11_cppflags="$X_CFLAGS"
  sim_ac_x11_ldflags="$X_LIBS"
  sim_ac_x11_libs="$X_PRE_LIBS -lX11 $X_EXTRA_LIBS"

  sim_ac_save_cppflags=$CPPFLAGS
  sim_ac_save_ldflags=$LDFLAGS
  sim_ac_save_libs=$LIBS

  CPPFLAGS="$CPPFLAGS $sim_ac_x11_cppflags"
  LDFLAGS="$LDFLAGS $sim_ac_x11_ldflags"
  LIBS="$sim_ac_x11_libs $LIBS"

  AC_CACHE_CHECK(
    [whether we can link against X11],
    sim_cv_lib_x11_avail,
    [AC_TRY_LINK([#include <X11/Xlib.h>],
                 [(void)XOpenDisplay(0L);],
                 [sim_cv_lib_x11_avail=yes],
                 [sim_cv_lib_x11_avail=no])])

  if test x"$sim_cv_lib_x11_avail" = x"yes"; then
    sim_ac_x11_avail=yes
    $1
  else
    CPPFLAGS=$sim_ac_save_cppflags
    LDFLAGS=$sim_ac_save_ldflags
    LIBS=$sim_ac_save_libs
    $2
  fi
fi
])

# Usage:
#  SIM_CHECK_X11SHMEM([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
#  Try to find the X11 shared memory extension. If it is found, this
#  shell variable is set:
#
#    $sim_ac_x11shmem_libs   (link libraries the linker needs for X11 Shm)
#
#  The LIBS flag will also be modified accordingly. In addition, the
#  variable $sim_ac_x11shmem_avail is set to "yes" if the X11 shared
#  memory extension is found.
#
#
# Author: Morten Eriksen, <mortene@sim.no>.
#
# TODO:
#    * [mortene:20000122] make sure this work on MSWin (with
#      Cygwin installation)
#

AC_DEFUN([SIM_CHECK_X11SHMEM], [
AC_PREREQ([2.14.1])

sim_ac_x11shmem_avail=no
sim_ac_x11shmem_libs="-lXext"
sim_ac_save_libs=$LIBS
LIBS="$sim_ac_x11shmem_libs $LIBS"

AC_CACHE_CHECK(
  [whether the X11 shared memory extension is available],
  sim_cv_lib_x11shmem_avail,
  [AC_TRY_LINK([#include <X11/Xlib.h>
               #include <X11/extensions/XShm.h>],
               [(void)XShmQueryVersion(0L, 0L, 0L, 0L);],
               [sim_cv_lib_x11shmem_avail=yes],
               [sim_cv_lib_x11shmem_avail=no])])

if test x"$sim_cv_lib_x11shmem_avail" = xyes; then
  sim_ac_x11shmem_avail=yes
  $1
else
  LIBS=$sim_ac_save_libs
  $2
fi
])

# Usage:
#  SIM_CHECK_X11MU([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
#  Try to find the X11 miscellaneous utilities extension. If it is
#  found, this shell variable is set:
#
#    $sim_ac_x11mu_libs   (link libraries the linker needs for X11 MU)
#
#  The LIBS flag will also be modified accordingly. In addition, the
#  variable $sim_ac_x11mu_avail is set to "yes" if the X11 miscellaneous
#  utilities extension is found.
#
# Author: Morten Eriksen, <mortene@sim.no>.
#
# TODO:
#    * [mortene:20000122] make sure this work on MSWin (with
#      Cygwin installation)
#

AC_DEFUN([SIM_CHECK_X11MU], [
AC_PREREQ([2.14.1])

sim_ac_x11mu_avail=no
sim_ac_x11mu_libs="-lXmu"
sim_ac_save_libs=$LIBS
LIBS="$sim_ac_x11mu_libs $LIBS"

AC_CACHE_CHECK(
  [whether the X11 miscellaneous utilities is available],
  sim_cv_lib_x11mu_avail,
  [AC_TRY_LINK([#include <X11/Xlib.h>
                #include <X11/Xmu/Xmu.h>
                #include <X11/Xmu/StdCmap.h>],
               [(void)XmuAllStandardColormaps(0L);],
               [sim_cv_lib_x11mu_avail=yes],
               [sim_cv_lib_x11mu_avail=no])])

if test x"$sim_cv_lib_x11mu_avail" = xyes; then
  sim_ac_x11mu_avail=yes
  $1
else
  LIBS=$sim_ac_save_libs
  $2
fi
])

# Usage:
#  SIM_CHECK_X11XID([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
#  Try to find the X11 extension device library. Sets this
#  shell variable:
#
#    $sim_ac_x11xid_libs   (link libraries the linker needs for X11 XID)
#
#  The LIBS flag will also be modified accordingly. In addition, the
#  variable $sim_ac_x11xid_avail is set to "yes" if the X11 extension
#  device library is found.
#
# Author: Morten Eriksen, <mortene@sim.no>.
#
# TODO:
#    * [mortene:20000122] make sure this work on MSWin (with
#      Cygwin installation)
#

AC_DEFUN([SIM_CHECK_X11XID], [
AC_PREREQ([2.14.1])

sim_ac_x11xid_avail=no
sim_ac_x11xid_libs="-lXi"
sim_ac_save_libs=$LIBS
LIBS="$sim_ac_x11xid_libs $LIBS"

AC_CACHE_CHECK(
  [whether the X11 extension device library is available],
  sim_cv_lib_x11xid_avail,
  [AC_TRY_LINK([#include <X11/extensions/XInput.h>],
               [(void)XOpenDevice(0L, 0);],
               [sim_cv_lib_x11xid_avail=yes],
               [sim_cv_lib_x11xid_avail=no])])

if test x"$sim_cv_lib_x11xid_avail" = xyes; then
  sim_ac_x11xid_avail=yes
  $1
else
  LIBS=$sim_ac_save_libs
  $2
fi
])

# Usage:
#  SIM_CHECK_X_INTRINSIC([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
#  Try to find the Xt intrinsic library. Sets this shell variable:
#
#    $sim_ac_xt_libs   (link library the linker needs for X Intrinsic)
#
#  The LIBS flag will also be modified accordingly. In addition, the
#  variable $sim_ac_xt_avail is set to "yes" if the X11 Intrinsic
#  library is found.
#
# Author: Morten Eriksen, <mortene@sim.no>.
#

AC_DEFUN([SIM_CHECK_X_INTRINSIC], [
AC_PREREQ([2.14.1])

sim_ac_xt_avail=no
sim_ac_xt_libs="-lXt"
sim_ac_save_libs=$LIBS
LIBS="$sim_ac_xt_libs $LIBS"

AC_CACHE_CHECK(
  [whether the X11 Intrinsic library is available],
  sim_cv_lib_xt_avail,
  [AC_TRY_LINK([#include <X11/Intrinsic.h>],
               [(void)XtVaCreateWidget("", 0L, 0L);],
               [sim_cv_lib_xt_avail=yes],
               [sim_cv_lib_xt_avail=no])])

if test x"$sim_cv_lib_xt_avail" = xyes; then
  sim_ac_xt_avail=yes
  $1
else
  LIBS=$sim_ac_save_libs
  $2
fi
])

# Usage:
#   SIM_CHECK_LIBXPM( [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND] )
#
# Description:
#   This macro checks for libXpm.
#
# Variables:
#   $sim_ac_xpm_avail      yes | no
#   $sim_ac_xpm_libs       [link-line libraries]
#
# Authors:
#   Lars J. Aas <larsa@sim.no>
#

AC_DEFUN([SIM_CHECK_LIBXPM], [
AC_PREREQ([2.14.1])

sim_ac_xpm_avail=no
sim_ac_xpm_libs="-lXpm"

AC_CACHE_CHECK(
  [whether libXpm is available],
  sim_cv_lib_xpm_avail,
  [sim_ac_save_libs=$LIBS
  LIBS="$sim_ac_xpm_libs $LIBS"
  AC_TRY_LINK([#include <X11/xpm.h>],
              [(void)XpmLibraryVersion();],
              [sim_cv_lib_xpm_avail=yes],
              [sim_cv_lib_xpm_avail=no])
  LIBS="$sim_ac_save_libs"])

if test x"$sim_cv_lib_xpm_avail" = x"yes"; then
  sim_ac_xpm_avail=yes
  LIBS="$sim_ac_xpm_libs $LIBS"
  $1
else
  ifelse([$2], , :, [$2])
fi
])


# Usage:
#  SIM_AC_CHECK_X11_XP([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
#  Try to find the Xp library for printing functionality. Sets this
#  shell variable:
#
#    $sim_ac_xp_libs   (link library the linker needs for the Xp library)
#
#  The LIBS flag will also be modified accordingly. In addition, the
#  variable $sim_ac_xp_avail is set to "yes" if the Xp library is found.
#
# Author: Morten Eriksen, <mortene@sim.no>.
#

AC_DEFUN([SIM_AC_CHECK_X11_XP], [
sim_ac_xp_avail=no
sim_ac_xp_libs="-lXp"
sim_ac_save_libs=$LIBS
LIBS="$sim_ac_xp_libs $LIBS"

AC_CACHE_CHECK(
  [whether the X11 printing library is available],
  sim_cv_lib_xp_avail,
  [AC_TRY_LINK([#include <X11/extensions/Print.h>],
               [XpEndJob(0L);],
               [sim_cv_lib_xp_avail=yes],
               [sim_cv_lib_xp_avail=no])])

if test x"$sim_cv_lib_xp_avail" = xyes; then
  sim_ac_xp_avail=yes
  $1
else
  LIBS=$sim_ac_save_libs
  $2
fi
])

# SIM_AC_CHECK_X11_ATHENA( [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND] )

AC_DEFUN([SIM_AC_CHECK_X11_ATHENA], [
sim_ac_athena_avail=no
sim_ac_athena_libs="-lXaw"
sim_ac_save_libs=$LIBS
LIBS="$sim_ac_athena_libs $LIBS"

AC_CACHE_CHECK(
  [whether the X11 Athena widgets library is available],
  sim_cv_lib_athena_avail,
  [AC_TRY_LINK([#include <X11/Xfuncproto.h>
                #include <X11/Xaw/XawInit.h>],
               [XawInitializeWidgetSet();],
               [sim_cv_lib_athena_avail=yes],
               [sim_cv_lib_athena_avail=no])])

if test x"$sim_cv_lib_athena_avail" = xyes; then
  sim_ac_athena_avail=yes
  $1
else
  LIBS=$sim_ac_save_libs
  $2
fi
])

# SIM_AC_X11_READY( [ACTION-IF-TRUE], [ACTION-IF-FALSE] )

AC_DEFUN([SIM_AC_CHECK_X11_READY],
[AC_CACHE_CHECK(
  [if X11 linkage is ready],
  [sim_cv_x11_ready],
  [AC_TRY_LINK(
    [#include <X11/Xlib.h>],
    [(void)XOpenDisplay(0L);],
    [sim_cv_x11_ready=true],
    [sim_cv_x11_ready=false])])
if ${sim_cv_x11_ready}; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # SIM_AC_X11_READY()


# Usage:
#  SIM_AC_CHECK_OPENGL([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
#  Try to find an OpenGL development system, either a native
#  implementation or the OpenGL-compatible Mesa libraries. If
#  it is found, these shell variables are set:
#
#    $sim_ac_gl_cppflags (extra flags the compiler needs for OpenGL/Mesa)
#    $sim_ac_gl_ldflags  (extra flags the linker needs for OpenGL/Mesa)
#    $sim_ac_gl_libs     (link libraries the linker needs for OpenGL/Mesa)
#
#  The CPPFLAGS, LDFLAGS and LIBS flags will also be modified accordingly.
#  In addition, the variable $sim_ac_gl_avail is set to "yes" if an
#  OpenGL-compatible development system is found.
#
#
# Author: Morten Eriksen, <mortene@sim.no>.

AC_DEFUN(SIM_AC_CHECK_OPENGL, [

unset sim_ac_gl_cppflags
unset sim_ac_gl_ldflags
unset sim_ac_gl_libs
sim_ac_gl_avail=no

AC_ARG_WITH(
  [mesa],
  AC_HELP_STRING([--with-mesa],
                 [prefer MesaGL (if found) over OpenGL [default=yes]]),
  [],
  [with_mesa=yes])

if test x"$sim_ac_linking_style" = xmswin; then
  sim_ac_gl_glname=opengl32.lib
  sim_ac_gl_gluname=glu32.lib
  # FIXME: are these two names correct? Probably not. 20000602 mortene.
  sim_ac_gl_mesaglname=mesagl.lib
  sim_ac_gl_mesagluname=mesaglu.lib
else
  sim_ac_gl_glname=-lGL
  sim_ac_gl_gluname=-lGLU
  sim_ac_gl_mesaglname=-lMesaGL
  sim_ac_gl_mesagluname=-lMesaGLU
fi

if test "x$with_mesa" = "xyes"; then
  sim_ac_gl_first_gl=$sim_ac_gl_mesaglname
  sim_ac_gl_first_glu=$sim_ac_gl_mesagluname
  sim_ac_gl_second_gl=$sim_ac_gl_glname
  sim_ac_gl_second_glu=$sim_ac_gl_gluname
else
  sim_ac_gl_first_gl=$sim_ac_gl_glname
  sim_ac_gl_first_glu=$sim_ac_gl_gluname
  sim_ac_gl_second_gl=$sim_ac_gl_mesaglname
  sim_ac_gl_second_glu=$sim_ac_gl_mesagluname
fi

AC_ARG_WITH(
  [opengl],
  AC_HELP_STRING([--with-opengl=DIR],
                 [OpenGL/Mesa installation directory]),
  [],
  [with_opengl=yes])

if test x"$with_opengl" != xno; then
  if test x"$with_opengl" != xyes; then
    sim_ac_gl_cppflags="-I${with_opengl}/include"
    sim_ac_gl_ldflags="-L${with_opengl}/lib"
  else
    # This is a common location for the OpenGL libraries on HPUX.
    sim_ac_gl_hpux=/opt/graphics/OpenGL
    if test -d $sim_ac_gl_hpux; then
      sim_ac_gl_cppflags=-I$sim_ac_gl_hpux/include
      sim_ac_gl_ldflags=-L$sim_ac_gl_hpux/lib
    fi
  fi

  sim_ac_save_cppflags=$CPPFLAGS
  sim_ac_save_ldflags=$LDFLAGS
  sim_ac_save_libs=$LIBS

  CPPFLAGS="$CPPFLAGS $sim_ac_gl_cppflags"
  LDFLAGS="$LDFLAGS $sim_ac_gl_ldflags"

  AC_CACHE_CHECK(
    [whether OpenGL libraries are available],
    sim_cv_lib_gl,
    [sim_cv_lib_gl=UNRESOLVED

    # Some platforms (like BeOS) have the GLU functionality in the GL
    # library (and no GLU library present), so the check is first done
    # against -lGL or -lMesaGL alone.
    for sim_ac_gl_libcheck in $sim_ac_gl_first_gl "$sim_ac_gl_first_gl $sim_ac_gl_first_glu"  $sim_ac_gl_second_gl "$sim_ac_gl_second_gl $sim_ac_gl_second_glu"; do
      if test "x$sim_cv_lib_gl" = "xUNRESOLVED"; then
        LIBS="$sim_ac_gl_libcheck $sim_ac_save_libs"
        AC_TRY_LINK([#ifdef _WIN32
                    #include <windows.h>
                    #endif
                    #include <GL/gl.h>
                    #include <GL/glu.h>],
                    [glPointSize(1.0f);
                    gluSphere(0L, 1.0, 1, 1);],
                    [sim_cv_lib_gl="$sim_ac_gl_libcheck"])
      fi
    done
  ])

  LIBS="$sim_ac_save_libs"

  if test "x$sim_cv_lib_gl" != "xUNRESOLVED"; then
    sim_ac_gl_libs="$sim_cv_lib_gl"
  else
    AC_MSG_WARN([couldn't compile or link with OpenGL libraries -- trying with pthread library in place...])

    SIM_AC_CHECK_PTHREAD([
      sim_ac_gl_cppflags="$sim_ac_gl_cppflags $sim_ac_pthread_cppflags"
      sim_ac_gl_ldflags="$sim_ac_gl_ldflags $sim_ac_pthread_ldflags"],
      AC_MSG_WARN(couldn't compile or link with pthread library))

    if test "x$sim_ac_pthread_avail" = "xyes"; then
      AC_CACHE_CHECK(
        [whether OpenGL libraries can be linked with pthread library],
        sim_cv_lib_gl_pthread,
        [sim_cv_lib_gl_pthread=UNRESOLVED

        # Some platforms (like BeOS) have the GLU functionality in the GL
        # library (and no GLU library present), so the check is first done
        # against -lGL or -lMesaGL alone.
        for sim_ac_gl_libcheck in $sim_ac_gl_first_gl "$sim_ac_gl_first_gl $sim_ac_gl_first_glu"  $sim_ac_gl_second_gl "$sim_ac_gl_second_gl $sim_ac_gl_second_glu"; do
          if test "x$sim_cv_lib_gl_pthread" = "xUNRESOLVED"; then
            LIBS="$sim_ac_gl_libcheck $sim_ac_pthread_libs $sim_ac_save_libs"
            AC_TRY_LINK([#include <GL/gl.h>
                        #include <GL/glu.h>],
                        [glPointSize(1.0f);
                        gluSphere(0L, 1.0, 1, 1);],
                        [sim_cv_lib_gl_pthread="$sim_ac_gl_libcheck"])
          fi
        done
      ])

      if test "x$sim_cv_lib_gl_pthread" != "xUNRESOLVED"; then
        sim_ac_gl_libs="$sim_cv_lib_gl_pthread $sim_ac_pthread_libs"
      fi
    fi
  fi


  if test "x$sim_ac_gl_libs" != "x"; then
    LIBS="$sim_ac_gl_libs $sim_ac_save_libs"
    sim_ac_gl_avail=yes
    $1
  else
    CPPFLAGS=$sim_ac_save_cppflags
    LDFLAGS=$sim_ac_save_ldflags
    LIBS=$sim_ac_save_libs
    $2
  fi
fi
])


# Usage:
#  SIM_AC_GLU_NURBSOBJECT([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
#  Try to find out whether the interface struct against the GLU
#  library NURBS functions is called "GLUnurbs" or "GLUnurbsObj".
#  (This seems to have changed somewhere between release 1.1 and
#  release 1.3 of GLU).
#
#  The variable $sim_ac_glu_nurbsobject is set to the correct name
#  if the nurbs structure is found.
#
# Author: Morten Eriksen, <mortene@sim.no>.

AC_DEFUN(SIM_AC_GLU_NURBSOBJECT, [
AC_CACHE_CHECK(
  [what structure to use in the GLU NURBS interface],
  sim_cv_func_glu_nurbsobject,
  [sim_cv_func_glu_nurbsobject=NONE
   for sim_ac_glu_structname in GLUnurbs GLUnurbsObj; do
    if test "$sim_cv_func_glu_nurbsobject" = NONE; then
      AC_TRY_LINK([#ifdef _WIN32
                  #include <windows.h>
                  #endif
                  #include <GL/gl.h>
                  #include <GL/glu.h>],
                  [$sim_ac_glu_structname * hepp = gluNewNurbsRenderer();
                   gluDeleteNurbsRenderer(hepp)],
                  [sim_cv_func_glu_nurbsobject=$sim_ac_glu_structname])
    fi
  done
])

if test $sim_cv_func_glu_nurbsobject = NONE; then
  sim_ac_glu_nurbsobject=
  $2
else
  sim_ac_glu_nurbsobject=$sim_cv_func_glu_nurbsobject
  $1
fi
])

# Usage:
#  SIM_AC_CHECK_PTHREAD([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
#
#  Try to find the PTHREAD development system. If it is found, these
#  shell variables are set:
#
#    $sim_ac_pthread_cppflags (extra flags the compiler needs for pthread)
#    $sim_ac_pthread_ldflags  (extra flags the linker needs for pthread)
#    $sim_ac_pthread_libs     (link libraries the linker needs for pthread)
#
#  The CPPFLAGS, LDFLAGS and LIBS flags will also be modified accordingly.
#  In addition, the variable $sim_ac_pthread_avail is set to "yes" if the
#  pthread development system is found.
#
#
# Author: Morten Eriksen, <mortene@sim.no>.

AC_DEFUN([SIM_AC_CHECK_PTHREAD], [

AC_ARG_WITH(
  [pthread],
  AC_HELP_STRING([--with-pthread=DIR],
                 [pthread installation directory]),
  [],
  [with_pthread=yes])

sim_ac_pthread_avail=no

if test x"$with_pthread" != xno; then
  if test x"$with_pthread" != xyes; then
    sim_ac_pthread_cppflags="-I${with_pthread}/include"
    sim_ac_pthread_ldflags="-L${with_pthread}/lib"
  fi
  sim_ac_pthread_libs="-lpthread"

  sim_ac_save_cppflags=$CPPFLAGS
  sim_ac_save_ldflags=$LDFLAGS
  sim_ac_save_libs=$LIBS

  CPPFLAGS="$CPPFLAGS $sim_ac_pthread_cppflags"
  LDFLAGS="$LDFLAGS $sim_ac_pthread_ldflags"
  LIBS="$sim_ac_pthread_libs $LIBS"

  AC_CACHE_CHECK(
    [whether the pthread development system is available],
    sim_cv_lib_pthread_avail,
    [AC_TRY_LINK([#include <pthread.h>],
                 [(void)pthread_create(0L, 0L, 0L, 0L);],
                 [sim_cv_lib_pthread_avail=yes],
                 [sim_cv_lib_pthread_avail=no])])

  if test x"$sim_cv_lib_pthread_avail" = xyes; then
    sim_ac_pthread_avail=yes
    $1
  else
    CPPFLAGS=$sim_ac_save_cppflags
    LDFLAGS=$sim_ac_save_ldflags
    LIBS=$sim_ac_save_libs
    $2
  fi
fi
])


# Usage:
#   SIM_EXPAND_DIR_VARS
#
# Description:
#   Expand these variables into their correct full directory paths:
#    $prefix  $exec_prefix  $includedir  $libdir  $datadir
# 
# Author: Morten Eriksen, <mortene@sim.no>.
# 

AC_DEFUN([SIM_EXPAND_DIR_VARS], [
test x"$prefix" = x"NONE" && prefix="$ac_default_prefix"
test x"$exec_prefix" = x"NONE" && exec_prefix="${prefix}"
includedir="`eval echo $includedir`"
libdir="`eval echo $libdir`"
datadir="`eval echo $datadir`"
mandir="`eval echo $mandir`"
])

# Convenience macros SIM_AC_DEBACKSLASH and SIM_AC_DOBACKSLASH for
# converting to and from MSWin/MS-DOS style paths.
#
# Example use:
#
#     SIM_AC_DEBACKSLASH(my_ac_reversed, "C:\\mydir\\bin")
#
# will give a shell variable $my_ac_reversed with the value "C:/mydir/bin").
# Vice versa for SIM_AC_DOBACKSLASH.
#
# Author: Marius Bugge Monsen <mariusbu@sim.no>
#         Lars Jørgen Aas <larsa@sim.no>
#         Morten Eriksen <mortene@sim.no>

AC_DEFUN([SIM_AC_DEBACKSLASH], [
eval "$1=\"`echo $2 | sed -e 's%\\\\%\\/%g'`\""
])

AC_DEFUN([SIM_AC_DOBACKSLASH], [
eval "$1=\"`echo $2 | sed -e 's%\\/%\\\\%g'`\""
])

AC_DEFUN([SIM_AC_DODOUBLEBACKSLASH], [
eval "$1=\"`echo $2 | sed -e 's%\\/%\\\\\\\\\\\\\\\\%g'`\""
])


