# **************************************************************************

# FIXME: try to fix all problems with spaces in directory names;
# whether it's the src dir, the install/build dir or directories with
# dependent libraries. Alternatively, we should at least detect and
# warn/exit. Ditto goes for all our other software modules aswell, of
# course. 20020519 mortene.

# **************************************************************************

# release version number info
m4_define([COIN_MAJOR], [2])
m4_define([COIN_MINOR], [0])
m4_define([COIN_MICRO], [0])
m4_define([COIN_BETA], [a])

# This is probably a more correct setup, but will be harder to keep if
# we want major and minor to grow slowly (but is that any point?)
# 20010807 larsa
m4_define([COIN_ABI_CURRENT], [m4_eval((COIN_MAJOR*20)+COIN_MINOR)])
m4_define([COIN_ABI_REVISION], [COIN_MICRO])
m4_define([COIN_ABI_AGE], [COIN_MINOR])

# For Mac OS X Compiler Frameworks:
m4_define([MAC_FRAMEWORK_NAME_DEFAULT], [Inventor])
m4_define([MAC_FRAMEWORK_PREFIX_DEFAULT], [/Library/Frameworks])
m4_define([MAC_FRAMEWORK_VERSION_DEFAULT], [B])

# **************************************************************************

AC_PREREQ(2.50)

AC_INIT([Coin], [COIN_MAJOR.COIN_MINOR.COIN_MICRO[]COIN_BETA],
  [coin-support@coin3d.org])

AC_CONFIG_AUX_DIR(cfg)
AC_CONFIG_SRCDIR(src/misc/SoDB.cpp)

# FIXME: detect and exit upon this problem (as seen reported on
# coin-bugs): the install prefix is set to the same directory as the
# build directory (or also as the srccode dir). 20020129 mortene.

SIM_AC_CVS_CHANGES([
  # the moving of "conf-macros" to "cfg/m4"
  SIM_AC_CVS_CHANGE([
    rm -rf conf-macros
    cvs co CoinMacros
  ], [test -d cfg/m4/CVS], [test ! -d conf-macros])
])

AH_TOP([
#include <unconfig.h>
])

AC_CANONICAL_SYSTEM

# For include/Inventor/system/inttypes.h.
AC_DEFINE_UNQUOTED(COIN_CONFIGURE_BUILD, SIM_AS_TR_CPP([$build]), [Build system.])
AC_DEFINE_UNQUOTED(COIN_CONFIGURE_HOST, SIM_AS_TR_CPP([$host]), [Build host system.])
AC_DEFINE_UNQUOTED(COIN_CONFIGURE_TARGET, SIM_AS_TR_CPP([$target]), [Build target system.])

# date used by debian setup
SIM_AC_DATE_RFC822([RFC822_DATE])
AC_SUBST([RFC822_DATE])

# date used by redhat setup
SIM_AC_ISO8601_DATE([coin_configure_date])
AC_SUBST([coin_configure_date])

# If the Microsoft Visual C++ cl.exe compiler is available, set us up for
# compiling with it and to generate an MSWindows .dll file.

AC_ARG_ENABLE([msvc],
  [AC_HELP_STRING([--disable-msvc], [don't require MS Visual C++ on Cygwin])],
  [case $enableval in
  no | false) sim_ac_try_msvc=false ;;
  *)          sim_ac_try_msvc=true ;;
  esac],
  [sim_ac_try_msvc=true])

if $sim_ac_try_msvc; then
  SIM_AC_SETUP_MSVC_IFELSE([], [
    case $host in
    *-cygwin) SIM_AC_ERROR([no-msvc++]) ;;
    esac
  ])
fi

# **************************************************************************
# Mac OS X: gcc

sim_ac_framework=false
sim_ac_framework_name="MAC_FRAMEWORK_NAME_DEFAULT"
sim_ac_framework_prefix="MAC_FRAMEWORK_PREFIX_DEFAULT"
sim_ac_framework_version="MAC_FRAMEWORK_VERSION_DEFAULT"

AM_CONDITIONAL([MAC_FRAMEWORK], [$sim_ac_framework])

AC_SUBST([MAC_FRAMEWORK], [$sim_ac_framework])
AC_SUBST([MAC_FRAMEWORK_NAME], [$sim_ac_framework_name])
AC_SUBST([MAC_FRAMEWORK_PREFIX], [$sim_ac_framework_prefix])
AC_SUBST([MAC_FRAMEWORK_VERSION], [$sim_ac_framework_version])

AC_SUBST([nop], [])

# **************************************************************************

AC_SUBST([COIN_MAJOR_VERSION], [COIN_MAJOR])
AC_SUBST([COIN_MINOR_VERSION], [COIN_MINOR])
AC_SUBST([COIN_MICRO_VERSION], [COIN_MICRO])
AC_SUBST([COIN_BETA_VERSION], [COIN_BETA])
AC_SUBST([COIN_VERSION], [$COIN_MAJOR_VERSION.$COIN_MINOR_VERSION.$COIN_MICRO_VERSION$COIN_BETA_VERSION])
VERSION=$COIN_VERSION

SIM_AC_CONFIGURATION_SETTING([Library version], [$COIN_VERSION])

# Libtool versioning
AC_SUBST([LT_CURRENT], [COIN_ABI_CURRENT])
AC_SUBST([LT_REVISION], [COIN_ABI_REVISION])
AC_SUBST([LT_AGE], [COIN_ABI_AGE])

#  These are for include/Inventor/C/basic.h.

AC_DEFINE_UNQUOTED([COIN_MAJOR_VERSION], [$COIN_MAJOR_VERSION],
  [define this to the libCoin major version number])
AC_DEFINE_UNQUOTED([COIN_MINOR_VERSION], [$COIN_MINOR_VERSION],
  [define this to the libCoin minor version number])
AC_DEFINE_UNQUOTED([COIN_MICRO_VERSION], [$COIN_MICRO_VERSION],
  [define this to the libCoin release version number])
AC_DEFINE_UNQUOTED([COIN_VERSION], ["$COIN_VERSION"],
  [define this to the full libCoin major.minor.micro version number])

# *******************************************************************
# Miscellaneous options and initializations.

coin_build_dir=`pwd`

