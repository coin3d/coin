dnl aclocal.m4 generated automatically by aclocal 1.4a

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
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

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN(AM_INIT_AUTOMAKE,
[AC_REQUIRE([AC_PROG_INSTALL])
dnl We require 2.13 because we rely on SHELL being computed by configure.
AC_PREREQ([2.13])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`CDPATH=: && cd $srcdir && pwd`" != "`pwd`" &&
   test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package]))
AC_REQUIRE([AM_SANITY_CHECK])
AC_REQUIRE([AC_ARG_PROGRAM])
AM_MISSING_PROG(ACLOCAL, aclocal)
AM_MISSING_PROG(AUTOCONF, autoconf)
AM_MISSING_PROG(AUTOMAKE, automake)
AM_MISSING_PROG(AUTOHEADER, autoheader)
AM_MISSING_PROG(MAKEINFO, makeinfo)
AM_MISSING_PROG(AMTAR, tar)
AM_MISSING_INSTALL_SH
dnl We need awk for the "check" target.  The system "awk" is bad on
dnl some platforms.
AC_REQUIRE([AC_PROG_AWK])
AC_REQUIRE([AC_PROG_MAKE_SET])
AC_REQUIRE([AM_DEP_TRACK])
AC_REQUIRE([AM_SET_DEPDIR])
ifdef([AC_PROVIDE_AC_PROG_CC], [AM_DEPENDENCIES(CC)], [
   define([AC_PROG_CC], defn([AC_PROG_CC])[AM_DEPENDENCIES(CC)])])