# **************************************************************************
#  Locate C++ compiler and set C++ as the default language to use
#  in tests. The configure script will automatically terminate if
#  it doesn't find a C++ compiler.
AC_PROG_CC
AC_PROG_CXX
AC_LANG_CPLUSPLUS

# AC_OBJEXT
# AC_EXEEXT

# **************************************************************************
# NOTE: if we touch CFLAGS/CXXFLAGS before AC_PROG_CC is invoked, the -g
# flag will be lost  20020104 larsa

if $BUILD_WITH_MSVC; then
  SIM_AC_SETUP_MSVCRT([
    CPPFLAGS="$CPPFLAGS $sim_ac_msvcrt_CPPFLAGS"
    CFLAGS="$CFLAGS $sim_ac_msvcrt_CFLAGS"
    CXXFLAGS="$CXXFLAGS $sim_ac_msvcrt_CXXFLAGS"
    LDFLAGS="$LDFLAGS $sim_ac_msvcrt_LDFLAGS"
    LIBS="$LIBS $sim_ac_msvcrt_LIBS"
    # sim_ac_msvcrt_LIBLIBS is set up at the end
    # sim_ac_msvcrt_LIBLDFLAGS is set up at the end
    AC_SUBST([COIN_MSVC_LIBC], [$sim_ac_msvcrt])
  ])
fi

# **************************************************************************

AM_INIT_AUTOMAKE(Coin, $VERSION)

#  Default to not building a static library.
#  Can be overridden by the user with --enable-static.
AM_DISABLE_STATIC

#  Turn off default maintainer make-rules -- use ./bootstrap instead.
AM_MAINTAINER_MODE

if $BUILD_WITH_MSVC; then
  RANLIB=:
  export RANLIB
fi

#  Initialize libtool
AC_PROG_LIBTOOL

#  These are used for constructing the coin-config file.
AC_SUBST([COIN_EXTRA_CPPFLAGS], [$CPPFLAGS])
AC_SUBST([COIN_EXTRA_CFLAGS], [])
AC_SUBST([COIN_EXTRA_CXXFLAGS], [])
AC_SUBST([COIN_EXTRA_LDFLAGS], [$LDFLAGS])
AC_SUBST([COIN_EXTRA_LIBS], [$LIBS])

# *******************************************************************
# Compiler checking.

## Make it possible for the user to turn off optimization flags for
## the compiler before running the fatal error checks below.
SIM_AC_COMPILER_OPTIMIZATION

## Smoke out some known defunct compilers.
SIM_AC_COMPILER_CPLUSPLUS_FATAL_ERRORS

## See which way the compiler knows the name of the current function. ####
SIM_AC_CHECK_VAR_FUNCTIONNAME

SIM_AC_MAC_CPP_ADJUSTMENTS

# *******************************************************************
# Enable/disable separate Coin modules

# *******************************************************************
# Search for the vrml97 extension

sim_ac_enable_vrml97=false

AC_ARG_ENABLE(
  [vrml97],
  AC_HELP_STRING([--enable-vrml97], [enable VRML97 support]), [
  case $enableval in
    yes) sim_ac_enable_vrml97=true ;;
    *) ;;
  esac])

HAVE_VRML97=1

if $sim_ac_enable_vrml97 ; then
  AC_DEFINE([HAVE_VRML97],, [define to use the VRML97 extension])
  VRML97MAKEFILE=src/vrml97/Makefile
  SIM_AC_CONFIGURATION_SETTING([VRML97 support], [Yes])
else
  HAVE_VRML97=0
  VRML97MAKEFILE=
  SIM_AC_CONFIGURATION_SETTING([VRML97 support], [No])
fi
AC_SUBST(HAVE_VRML97)

AM_CONDITIONAL([BUILD_VRML97], [$sim_ac_enable_vrml97])

AM_CONDITIONAL([MAC_FRAMEWORK], [false])

# *******************************************************************
# Search for the "local GLU" extension

enable_superglu=false

if test -x ${srcdir}/src/extensions/superglu/configure; then
  enable_superglu=true
  AC_DEFINE(HAVE_SUPERGLU,1, [*EXPERIMENTAL* to use the GLU we've "appropriated" from SGI])
  HAVE_SUPERGLU=1
  AC_CONFIG_SUBDIRS([src/extensions/superglu])
  CPPFLAGS="$CPPFLAGS -I$coin_build_dir/src/extensions"
else
  enable_superglu=false
  HAVE_SUPERGLU=0
fi
AC_SUBST([HAVE_SUPERGLU])

AC_SUBST([SUPERGLUPREFIX], [Super])
export SUPERGLUPREFIX

AM_CONDITIONAL(BUILD_WITH_SUPERGLU, $enable_superglu)

# *******************************************************************

AC_ARG_ENABLE([threads],
  [AC_HELP_STRING([--disable-threads], [disable multithreading support])],
  [case $enableval in
  yes | true ) sim_ac_enable_threads=true ;;
  no | false ) sim_ac_enable_threads=false ;;
  *) AC_MSG_ERROR([invalid arg "$enableval" for --enable-threads option]) ;;
  esac],
  [sim_ac_enable_threads=false])

sim_ac_threads_api="none"

if $sim_ac_enable_threads; then

  # Make it possible to explicitly turn off Win32 threads, to for instance
  # use POSIX threads instead under Win32.
  AC_ARG_ENABLE([w32threads],
    [AC_HELP_STRING([--disable-w32threads], [disable Win32 threads, even when available])],
    [case $enableval in
    yes | true ) sim_ac_w32_enable_threads=true ;;
    no | false ) sim_ac_w32_enable_threads=false ;;
    *) AC_MSG_ERROR([invalid arg "$enableval" for --enable-threads option]) ;;
    esac],
    [sim_ac_w32_enable_threads=true])

  sim_ac_w32thread=false

  # Check for platform-native Win32 thread API first.
  if $sim_ac_w32_enable_threads; then
    AC_MSG_CHECKING([for Win32 threads])
    AC_TRY_LINK(
     [#include <windows.h>],
     [HANDLE h = CreateThread(NULL, 0, NULL, NULL, 0, NULL);
      (void)SetThreadPriority(h, 0);
      ExitThread(0);],
     [sim_ac_w32thread=true
      sim_ac_threads_api="Win32"
      AC_DEFINE([USE_W32THREAD], , [define to use the Win32 threads API])
      AC_DEFINE([COIN_THREADID_TYPE], DWORD, [System dependant thread ID type])])
    AC_MSG_RESULT($sim_ac_w32thread)
  fi

  if ! $sim_ac_w32thread; then
    SIM_AC_CHECK_PTHREAD([
      AC_DEFINE([USE_PTHREAD], , [define to use the POSIX threads API])
      AC_DEFINE([COIN_THREADID_TYPE], pthread_t,
        [System dependant thread ID type])
      sim_ac_threads_api="POSIX"
      COIN_EXTRA_CPPFLAGS="$COIN_EXTRA_CPPFLAGS $sim_ac_pthread_cppflags"
      COIN_EXTRA_LDFLAGS="$COIN_EXTRA_LDFLAGS $sim_ac_pthread_ldflags"
      COIN_EXTRA_LIBS="$sim_ac_pthread_libs $COIN_EXTRA_LIBS"
    ])
  fi
fi

HAVE_THREADS=1
if test "$sim_ac_threads_api" = "none"; then
  HAVE_THREADS=0
else
  AC_DEFINE([HAVE_THREADS],, [*EXPERIMENTAL* to use the threads extension])
fi
AC_SUBST([HAVE_THREADS])

SIM_AC_CONFIGURATION_SETTING([System threads API], [$sim_ac_threads_api])


AM_CONDITIONAL([BUILD_WITH_THREADS], [$sim_ac_enable_threads])

# *******************************************************************
# Headers we might want to use

AC_CHECK_HEADERS([unistd.h sys/types.h inttypes.h stdint.h sys/param.h sys/time.h sys/timeb.h io.h windows.h winsock2.h])

AC_CHECK_HEADERS(
  [sys/unistd.h],
  [sim_ac_have_sys_unistd_h=true],
  [sim_ac_have_sys_unistd_h=false])

if $sim_ac_have_sys_unistd_h; then
  AC_MSG_CHECKING([for IN_PATH define conflict])
  AC_TRY_COMPILE(
   [#include <sys/unistd.h>],
   [void * v; v = (void*) IN_PATH;],
   [sim_ac_in_path_defined=true],
   [sim_ac_in_path_defined=false])
  if $sim_ac_in_path_defined; then
    AC_MSG_RESULT([yes])
    AC_DEFINE([COIN_UNDEF_IN_PATH_HACK],1,
     [define to "remove" conflicting IN_PATH define from <sys/unistd.h>])
  else
    AC_MSG_RESULT([no])
  fi
fi
    
# According to Coin user Ralf Corsepius, at least SunOS4 needs
# to include sys/types.h before netinet/in.h. There have also
# been a problem report for FreeBSD which seems to indicate
# the same dependency on that platform aswell.
#
# FIXME: should we make this into a general macro
# SIM_AC_CHECK_DEPENDENT_HEADER(HEADER, DEPENDONHEADERS..)? 20001025 mortene.
#
# FIXME2: is this really necessary? AC _CHECK_HEADER(S) tests by using the
# preprocessor, not by doing a compile, so we could probably just
# include netinet/in.h in the above list to AC _CHECK_HEADERS. We wouldn't
# catch problems quiet so early on, though. 20001025 mortene.

AC_CACHE_CHECK([for netinet/in.h], sim_cv_netinet_in_h, [
  AC_TRY_CPP([
    #if HAVE_SYS_TYPES_H
    #include <sys/types.h>
    #endif /* HAVE_SYS_TYPES_H */
    #include <netinet/in.h>],
    [sim_cv_netinet_in_h=true], [sim_cv_netinet_in_h=false])])

if $sim_cv_netinet_in_h; then
  AC_DEFINE_UNQUOTED([HAVE_NETINET_IN_H], [1],
    [Define if you have the <netinet/in.h> header file.])
fi

# headers used on Mac OS X
AC_CHECK_HEADERS([AGL/agl.h Carbon/Carbon.h])

# For Inventor/system/inttypes.h.in.
#
# Note: this assumes AC_CHECK_HEADERS(sys/types.h stdint.h) has been done.
#
# FIXME: the header file checking should be AC_REQUIREd, if possible.
# 20010711 mortene.

SIM_AC_DEFINE_BYTESIZE_TYPES


# Test for ntohl() etc.
# disabled after converting from htonl to coin_hton_*() in src/tidbits.c
SIM_AC_BYTEORDER_CONVERSION(
  [COIN_EXTRA_LIBS="$sim_ac_byteorder_conversion_libs $COIN_EXTRA_LIBS"],
  [SIM_AC_ERROR([no-network-conversion])])

# *******************************************************************

sim_ac_build_library=true
AC_ARG_ENABLE(
  [build],
  [AC_HELP_STRING([--disable-build], [disable configuration for library build])],
  [case $enableval in
  no | false) sim_ac_build_library=false ;;
  esac])

AM_CONDITIONAL(BUILD_LIBRARY, $sim_ac_build_library)

# *******************************************************************
# * Do we want to build the documentation?
# *******************************************************************

AC_ARG_VAR([htmldir],
           [destination for HTML docs (default ${datadir}/Coin/html)])

AC_ARG_ENABLE(html,
  AC_HELP_STRING([--enable-html], [build and install Coin HTML documentation]),
  [case $enableval in
    yes | true) want_html=yes ;;
    *)          want_html=no ;;
  esac],
  [want_html=no])

coin_html_dir=`pwd`/html

case $htmldir in
"")
  htmldir="$datadir/Coin/html"
  ;;