ifdef([AC_PROVIDE_AC_PROG_CXX], [AM_DEPENDENCIES(CXX)], [
   define([AC_PROG_CXX], defn([AC_PROG_CXX])[AM_DEPENDENCIES(CXX)])])
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
  AC_MSG_WARN([`missing' script is too old or missing])
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
  DEPDIR=`sed -n -e '/^DEPDIR = / s///p' < "$mf"`
  test -z "$DEPDIR" && continue
  # When using ansi2knr, U may be empty or an underscore; expand it
  U=`sed -n -e '/^U = / s///p' < "$mf"`
  test -d "$dirpart/$DEPDIR" || mkdir "$dirpart/$DEPDIR"
  # We invoke sed twice because it is the simplest approach to
  # changing $(DEPDIR) to its actual value in the expansion.
  for file in `sed -n -e '
    /^DEP_FILES = .*\\\\$/ {
      s/^DEP_FILES = //
      :loop
	s/\\\\$//
	p
	n
	/\\\\$/ b loop
      p
    }
    /^DEP_FILES = / s/^DEP_FILES = //p' < "$mf" | \
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
# the libltdl convenience library, adds --enable-ltdl-convenience to
# the configure arguments.  Note that LIBLTDL is not AC_SUBSTed, nor
# is AC_CONFIG_SUBDIRS called.  If DIR is not provided, it is assumed
# to be `${top_builddir}/libltdl'.  Make sure you start DIR with
# '${top_builddir}/' (note the single quotes!) if your package is not
# flat, and, if you're not using automake, define top_builddir as
# appropriate in the Makefiles.
AC_DEFUN(AC_LIBLTDL_CONVENIENCE, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
  case "$enable_ltdl_convenience" in
  no) AC_MSG_ERROR([this package needs a convenience libltdl]) ;;
  "") enable_ltdl_convenience=yes
      ac_configure_args="$ac_configure_args --enable-ltdl-convenience" ;;
  esac
  LIBLTDL=ifelse($#,1,$1,['${top_builddir}/libltdl'])/libltdlc.la
  INCLTDL=ifelse($#,1,-I$1,['-I${top_builddir}/libltdl'])
])

# AC_LIBLTDL_INSTALLABLE[(dir)] - sets LIBLTDL to the link flags for
# the libltdl installable library, and adds --enable-ltdl-install to
# the configure arguments.  Note that LIBLTDL is not AC_SUBSTed, nor
# is AC_CONFIG_SUBDIRS called.  If DIR is not provided, it is assumed
# to be `${top_builddir}/libltdl'.  Make sure you start DIR with
# '${top_builddir}/' (note the single quotes!) if your package is not
# flat, and, if you're not using automake, define top_builddir as
# appropriate in the Makefiles.
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
    LIBLTDL=ifelse($#,1,$1,['${top_builddir}/libltdl'])/libltdl.la
    INCLTDL=ifelse($#,1,-I$1,['-I${top_builddir}/libltdl'])
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
#   SIM_CHECK_MACRO_QUOTE([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
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

AC_DEFUN([SIM_CHECK_MACRO_QUOTE], [
AC_PREREQ([2.14])

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
  ifelse([$1], , :, [$1])
else
  AC_CACHE_CHECK(
    [whether quoting in macros can be done with apostrophes],
    sim_cv_quote_apostrophes,
    [AC_TRY_RUN([#include <string.h>
                 #define TEST_QUOTE(str) "str"
                 int main(void) { return strcmp("sim", TEST_QUOTE(sim)); }],
                [sim_cv_quote_apostrophes=yes],
                [sim_cv_quote_apostrophes=no])])
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
#   SIM_CHECK_SIMAGE( ACTION-IF-FOUND, ACTION-IF-NOT-FOUND, ATTRIBUTE-LIST )
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

AC_DEFUN([SIM_CHECK_SIMAGE], [
AC_PREREQ([2.14.1])

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

  AC_PATH_PROG(sim_ac_conf_cmd, simage-config, false, $sim_ac_path)
  if test "x$sim_ac_conf_cmd" = "xfalse"; then
    AC_MSG_WARN(could not find 'simage-config' in $sim_ac_path)
  fi

  sim_ac_simage_cppflags=`$sim_ac_conf_cmd --cppflags`
  sim_ac_simage_ldflags=`$sim_ac_conf_cmd --ldflags`
  sim_ac_simage_libs=`$sim_ac_conf_cmd --libs`

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
#
# TODO:
#    * [mortene:20000122] make sure this work on MSWin (with
#      Cygwin installation)
#

AC_DEFUN([SIM_CHECK_X11], [
AC_PREREQ([2.14.1])

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
#  SIM_CHECK_OPENGL([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
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
#
# TODO:
#    * [mortene:20000122] make sure this work on MSWin (with
#      Cygwin installation)
#

AC_DEFUN(SIM_CHECK_OPENGL, [

AC_ARG_WITH(
  [opengl],
  AC_HELP_STRING([--with-opengl=DIR],
                 [OpenGL/Mesa installation directory]),
  [],
  [with_opengl=yes])

sim_ac_gl_avail=no

if test x"$with_opengl" != xno; then
  if test x"$with_opengl" != xyes; then
    sim_ac_gl_cppflags="-I${with_opengl}/include"
    sim_ac_gl_ldflags="-L${with_opengl}/lib"
  else
    case "$host_os" in
      hpux*)
        # This is a common location for the OpenGL libraries on HPUX.
        sim_ac_gl_cppflags="-I/opt/graphics/OpenGL/include"
        sim_ac_gl_ldflags="-L/opt/graphics/OpenGL/lib" ;;
    esac
  fi

  sim_ac_save_cppflags=$CPPFLAGS
  sim_ac_save_ldflags=$LDFLAGS

  CPPFLAGS="$CPPFLAGS $sim_ac_gl_cppflags"
  LDFLAGS="$LDFLAGS $sim_ac_gl_ldflags"

  sim_ac_save_libs=$LIBS

  AC_CACHE_CHECK(
    [whether OpenGL libraries are available],
    sim_cv_lib_gl,
    [sim_cv_lib_gl=UNRESOLVED
    # Some platforms (like BeOS) have the GLU functionality in the GL library.
    for sim_ac_gl_libcheck in -lMesaGL -lGL "-lMesaGLU -lMesaGL" "-lGLU -lGL"; do
      if test "x$sim_cv_lib_gl" = "xUNRESOLVED"; then
        LIBS="$sim_ac_gl_libcheck $sim_ac_save_libs"
        AC_TRY_LINK([#include <GL/gl.h>
                    #include <GL/glu.h>],
                    [glPointSize(1.0f);
                    gluSphere(0L, 1.0, 1, 1);],
                    [sim_cv_lib_gl="$sim_ac_gl_libcheck"])
      fi
    done
  ])


  if test "x$sim_cv_lib_gl" != "xUNRESOLVED"; then
    sim_ac_gl_libs="$sim_cv_lib_gl"
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
#   SIM_RTTI_SUPPORT
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
# 
# TODO:
#   * [mortene:19991114] make this work with compilers other than gcc/g++
# 

AC_DEFUN([SIM_RTTI_SUPPORT], [
AC_PREREQ([2.13])
AC_ARG_ENABLE(
  [rtti],
  AC_HELP_STRING([enable-rtti], [(g++ only) compile with RTTI [default=no]]),
  [case "${enableval}" in
    yes) enable_rtti=yes ;;
    no)  enable_rtti=no ;;
    *) AC_MSG_ERROR(bad value \"${enableval}\" for --enable-rtti) ;;
  esac],
  [enable_rtti=no])

if test x"$enable_rtti" = x"no"; then
  if test x"$GXX" = x"yes"; then
    CXXFLAGS="$CXXFLAGS -fno-rtti"
  fi
else
  if test x"$GXX" != x"yes"; then
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
  AC_HELP_STRING([enable-exceptions],
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
    SIM_COMPILER_OPTION([-fno-exceptions], [_exception_flag=-fno-exceptions])
    if test x"${_exception_flag+set}" != x"set"; then
      dnl For GCC versions < 2.8
      SIM_COMPILER_OPTION([-fno-handle-exceptions],
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


# Usage:
#   SIM_COMPILER_OPTION( OPTION-TO-TEST, ACTION-IF-TRUE [, ACTION-IF-FALSE])
#
# Description:
#   Use this file to store miscellaneous macros related to checking
#   compiler features.
#
# Author: Morten Eriksen, <mortene@sim.no>.
#
# TODO:
#   * [mortene:19991125] make SIM_COMPILER_OPTION work with C compilers.
#
#   * [mortene:19991218] improve SIM_COMPILER_OPTION by catching
#     and analyzing stderr (at least to see if there was any output
#     there.)
#

AC_DEFUN([SIM_COMPILER_OPTION], [
AC_PREREQ([2.13])
AC_MSG_CHECKING([whether $CXX accepts $1])
_save_cxxflags=$CXXFLAGS
CXXFLAGS="$CXXFLAGS [$1]"
AC_TRY_COMPILE( [], [], [_accept_result=yes $2], [_accept_result=no $3])
AC_MSG_RESULT([$_accept_result])
CXXFLAGS=$_save_cxxflags
unset _accept_result _save_cxxflags
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
  AC_HELP_STRING([enable-profile],
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

AC_DEFUN([SIM_COMPILER_WARNINGS], [
AC_PREREQ([2.14])
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
    SIM_COMPILER_OPTION([-Wno-multichar], [_warn_flags=-Wno-multichar])
    _warn_flags="-W -Wall -Wno-unused $_warn_flags"

    CFLAGS="$CFLAGS $_warn_flags"
    CXXFLAGS="$CXXFLAGS $_warn_flags"
  else
    case $host in
    *-*-irix*) 
      if test x"$CC" = xcc || test x"$CXX" = xCC; then
        _warn_flags=
        # Turn on all warnings.
        SIM_COMPILER_OPTION([-fullwarn],
                            [_warn_flags="$_warn_flags -fullwarn"])
        # Turn off ``type qualifiers are meaningless in this declaration''
        # warnings.
        SIM_COMPILER_OPTION([-woff 3115],
                            [_warn_flags="$_warn_flags -woff 3115"])
        # Turn off warnings on unused variables.
        SIM_COMPILER_OPTION([-woff 3262],
                            [_warn_flags="$_warn_flags -woff 3262"])

        CFLAGS="$CFLAGS $_warn_flags"
        CXXFLAGS="$CXXFLAGS $_warn_flags"
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
                 [(void)_vsnprintf(0L, 0, 0L);],
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
#   SIM_EXPAND_DIR_VARS
#
# Description:
#   Expand these variables into their correct full directory paths:
#    $prefix  $exec_prefix  $includedir  $libdir
# 
# Author: Morten Eriksen, <mortene@sim.no>.
# 

AC_DEFUN([SIM_EXPAND_DIR_VARS], [
test x"$prefix" = x"NONE" && prefix="$ac_default_prefix"
test x"$exec_prefix" = x"NONE" && exec_prefix="${prefix}"
includedir="`eval echo $includedir`"
libdir="`eval echo $libdir`"
])