/*)
  # do nothing - absolute path
  ;;
*)
  htmldir="\${prefix}/$htmldir"
  ;;
esac

AC_SUBST(htmldir)

AC_ARG_ENABLE(man,
  AC_HELP_STRING([--enable-man], [build and install Coin man pages]),
  [case $enableval in
    yes | true) want_man=yes ;;
    *)          want_man=no ;;
  esac],
  [want_man=no])

coin_man_dir=`pwd`/man

# Used in the Doxygen parameter file.
AC_SUBST([COIN_DOC_HTML], [`echo $want_html | tr '[a-z]' '[A-Z]'`])
AC_SUBST([coin_html_dir])
AC_SUBST([COIN_DOC_MAN], [`echo $want_man | tr '[a-z]' '[A-Z]'`])
AC_SUBST([coin_man_dir])

AM_CONDITIONAL(BUILD_MANPAGES, test x"$want_man" = x"yes")
AM_CONDITIONAL(BUILD_HTMLPAGES, test x"$want_html" = x"yes")

if test x"$want_man" = x"yes"; then
  SIM_AC_CONFIGURATION_SETTING([manpage installation], [$mandir])
fi

if test x"$want_html" = x"yes"; then
  SIM_AC_CONFIGURATION_SETTING([HTML installation], [$htmldir])
fi


if test x"$want_html" != xno -o x"$want_man" != xno; then
  SIM_AC_DOXYGEN_TOOL([], [SIM_AC_ERROR([no-doxygen])])
  sim_ac_doxygen_our_version=1.2.1
  if test x"$sim_ac_doxygen_avail" != x"$sim_ac_doxygen_our_version"; then
    AC_MSG_WARN(You are not using Doxygen version $sim_ac_doxygen_our_version)
  fi

  AC_PATH_PROG(sim_ac_perl_exe, perl, false, $PATH)
  if test x"$sim_ac_perl_exe" = xfalse; then
    AC_MSG_WARN(Could not find the Perl executable)
  fi
fi

# path_tag is used to identify paths in docs/coin.doxygen that needs to be
# transformed using cygpath under cygwin.

case $host in
*-cygwin) path_tag="<PATH>" ;;
*)        path_tag= ;;
esac

AC_SUBST(path_tag)

# *******************************************************************
# Configure an alternate installation?

AC_ARG_WITH(
  [alternate],
  [AC_HELP_STRING([--with-alternate=string],
                  [specify name of alternate configuration])],
  [CONFIG=$withval],
  [CONFIG=default])

AC_ARG_WITH(
  [suffix],
  [AC_HELP_STRING([--with-suffix=string],
                  [specify library name suffix (e.g. "_g")])],
  [SUFFIX=$withval],
  [SUFFIX=])

AC_SUBST(CONFIG)
AC_SUBST(SUFFIX)

# *******************************************************************
# * Check compiler features, bugs, etc.
# *******************************************************************

SIM_AC_COMPILE_DEBUG([
  CPPFLAGS="$CPPFLAGS -DCOIN_DEBUG=1"
], [
  CPPFLAGS="$CPPFLAGS -DCOIN_DEBUG=0"
])

SIM_AC_DEBUGSYMBOLS
SIM_AC_RTTI_SUPPORT
SIM_AC_EXCEPTION_HANDLING
SIM_AC_PROFILING_SUPPORT
SIM_AC_COMPILER_WARNINGS

#  Yep, this is important in case we want to add code to make
#  the API interface "Open Inventor strict", meaning the user must
#  use the exact same include files as under "real" Open Inventor.
#  We would then need to know that we are actually allowed to include
#  any file during internal build.
#
#  The COIN_INTERNAL define is also in use to detect
#  "inefficient including" internally.

CPPFLAGS="$CPPFLAGS -DCOIN_INTERNAL"

# **************************************************************************

# Try to find snprintf() and vsnprintf() on the system.
SIM_AC_CHECK_NPRINTF

SIM_AC_CHECK_MACRO_QUOTE([], [SIM_AC_ERROR([no-cpp-quoting])])
if test x"$sim_ac_quote_hash" = xyes; then
  AC_DEFINE([HAVE_HASH_QUOTING],1,
    [define if preprocessor can quote arguments with the hash symbol])
else
  if test x"$sim_ac_quote_apostrophes" = xyes; then
    AC_DEFINE([HAVE_APOSTROPHES_QUOTING],1,
      [define if preprocessor can quote arguments with apostophes])
  fi
fi


if $sim_ac_build_library; then

# *******************************************************************
# Instead of getenv() on most Win32 platforms (not Windows CE).

AC_MSG_CHECKING([for GetEnvironmentVariable() function])
AC_TRY_LINK([#include <windows.h>],
            [(void)GetEnvironmentVariable("FOOBAR", NULL, 0);],
  [AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE_GETENVIRONMENTVARIABLE],1, [Define to use GetEnvironmentVariable() instead of getenv()])],
  [AC_MSG_RESULT([no])])

# *******************************************************************
#

sim_ac_save_LIBS=$LIBS
LIBS="$LIBS -lwinmm"
AC_MSG_CHECKING([for timeGetTime() function])
AC_TRY_LINK([
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
], [
  (void)timeGetTime();
], [
  AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE_TIMEGETTIME],, [Define to use timeGetTime()])
], [
  AC_MSG_RESULT([no])
  LIBS=$sim_ac_save_LIBS
])

# *******************************************************************
# Test for misc functions to get systemtime with.

AC_MSG_CHECKING([for QueryPerformanceCounter() function])
AC_TRY_LINK([
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
], [
  LARGE_INTEGER l;
  (void)QueryPerformanceFrequency(&l);
  (void)QueryPerformanceCounter(&l);
], [
  AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE_QUERYPERFORMANCECOUNTER],1, [Define to use Win32 QueryPerformanceCounter()])
], [
  AC_MSG_RESULT([no])
])

AC_MSG_CHECKING([for _ftime() function])
AC_TRY_LINK([
#include <windows.h>
#include <sys/types.h>
#include <sys/timeb.h>
], [
  struct _timeb timebuffer;
  _ftime(&timebuffer);
], [
  AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE__FTIME],1, [Define to use Win32 _ftime()])
], [
  AC_MSG_RESULT([no])
])

AC_MSG_CHECKING([for gettimeofday() function])
AC_TRY_LINK([
#include <sys/time.h>
#include <unistd.h>
], [
  struct timeval tmp;
  (void)gettimeofday(&tmp, NULL);
], [
  AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE_GETTIMEOFDAY],1, [Define to use SVr4 / BSD4.3 gettimeofday()])
], [
  AC_MSG_RESULT([no])
])

# *******************************************************************
# Test for misc functions, typically not part of neither ISO C or POSIX.

AC_MSG_CHECKING([for strncasecmp() function])
AC_TRY_LINK([
#include <string.h>
], [
  (void)strncasecmp(NULL, NULL, 0);
], [
  AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE_STRNCASECMP],1, [Define to use BSD4.3 strncasecmp()])
], [
  AC_MSG_RESULT([no])
])



# *******************************************************************
# * Test to see if dynamic linking through either the (Linux/IRIX/...)
# * dl library or the Win32 LoadLibrary() call is available.
# *******************************************************************

coin_can_do_runtime_linking=false

SIM_AC_CHECK_DL([
  AC_DEFINE_UNQUOTED([HAVE_DL_LIB],1, [define if you have the dl library])
  COIN_EXTRA_CPPFLAGS="$COIN_EXTRA_CPPFLAGS $sim_ac_dl_cppflags"
  COIN_EXTRA_LDFLAGS="$COIN_EXTRA_LDFLAGS $sim_ac_dl_ldflags"
  COIN_EXTRA_LIBS="$sim_ac_dl_libs $COIN_EXTRA_LIBS"
  coin_can_do_runtime_linking=true
])

if $coin_can_do_runtime_linking; then
  :
else
  SIM_AC_CHECK_LOADLIBRARY([
    AC_DEFINE_UNQUOTED([HAVE_WINDLL_RUNTIME_BINDING],1,
                       [define if the Win32 LoadLibrary method is available])
    coin_can_do_runtime_linking=true
  ])
fi

# *******************************************************************
# * Set us up for using the simage library (if wanted, and if
# * available).
# *******************************************************************

AC_ARG_ENABLE([dl-simage],
  AC_HELP_STRING([--enable-dl-simage],
                 [use runtime dynamic linking for libsimage if possible [[default=yes]]]),
  [case "${enableval}" in
    yes | true) enable_dl_simage=true ;;
    no | false) enable_dl_simage=false ;;
    *) SIM_AC_ENABLE_ERROR([--enable-dl-simage]) ;;
  esac],
  [enable_dl_simage=true])

if $enable_dl_simage && $coin_can_do_runtime_linking; then
  AC_DEFINE_UNQUOTED([SIMAGE_RUNTIME_LINKING],1,
    [define for runtime linking with simage])
  SIM_AC_CONFIGURATION_SETTING([simage linkage], [run-time binding])
else
  SIM_AC_HAVE_SIMAGE_IFELSE([
    AC_DEFINE([HAVE_LIBSIMAGE],1,
      [define if you have the simage library])
    CPPFLAGS="$CPPFLAGS $sim_ac_simage_cppflags"
    LDFLAGS="$LDFLAGS $sim_ac_simage_ldflags"
    LIBS="$sim_ac_simage_libs $LIBS"
    COIN_EXTRA_CPPFLAGS="$COIN_EXTRA_CPPFLAGS $sim_ac_simage_cppflags"
    COIN_EXTRA_LDFLAGS="$COIN_EXTRA_LDFLAGS $sim_ac_simage_ldflags"
    COIN_EXTRA_LIBS="$sim_ac_simage_libs $COIN_EXTRA_LIBS"
    SIM_AC_CONFIGURATION_SETTING([simage linkage], [link-time binding])
  ], [
    if $sim_ac_simage_desired; then
      AC_MSG_WARN(simage development system not found);
    fi
    SIM_AC_CONFIGURATION_SETTING([simage linkage], [no simage support])
  ])
fi


# *******************************************************************
# * Mathlib stuff.
# *******************************************************************

# Need to check necessity of including "-lm" on the link-line for both
# C and C++ compiled code, as it could differ. We've had a gcc/g++
# on AIX problem with missing "-lm" on the final link-line which I
# suspect to stem from this. <mortene@sim.no>.

AC_LANG_PUSH(C)
SIM_AC_MATHLIB_READY_IFELSE([], [
  SIM_AC_CHECK_MATHLIB(
    [COIN_EXTRA_LIBS="$COIN_EXTRA_LIBS $sim_ac_libm"],
    [SIM_AC_ERROR([no-math-library])])])
AC_LANG_POP(C)

AC_LANG_PUSH(C++)
SIM_AC_MATHLIB_READY_IFELSE([], [
  SIM_AC_CHECK_MATHLIB(
    [COIN_EXTRA_LIBS="$COIN_EXTRA_LIBS $sim_ac_libm"],
    [SIM_AC_ERROR([no-math-library])])])
AC_LANG_POP(C++)


AC_CACHE_CHECK([whether single-precision math functions are available],
  sim_cv_func_singleprec_avail,
  [AC_TRY_LINK([#include <math.h>],
               [(void)sqrtf(1.0f);
                (void)powf(1.0f, 1.0f);
                (void)cosf(1.0f);
                (void)sinf(1.0f);
                (void)tanf(1.0f);
                (void)acosf(1.0f);
                (void)asinf(1.0f);
                (void)atanf(1.0f);
                (void)atan2f(1.0f, 1.0f);],
               sim_cv_func_singleprec_avail=true,
               sim_cv_func_singleprec_avail=false)])

$sim_cv_func_singleprec_avail &&
  AC_DEFINE([HAVE_SINGLEPRECISION_MATH],1,
    [define if single-precision math functions are avialable])

# *******************************************************************
# * Handle the detection and inclusion of X11 libraries.
# *******************************************************************

SIM_AC_CHECK_X11([COIN_EXTRA_CPPFLAGS="$COIN_EXTRA_CPPFLAGS $sim_ac_x11_cppflags"
                  COIN_EXTRA_LDFLAGS="$COIN_EXTRA_LDFLAGS $sim_ac_x11_ldflags"
                  COIN_EXTRA_LIBS="$sim_ac_x11_libs $COIN_EXTRA_LIBS"])

if test x"$sim_ac_x11_avail" = xyes; then
  #  If the X11 shared memory extension is available, include it as
  #  the Mesa library has probably been linked against it.
  SIM_AC_CHECK_X11SHMEM(COIN_EXTRA_LIBS="$sim_ac_x11shmem_libs $COIN_EXTRA_LIBS")
fi

## We need this extra, final X11 check to set up the HAVE_X11_AVAILABLE
## define -- the X_DISPLAY_MISSING define provided by the built-in
## Autoconf X11 check is not strict enough.
SIM_AC_CHECK_X11_READY([AC_DEFINE([HAVE_X11_AVAILABLE],1,
                          [define if it is possible to build against X11])])

# *******************************************************************
# * Test for installation of Mesa or OpenGL development system.
# *******************************************************************

SIM_AC_CHECK_OPENGL([

  AC_DEFINE([HAVE_OPENGL],1, [define if you have OpenGL])

  COIN_EXTRA_CPPFLAGS="$COIN_EXTRA_CPPFLAGS $sim_ac_ogl_cppflags"
  COIN_EXTRA_LDFLAGS="$COIN_EXTRA_LDFLAGS $sim_ac_ogl_ldflags"
  COIN_EXTRA_LIBS="$sim_ac_ogl_libs $COIN_EXTRA_LIBS"

  SIM_AC_CHECK_HEADER_GLEXT([
    CPPFLAGS="$CPPFLAGS $sim_ac_glext_cppflags"
    COIN_EXTRA_CPPFLAGS="$COIN_EXTRA_CPPFLAGS $sim_ac_glext_cppflags"
  ])

  # *******************************************************************
  # ** Check what OpenGL->windowsystem bindings are available.
  # *******************************************************************

  SIM_AC_HAVE_WGL_IFELSE([
    AC_DEFINE([HAVE_WGL], 1, [define if you have WGL Win32 OpenGL bindings])
    LIBS="$LIBS $sim_ac_wgl_libs"
    COIN_EXTRA_LIBS="$COIN_EXTRA_LIBS $sim_ac_wgl_libs"
  ], [
    SIM_AC_HAVE_GLX_IFELSE([
      AC_DEFINE([HAVE_GLX], 1, [define if you have GLX X11 OpenGL bindings])
    ], [
      SIM_AC_HAVE_AGL_IFELSE([
        AC_DEFINE([HAVE_AGL], 1, [define if you have AGL OpenGL bindings])
        LDFLAGS="$LDFLAGS $sim_ac_agl_ldflags"
        COIN_EXTRA_LDFLAGS="$COIN_EXTRA_LDFLAGS $sim_ac_agl_ldflags"
      ], [
        SIM_AC_CONFIGURATION_WARNING([
Could not find or compile or link against any usable window-system
OpenGL binding, neither GLX (OpenGL on X11), nor WGL (OpenGL on Win32)
nor AGL (OpenGL on Mac OS). The SoOffscreenRenderer functionality
will be disabled. Note: this might be a configure bug. Check the file
config.log.])
      ])])])

  if test x"$sim_cv_have_glx" = xtrue; then
    SIM_AC_HAVE_GLXGETPROCADDRESSARB_IFELSE([
      AC_DEFINE([HAVE_GLXGETPROCADDRESSARB],, [define if glXGetProcAddressARB() is available])])
  fi

  if $enable_superglu; then
    SIM_AC_CONFIGURATION_SETTING([GLU linkage], [superglu])
  else
    # *******************************************************************
    # ** Check if GLU should be loaded and linked at runtime (to avoid
    # ** install dependency libCoin -> libGLU).
    # *******************************************************************
    AC_ARG_ENABLE([dl-glu],
      AC_HELP_STRING([--enable-dl-glu],
                     [use runtime dynamic linking for GLU if possible [[default=yes]]]),
      [case "${enableval}" in
        yes | true) enable_dl_glu=true ;;
        no | false) enable_dl_glu=false ;;
        *) SIM_AC_ENABLE_ERROR([--enable-dl-glu]) ;;
      esac],
      [enable_dl_glu=true])
  
    if $coin_can_do_runtime_linking && $enable_dl_glu; then
      # See if GLU is part of GL library (this is the case on BeOS, for
      # instance) before setting us up for runtime linking.
      SIM_AC_GLU_READY_IFELSE([
        AC_DEFINE([HAVE_GLU],1, [define if you have GLU])
        SIM_AC_CONFIGURATION_SETTING([GLU linkage], [part of GL library])
      ], [
        AC_DEFINE_UNQUOTED([GLU_RUNTIME_LINKING],1,
          [define for runtime linking with GLU])
        SIM_AC_CONFIGURATION_SETTING([GLU linkage], [run-time binding])
      ])
    else
      # *******************************************************************
      # ** Check if GLU headers and lib is available on build host.
      # *******************************************************************
      SIM_AC_CHECK_GLU([
        AC_DEFINE([HAVE_GLU],1, [define if you have GLU])
        COIN_EXTRA_CPPFLAGS="$COIN_EXTRA_CPPFLAGS $sim_ac_glu_cppflags"
        COIN_EXTRA_LDFLAGS="$COIN_EXTRA_LDFLAGS $sim_ac_glu_ldflags"
        COIN_EXTRA_LIBS="$sim_ac_glu_libs $COIN_EXTRA_LIBS"
        SIM_AC_CONFIGURATION_SETTING([GLU linkage], [link-time binding])
      ], [
        SIM_AC_CONFIGURATION_SETTING([GLU linkage], [no GLU support])
      ])
    fi
  fi
  ], # OpenGL-linkage ok, end of block
  [ # Couldn't link with OpenGL:
  # Don't stop the configure process if we seem to be configuring to build
  # the documentation (I don't care to install an OpenGL development system
  # on our webserver). 20000703 mortene.
  if test x"$want_html" = xno -a x"$want_man" = xno; then
    # FIXME: should be able to handle systems where OpenGL is not available,
    # as it would be handy for making e.g. import/export-only versions of the
    # library. 20000123 mortene.
    SIM_AC_ERROR([no-opengl])
  else
    SIM_AC_ERROR([no-opengl])
  fi
])

# *******************************************************************
# * Provide this nasty hack to help building Coin on platforms
# * where one can bump into the "Arg list too long" problem
# * (IBM AIX, SGI IRIX 6.2 and IRIX 6.5 with standard settings are
# * known to fail).
# *******************************************************************

AC_ARG_ENABLE(compact,
  AC_HELP_STRING([--enable-compact], [enable hack for compact compilation [[default=no]]]),
  [case "${enableval}" in
    yes | true)  enable_compact=yes ;;
    no | false)  enable_compact=no ;;
    *) SIM_AC_ENABLE_ERROR([--enable-compact]) ;;
  esac],
  enable_compact=no)

AM_CONDITIONAL(HACKING_COMPACT_BUILD, test x"$enable_compact" = x"yes")

# *******************************************************************
# * Enable/disable compilation with the "incremental linking through
# * the use of dynamic libraries"-hack.
# *******************************************************************

AC_ARG_ENABLE(hacking,
  AC_HELP_STRING([--enable-hacking],
                 [enable hacks for quick turn-around cycles during development [[default=no]]]),
  [case "${enableval}" in
    yes | true)  enable_hacking=yes ;;
    no | false)  enable_hacking=no ;;
    *) SIM_AC_ENABLE_ERROR([--enable-hacking]) ;;
  esac],
  enable_hacking=no)

AM_CONDITIONAL(HACKING_DYNAMIC_MODULES, test x"$enable_hacking" = x"yes")

if test x"$enable_hacking" = x"yes"; then
  for dir in \
    base actions bundles caches details draggers elements engines \
    errors events fields manips misc lists nodekits nodes projectors \
    sensors upgraders; \
  do
    HACKING_LIBDIRS="$HACKING_LIBDIRS -L${coin_build_dir}/src/$dir/.libs"
    EXTRA_LIBS="$EXTRA_LIBS -l$dir${SUFFIX}LINKHACK"                  
  done
  if $sim_ac_enable_vrml97; then
    HACKING_LIBDIRS="$HACKING_LIBDIRS -L${coin_build_dir}/src/vrml97/.libs"
    EXTRA_LIBS="$EXTRA_LIBS -lvrml97${SUFFIX}LINKHACK"
  fi
  if $enable_superglu; then
    HACKING_LIBDIRS="$HACKING_LIBDIRS -L${coin_build_dir}/src/extensions/superglu/.libs"
    EXTRA_LIBS="$EXTRA_LIBS -l${SUPERGLUPREFIX}GLU"
  fi
  if $sim_ac_enable_threads; then
    HACKING_LIBDIRS="$HACKING_LIBDIRS -L${coin_build_dir}/src/threads/.libs"
    EXTRA_LIBS="$EXTRA_LIBS -lthreads${SUFFIX}LINKHACK"
  fi
  COIN_HACKING_LIBDIRS="$HACKING_LIBDIRS"
  COIN_EXTRA_LIBS="$EXTRA_LIBS $COIN_EXTRA_LIBS"
fi

AC_SUBST(COIN_HACKING_LIBDIRS)

fi

# *******************************************************************
# * For exposing various build, src and installation paths to
# * misc configuration files under the $top_src_dir/build/ directory.
# *******************************************************************

AC_SUBST([coin_build_dir], [`pwd`])
AC_SUBST([coin_src_dir], [`cd $srcdir; pwd`])

# **************************************************************************
# Remove redundant options from certain option lists.

SIM_AC_UNIQIFY_LIST(COIN_EXTRA_CPPFLAGS, -I$includedir $COIN_EXTRA_CPPFLAGS)
SIM_AC_UNIQIFY_LIST(COIN_EXTRA_LDFLAGS, -L$libdir $COIN_EXTRA_LDFLAGS)
SIM_AC_UNIQIFY_LIST(COIN_EXTRA_LIBS, $COIN_EXTRA_LIBS)
SIM_AC_UNIQIFY_LIST(CPPFLAGS, $CPPFLAGS)
SIM_AC_UNIQIFY_LIST(LDFLAGS, $LDFLAGS)
SIM_AC_UNIQIFY_LIST(LIBS, $LIBS)

case $enable_static in
yes | true)  COIN_STATIC=true ;;
*)           COIN_STATIC=false ;;
esac

case $enable_shared in
yes | true)  COIN_STATIC=false ;;
*)           COIN_STATIC=true ;;
esac

AC_SUBST([LIBFLAGS], [])

# **************************************************************************
# Remaining setup based on platform.

AM_CONDITIONAL(BUILD_WITH_MSVC, $BUILD_WITH_MSVC)

if $sim_ac_build_library; then
  if $BUILD_WITH_MSVC; then
    rm -f vc60.pdb
    LIBFLAGS="$LIBFLAGS $sim_ac_msvcrt_LIBLDFLAGS"
    LIBS="$LIBS $sim_ac_msvcrt_LIBLIBS"

    if $COIN_STATIC; then
      COIN_EXTRA_CPPFLAGS="$COIN_EXTRA_CPPFLAGS -DCOIN_NOT_DLL"
    else
      CPPFLAGS="-DCOIN_MAKE_DLL $CPPFLAGS"
      COIN_EXTRA_CPPFLAGS="$COIN_EXTRA_CPPFLAGS -DCOIN_DLL"
    fi

    debugfile="`pwd`/src/coin${COIN_MAJOR_VERSION}${SUFFIX}.pdb"
    debugfile=`cygpath -w "$debugfile" | sed 's,\\\\,\\\\\\\\,g'`

    if $COIN_STATIC; then
      SIM_AC_CONFIGURATION_SETTING([Coin build type], [static .lib])
      LIBFLAGS="-LIB $LIBFLAGS"
      LIBFLAGS="$LIBFLAGS /OUT:coin$COIN_MAJOR_VERSION$SUFFIX.lib"
    else
      LIBFLAGS="-DLL /INCREMENTAL:NO $LIBFLAGS"
      case $enable_symbols in
      no | false)
        SIM_AC_CONFIGURATION_SETTING([Coin build type],
                                     [dynamic .dll, no symbols])
        LIBFLAGS="$LIBFLAGS /RELEASE"
        ;;
      *)
        SIM_AC_CONFIGURATION_SETTING([Coin build type],
                                     [dynamic .dll + .pdb])
        CFLAGS="/Fd$debugfile $CFLAGS"
        CXXFLAGS="/Fd$debugfile $CXXFLAGS"
        LIBFLAGS="$LIBFLAGS /DEBUG /PDB:coin$COIN_MAJOR_VERSION$SUFFIX.pdb"
        ;;
      esac
      LIBFLAGS="$LIBFLAGS /OUT:coin$COIN_MAJOR_VERSION$SUFFIX.dll"
    fi
    SIM_AC_CONFIGURATION_SETTING([C library version], [$sim_ac_msvcrt])
  
    # We use a version suffix on the .dll-file, so several incompatible
    # (major) versions can be installed on a system.
    #
    # BTW, when linking DLLs, the 3rd-party .lib files will be
    # linked into the .dll file. I believe it is still advisable to
    # list all libs used upon `coin-config --libs`, as we can then
    # also use them from "parent" code (remember that their interfaces
    # is not exposed from the DLL) without any fuss.
    if test -n "$COIN_EXTRA_LIBS"; then
      COIN_EXTRA_LIBS="-lcoin${COIN_MAJOR_VERSION}${SUFFIX} $COIN_EXTRA_LIBS"
    else
      COIN_EXTRA_LIBS="-lcoin${COIN_MAJOR_VERSION}${SUFFIX}"
    fi
  else
    if test -n "$COIN_EXTRA_LIBS"; then
      COIN_EXTRA_LIBS="-lCoin${SUFFIX} $COIN_EXTRA_LIBS"
    else
      COIN_EXTRA_LIBS="-lCoin${SUFFIX}"
    fi

  fi

  COIN_COMPILER="$CXX"
  if $BUILD_WITH_MSVC; then
    # we'll use the installed wrapper when we use the coin-config script later
    # so we can remove the source code hierarchy
    COIN_COMPILER="wrapmsvc"
  fi
  AC_SUBST(COIN_COMPILER)

  if test x"$prefix" = x"NONE"; then
    SIM_AC_CONFIGURATION_SETTING([Install prefix], [/usr/local])
  else
    SIM_AC_CONFIGURATION_SETTING([Install prefix], [$prefix])
  fi
else
  SIM_AC_CONFIGURATION_SETTING([Coin build type], [build disabled])
fi

# **************************************************************************

AM_CONFIG_HEADER(include/config.h)

# **************************************************************************

# ATTENTION [20011120 larsa]
# The Makefile list is now automatically patched by bootstrap before
# Autoconf is run for a second time.  Make sure the bootstrap process works
# without any error messages before you commit changes to this part.

vrml97makefile=src/vrml97/Makefile
if $sim_ac_enable_vrml97; then :; else
  vrml97makefile=
fi

AC_CONFIG_FILES([
        Makefile
	bin/Makefile
        include/Makefile
        include/Inventor/Makefile
	include/Inventor/C/Makefile
	include/Inventor/C/base/Makefile
	include/Inventor/C/errors/Makefile
	include/Inventor/C/threads/Makefile
        include/Inventor/lock/Makefile
        include/Inventor/system/Makefile
        data/Makefile
        data/draggerDefaults/Makefile
        man/Makefile
        man/man1/Makefile
        man/man3/Makefile
        html/Makefile
        src/Makefile
        src/base/Makefile
        src/actions/Makefile
        src/bundles/Makefile
        src/caches/Makefile
        src/details/Makefile
        src/draggers/Makefile
        src/elements/Makefile
        src/engines/Makefile
        src/errors/Makefile
        src/events/Makefile
        src/fields/Makefile
        src/manips/Makefile
        src/misc/Makefile
        src/lists/Makefile
        src/nodekits/Makefile
        src/nodes/Makefile
        src/projectors/Makefile
        src/sensors/Makefile
        src/upgraders/Makefile
        src/threads/Makefile
        src/extensions/Makefile
        src/vrml97/Makefile
        docs/coin.doxygen
])

AC_OUTPUT

# *******************************************************************
# * Platform-specific stuff.
# *******************************************************************

if $sim_ac_build_library; then
  ac_coin_untested_platform=true
  ac_coin_not_sim_platform=true
else
  ac_coin_untested_platform=false
  ac_coin_not_sim_platform=false
fi

case "$host_os" in
  irix6.2)
    case $CXX in
    CC | */CC ) ac_coin_untested_platform=false ;;
    esac
    ;;
  irix6.5)
    case $CXX in
    CC | */CC )
      ac_coin_untested_platform=false
      ac_coin_not_sim_platform=false
      ;;
    esac
    ;;
  aix*)
    case $CXX in
    xlC | */xlC ) ac_coin_untested_platform=false ;;
    esac
    ;;
  solaris*)
    case $CXX in
    g++ ) ac_coin_untested_platform=false ;;
    esac
    ;;
  hpux10.20)
    case $CXX in
    aCC | */aCC ) ac_coin_untested_platform=false ;;
    esac
    ;;
  linux-gnu)
    case "$host_cpu" in
    i?86)
      ac_coin_untested_platform=false
      ac_coin_not_sim_platform=false
    ;;
    esac
    ;;
  cygwin*)
    case "$CXX" in
    *wrapmsvc*)
      ac_coin_untested_platform=false
      ac_coin_not_sim_platform=false
    ;;
    esac
    ;;
  darwin*)
    if test "$CXX" = c++; then ac_coin_unknown_platform=false; fi
    ;;
  beos)
    case "$host_cpu" in
      i?86)
        if test "$CXX" = c++; then ac_coin_untested_platform=false; fi
        ;;
    esac
    ;;
esac

if $ac_coin_untested_platform; then
  echo
  echo "************************* WARNING ****************************"
  echo "*"
  echo "* We have not tested Coin on the $host_os $host_cpu"
  echo "* platform with the $CXX C++ compiler. Please report"
  echo "* back to us at <coin-support@coin3d.org> how it works out."
  echo "*"
  echo "**************************************************************"
  echo
else
  if $ac_coin_not_sim_platform; then
    echo
    echo "NOTE: This platform ($host_os $host_cpu with the $CXX C++ compiler)"
    echo "      is not among our internal development systems. Please let us"
    echo "      know at <coin-support@coin3d.org> if you run into any problems."
    echo
  fi
fi

if $sim_ac_build_library; then
  case "$host_os" in
  irix*)
    echo ""
    echo " Detected SGI IRIX. There are some hints regarding this platform"
    echo " in the file "
    echo ""
    echo "           ${srcdir}/README.IRIX"
    echo ""
    echo " Please take a look if this is your first time building on IRIX."
    echo ""
    ;;
  aix*)
    echo ""
    echo " Detected IBM AIX. There are some hints regarding this platform"
    echo " in the file "
    echo ""
    echo "           ${srcdir}/README.AIX"
    echo ""
    echo " Please take a look if this is your first time building on AIX."
    echo ""
    ;;
  esac
fi

SIM_AC_CONFIGURATION_SUMMARY

if $sim_ac_build_library; then
  cat <<EOF

Warning: You are now building the development version of the Coin library.
  Unless you are working on development of Coin, you will probably want to
  build the sources from the Coin-1.* release branch instead.  This
  development branch may become ABI-incompatible with itself from time
  to time, with all the headaches that follow...

EOF
else
  echo ""
fi

