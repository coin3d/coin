cmake_minimum_required(VERSION 3.0)

set(COIN_MAJOR_VERSION 4)
set(COIN_MINOR_VERSION 0)
set(COIN_MICRO_VERSION 0)
set(COIN_BETA_VERSION a)
set(COIN_VERSION ${COIN_MAJOR_VERSION}.${COIN_MINOR_VERSION}.${COIN_MICRO_VERSION}${COIN_BETA_VERSION})

project(Coin VERSION ${COIN_MAJOR_VERSION}.${COIN_MINOR_VERSION}.${COIN_MICRO_VERSION})
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

string(TIMESTAMP COIN_BUILD_YEAR "%Y")
math(EXPR COIN_SO_VERSION ${PROJECT_VERSION_MAJOR}*20)

# ############################################################################
# these will be removed after upgrading CMake minimum version
set(PROJECT_DESCRIPTION   "A high-level 3D visualization library with Open Inventor 2.1 API")
# ############################################################################

# get rid of OpenGL GLVND warning from CMake 3.11
if (NOT CMAKE_VERSION VERSION_LESS "3.11") # CMake 3.5 has no VERSION_GREATER_EQUAL
  cmake_policy(SET CMP0072 NEW)
endif()

# ############################################################################
# Prevent insource builds, as they often cause severe build problems
# ############################################################################

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "${CMAKE_PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake <path_to_${CMAKE_PROJECT_NAME}> [options]' from there.")
endif()

# ############################################################################
# Include necessary submodules
# ############################################################################

# Add path for Coin specific helper scripts
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include(CheckCSourceRuns)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckStructHasMember)
include(CheckTypeSize)
include(CMakeDependentOption)
include(GNUInstallDirs)
include(FeatureSummary)
include(CoinCMakeUtilities)

# ############################################################################
# Provide options to customise the build
# ############################################################################

option(COIN_BUILD_SHARED_LIBS "Build shared library when ON (default), static when OFF." ON)
option(COIN_BUILD_TESTS "Build unit tests when ON (default), skips them when OFF." ON)
option(COIN_BUILD_DOCUMENTATION "Create and install API documentation (requires Doxygen)" ON)
cmake_dependent_option(COIN_BUILD_INTERNAL_DOCUMENTATION "Document internal code not part of the API (depends on COIN_BUILD_DOCUMENTATION)" OFF "COIN_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(COIN_BUILD_DOCUMENTATION_MAN "Build and install Coin man pages." OFF "COIN_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(COIN_BUILD_DOCUMENTATION_QTHELP "Generate QtHelp documentation files" OFF "COIN_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(COIN_BUILD_DOCUMENTATION_CHM "Create compressed HTML help (requires HTML help compiler)" OFF "COIN_BUILD_DOCUMENTATION" OFF)

option(COIN_THREADSAFE "Enable thread safe render traversals." OFF)
option(HAVE_VRML97 "Enable VRML97 support." ON)
cmake_dependent_option(COIN_HAVE_JAVASCRIPT "Enable JavaScript capabilities for VRML97." ON "HAVE_VRML97" OFF)
option(HAVE_NODEKITS "Enable NodeKit support." ON)
cmake_dependent_option(HAVE_DRAGGERS "Enable Dragger support." ON "HAVE_NODEKITS" OFF)
cmake_dependent_option(HAVE_MANIPULATORS "Enable Manipulator support." ON "HAVE_NODEKITS" OFF)
option(HAVE_SOUND "Enable OpenAL sound support." ON)
option(HAVE_3DS_IMPORT_CAPABILITIES "Enable 3ds import capabilities." ON)
option(USE_EXTERNAL_EXPAT "Use system install of expat." OFF)
option(USE_EXCEPTIONS "Compile with exceptions (g++ only)." ON)
option(USE_SUPERGLU "Use superglu library when ON, GLU implementation of platform when OFF (default)." OFF)

option(FONTCONFIG_RUNTIME_LINKING   "Enable FontConfig runtime linking when ON (default), disable when OFF." ON)
option(FREETYPE_RUNTIME_LINKING     "Enable FreeType runtime linking when ON (default), disable when OFF." ON)
option(LIBBZIP2_RUNTIME_LINKING     "Enable libBZip2 runtime linking when ON (default), disable when OFF." ON)
option(OPENAL_RUNTIME_LINKING       "Enable OpenAL runtime linking when ON (default), disable when OFF." ON)
option(SIMAGE_RUNTIME_LINKING       "Enable simage runtime linking when ON (default), disable when OFF" ON)
option(ZLIB_RUNTIME_LINKING         "Enable ZLib runtime linking when ON (default), disable when OFF." ON)
option(GLU_RUNTIME_LINKING          "Enable GLU runtime linking when ON (default), disable when OFF." ON)
option(SPIDERMONKEY_RUNTIME_LINKING "Enable SpiderMonkey runtime linking when ON (default), disable when OFF." ON)

option(COIN_VERBOSE "Add verbose debugging information during the configure process." OFF)
option(HAVE_MULTIPLE_VERSION "Forces versioned paths for includes and documentation when ON, usual behaviour otherwise." OFF)

if(HAVE_MULTIPLE_VERSION)
  # Modifies the installation path for both the distributed includes and docs
  versionize(INCLUDEDIR DOCDIR)
  dump_variable(CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_FULL_INCLUDEDIR CMAKE_INSTALL_DOCDIR CMAKE_INSTALL_FULL_DOCDIR)
endif()

# ############################################################################
# Find all necessary and optional Coin3D dependencies
# ############################################################################

# Fail early if one of the required packages cannot be found

# This is needed when Coins local Boost includes are removed and system installation of Boost is used.
#find_package(Boost REQUIRED) # almost any Boost will do
#list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})

# As not all CMake FindXXX modules and config packages provide target support,
# we should define them on our own.
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  set(HAVE_OPENGL 1)
  set(OPENGL_SYSTEM_LIBRARY_NAME ${OPENGL_gl_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX})
  if(OpenGL_GLU_FOUND)
    set(HAVE_GLU 1)
  endif()
  if(APPLE)
    set(GLU_IS_PART_OF_GL 1)
  endif()
  include_directories(${OPENGL_INCLUDE_DIR})
  if (NOT TARGET OpenGL::GL)
    list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${OPENGL_INCLUDE_DIR})
    list(APPEND COIN_TARGET_LINK_LIBRARIES ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
  else()
    list(APPEND COIN_TARGET_LINK_LIBRARIES OpenGL::GL OpenGL::GLU)
  endif()
endif()

set(CMAKE_THREAD_PREFER_PTHREAD 1)
find_package(Threads)
if(Threads_FOUND)
  set(HAVE_THREADS 1)
  if(CMAKE_USE_WIN32_THREADS_INIT)
    set(USE_W32THREAD 1)
    set(COIN_THREADID_TYPE DWORD)
  elseif(CMAKE_USE_PTHREADS_INIT)
    set(USE_PTHREAD 1)
    set(COIN_THREADID_TYPE pthread_t)
  endif()
  #include_directories(${THREADS_INCLUDE_DIRS})
  if (NOT TARGET Threads::Threads)
    list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${THREADS_INCLUDE_DIRS})
    list(APPEND COIN_TARGET_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
  else()
    list(APPEND COIN_TARGET_LINK_LIBRARIES Threads::Threads)
  endif()
endif()

if(USE_EXTERNAL_EXPAT)
  find_package(EXPAT)
  if(EXPAT_FOUND)
    set(HAVE_EXPAT 1)
    #include_directories(${EXPAT_INCLUDE_DIRS})
    if (NOT TARGET EXPAT::EXPAT)
      list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${EXPAT_INCLUDE_DIRS})
      list(APPEND COIN_TARGET_LINK_LIBRARIES ${EXPAT_LIBRARIES})
    else()
      list(APPEND COIN_TARGET_LINK_LIBRARIES EXPAT::EXPAT)
    endif()
  endif()
endif()

if(USE_SUPERGLU)
  find_package(superglu)
  if(superglu_FOUND)
    set(HAVE_SUPERGLU 1)
    set(GLU_RUNTIME_LINKING OFF)
    list(REMOVE_ITEM COIN_TARGET_LINK_LIBRARIES ${OPENGL_glu_LIBRARY})
    if(TARGET OpenGL::GLU)
      list(REMOVE_ITEM COIN_TARGET_LINK_LIBRARIES OpenGL::GLU)
    endif()
    list(APPEND COIN_TARGET_LINK_LIBRARIES superglu::GLU)
  endif()
endif()

#find_package(FLEX)
#if(FLEX_FOUND)
# set(HAVE_FLEX 1)
# #include_directories(${FLEX_INCLUDE_DIRS})
# #list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${FLEX_INCLUDE_DIRS})
#endif()
#
#find_package(BISON)
#if(BISON_FOUND)
# set(HAVE_BISON 1)
# #include_directories(${BISON_INCLUDE_DIRS})
# #list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${BISON_INCLUDE_DIRS})
#endif()

if(HAVE_SOUND)
  if(NOT OPENAL_RUNTIME_LINKING)
    find_package(OpenAL)
    if(OpenAL_FOUND)
      set(HAVE_OPENAL 1)
      #include_directories(${OPENAL_INCLUDE_DIRS})
      list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${OPENAL_INCLUDE_DIRS})
      list(APPEND COIN_TARGET_LINK_LIBRARIES ${OPENAL_LIBRARY})
      check_include_file(AL/al.h HAVE_AL_AL_H)
      check_include_file(OpenAL/al.h HAVE_OPENAL_AL_H)
    endif()
  endif()
endif()

if(NOT ZLIB_RUNTIME_LINKING)
  find_package(ZLIB)
  if(ZLIB_FOUND)
    set(HAVE_ZLIB 1)
    #include_directories(${ZLIB_INCLUDE_DIRS})
    if (NOT TARGET ZLIB::ZLIB)
      list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRS})
      list(APPEND COIN_TARGET_LINK_LIBRARIES ${ZLIB_LIBRARIES})
    else()
      list(APPEND COIN_TARGET_LINK_LIBRARIES ZLIB::ZLIB)
    endif()
  endif()
endif()

if(NOT LIBBZIP2_RUNTIME_LINKING)
  find_package(BZip2)
  if(BZip2_FOUND)
    set(HAVE_BZIP2 1)
    #include_directories(${BZIP2_INCLUDE_DIRS})
    if (NOT TARGET BZip2::BZip2)
      list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${BZIP2_INCLUDE_DIRS})
      list(APPEND COIN_TARGET_LINK_LIBRARIES ${BZIP2_LIBRARIES})
    else()
      list(APPEND COIN_TARGET_LINK_LIBRARIES BZip2::BZip2)
    endif()
  endif()
endif()

if(NOT FREETYPE_RUNTIME_LINKING)
  find_package(Freetype)
  if(Freetype_FOUND)
    set(HAVE_FREETYPE 1)
    #include_directories(${FREETYPE_INCLUDE_DIRS})
    if (NOT TARGET Freetype::Freetype)
      list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${FREETYPE_INCLUDE_DIRS})
      list(APPEND COIN_TARGET_LINK_LIBRARIES ${FREETYPE_LIBRARIES})
    else()
      list(APPEND COIN_TARGET_LINK_LIBRARIES Freetype::Freetype)
    endif()
    check_include_file(ft2build.h HAVE_FREETYPE_H)
  endif()
endif()

if(NOT FONTCONFIG_RUNTIME_LINKING)
  find_package(Fontconfig)
  if(FONTCONFIG_FOUND)
    set(HAVE_FONTCONFIG 1)
    #include_directories(${FONTCONFIG_INCLUDE_DIR})
    list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${FONTCONFIG_INCLUDE_DIR})
    list(APPEND COIN_TARGET_LINK_LIBRARIES ${FONTCONFIG_LIBRARY})
    check_include_file(fontconfig/fontconfig.h HAVE_FONTCONFIG_H)
  endif()
endif()

if(HAVE_VRML97 AND COIN_HAVE_JAVASCRIPT)
  if(NOT SPIDERMONKEY_RUNTIME_LINKING)
    find_package(SpiderMonkey)
    if(SPIDERMONKEY_FOUND)
      set(HAVE_SPIDERMONKEY_VIA_LINKTIME_LINKING 1)
      #include_directories(${SPIDERMONKEY_INCLUDE_DIRS})
      list(APPEND COIN_TARGET_INCLUDE_DIRECTORIES ${SPIDERMONKEY_INCLUDE_DIRS})
      list(APPEND COIN_TARGET_LINK_LIBRARIES ${SPIDERMONKEY_LIBRARIES})
    endif()
  endif()
endif()

if(NOT SIMAGE_RUNTIME_LINKING)
  find_package(simage)
  if(simage_FOUND)
    set(HAVE_LIBSIMAGE ON)
    list(APPEND COIN_TARGET_LINK_LIBRARIES simage::simage)
  else()
    set(HAVE_LIBSIMAGE OFF)
  endif()
endif()

message(STATUS "${COIN_TARGET_INCLUDE_DIRECTORIES}")
message(STATUS "${COIN_TARGET_LINK_LIBRARIES}")

# ##########################################################################
# Setup build environment
# ##########################################################################

if(NOT CMAKE_BUILD_TYPE)
  # Has no effect for multi configuration generators (VisualStudio, Xcode).
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose type of build, options are Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()
# Set common output directories for all targets built.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Checks for existence of specified types and set variable to the first type found
find_int_type_with_size(COIN_INT8_T 1 "int8_t" "char")
find_int_type_with_size(COIN_UINT8_T 1 "uint8_t" "u_int8_t" "unsigned char")
find_int_type_with_size(COIN_INT16_T 2 "int16_t" "short" "int")
find_int_type_with_size(COIN_UINT16_T 2 "uint16_t" "u_int16_t" "unsigned short" "unsigned int")
find_int_type_with_size(COIN_INT32_T 4 "int32_t" "int" "long")
find_int_type_with_size(COIN_UINT32_T 4 "uint32_t" "u_int32_t" "unsigned int" "unsigned long")
find_int_type_with_size(COIN_INT64_T 8 "int64_t" "long" "int" "long long" "__int64")
find_int_type_with_size(COIN_UINT64_T 8 "uint64_t" "u_int64_t" "unsigned long" "unsigned int" "unsigned long long" "unsigned __int64")
find_int_type_with_size(COIN_INTPTR_T ${CMAKE_SIZEOF_VOID_P} "intptr_t" "int" "long" "long long" "__int64")
find_int_type_with_size(COIN_UINTPTR_T ${CMAKE_SIZEOF_VOID_P} "uintptr_t" "u_intptr_t" "_W64 unsigned int" "unsigned int" "unsigned long" "u_int64_t" "unsigned long long" "unsigned __int64")

if(MSVC)
  option(COIN_BUILD_SINGLE_LIB "Build only one library when ON, multiple when OFF." ON)
  if (MSVC_VERSION GREATER 1500 OR MSVC_VERSION EQUAL 1500)
    option(COIN_BUILD_MSVC_MP "Enable build with multiple processes in Visual Studio" ON)
  else()
    set(COIN_BUILD_MSVC_MP OFF CACHE BOOL "Compiler option /MP requires at least Visual Studio 2008 (VS9) or newer" FORCE)
  endif()
  if(COIN_BUILD_MSVC_MP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  endif ()
  if(COIN_BUILD_SHARED_LIBS)
    add_definitions(-DCOIN_MAKE_DLL)
  endif()

  # Enable C++ exception handling (for code that uses boost in SbByteBuffer)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS)
else()
  option(COIN_BUILD_SINGLE_LIB "Build only one library when ON, multiple when OFF." OFF)
endif()

set(HAVE_GZDOPEN 1)
if(WIN32)
  set(HAVE_WIN32_API 1)
  set(HAVE_WGL 1)
  # On Windows the major version number is part of the library name
  if(COIN_BUILD_SHARED_LIBS)
    set(CMAKE_RELEASE_POSTFIX ${PROJECT_VERSION_MAJOR})
    set(CMAKE_MINSIZEREL_POSTFIX ${PROJECT_VERSION_MAJOR})
    set(CMAKE_RELWITHDEBINFO_POSTFIX ${PROJECT_VERSION_MAJOR})
    set(CMAKE_DEBUG_POSTFIX ${PROJECT_VERSION_MAJOR}d)
  else()
    set(CMAKE_RELEASE_POSTFIX ${PROJECT_VERSION_MAJOR}s)
    set(CMAKE_MINSIZEREL_POSTFIX ${PROJECT_VERSION_MAJOR}s)
    set(CMAKE_RELWITHDEBINFO_POSTFIX ${PROJECT_VERSION_MAJOR}s)
    set(CMAKE_DEBUG_POSTFIX ${PROJECT_VERSION_MAJOR}sd)
  endif()
elseif(APPLE)
  cmake_policy(SET CMP0042 NEW)
  configure_file(Info.plist.in Info.plist)
  configure_file(version.plist.in version.plist)
  set(CMAKE_SHARED_LINKER_FLAGS "-framework CoreFoundation -framework CoreGraphics")
  set(HAVE_GZDOPEN)

  # Let's enable all OS X specific code
  set(COIN_MACOSX_FRAMEWORK 1)
  set(COIN_MACOS_10 1)
  set(COIN_MACOS_10_3 1)
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include_directories(${CMAKE_BINARY_DIR}/include)

check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stddef.h HAVE_STDDEF_H)
check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(sys/timeb.h HAVE_SYS_TIMEB_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/param.h HAVE_SYS_PARAM_H)
check_include_file(io.h HAVE_IO_H)
check_include_file(ieeefp.h HAVE_IEEEFP_H)
check_include_file(time.h HAVE_TIME_H)
check_include_files("stdlib.h;stdarg.h;string.h;float.h" STDC_HEADERS)

# I do not have any FreeBSD knowledge, but should this not have been handled already by the 
# find_package(OpenGL) and the following include_directories call?

# On FreeBSD (possibly other BSDs as well), GL/gl.h does not live
# in the standard search paths. Test for it explicitly.
if(CMAKE_SYSTEM MATCHES "FreeBSD")
  if(NOT HAVE_OPENGL_GL_H)
    include_directories(/usr/local/include)
    check_include_file(GL/gl.h _have_gl_gl_h)
    if (NOT _have_gl_gl_h)
      message(WARNING "No GL/gl.h found")
    endif()
  endif()
endif()

check_include_file(windows.h HAVE_WINDOWS_H)
if(HAVE_WINDOWS_H)
  check_include_file(direct.h HAVE_DIRECT_H)
  check_include_files("windows.h;tlhelp32.h" HAVE_TLHELP32_H)
  check_symbol_exists(_fpclass float.h HAVE__FPCLASS)
  check_symbol_exists(_splitpath stdlib.h HAVE__SPLITPATH)
  check_symbol_exists(LoadLibrary windows.h HAVE_WINDLL_RUNTIME_BINDING)
  check_symbol_exists(GetEnvironmentVariable windows.h HAVE_GETENVIRONMENTVARIABLE)
  check_symbol_exists(QueryPerformanceCounter windows.h HAVE_QUERYPERFORMANCECOUNTER)
else()
  check_include_file(libgen.h HAVE_LIBGEN_H)
  check_symbol_exists(gettimeofday sys/time.h HAVE_GETTIMEOFDAY)
  check_include_file(mach-o/dyld.h HAVE_MACH_O_DYLD_H)
  if(HAVE_MACH_O_DYLD_H)
    check_symbol_exists(NSLookupAndBindSymbol mach-o/dyld.h HAVE_DYLD_RUNTIME_BINDING)
  endif()
  check_library_exists(dl dlopen "" HAVE_DL_LIB)
  if(HAVE_DL_LIB)
    check_include_file(dlfcn.h HAVE_DLFCN_H)
  endif()
  check_library_exists(dld shl_load "" HAVE_DLD_LIB)

  check_include_file(X11/Xlib.h HAVE_X11_AVAILABLE)
endif()

if(HAVE_WINDOWS_H)
  check_include_files("windows.h;GL/gl.h" HAVE_GL_GL_H)
  check_include_files("windows.h;GL/glu.h" HAVE_GL_GLU_H)
  check_include_files("windows.h;GL/gl.h;GL/glext.h" HAVE_GL_GLEXT_H)
else()
  check_include_files("GL/gl.h" HAVE_GL_GL_H)
  check_include_files("GL/glu.h" HAVE_GL_GLU_H)
  check_include_files("GL/gl.h;GL/glext.h" HAVE_GL_GLEXT_H)
  check_include_files("GL/glx.h" HAVE_GLX_H)
endif()

check_include_file(OpenGL/gl.h HAVE_OPENGL_GL_H)
if(HAVE_OPENGL_GL_H)
  check_include_file(OpenGL/glu.h HAVE_OPENGL_GLU_H)
  check_include_file(OpenGL/glext.h HAVE_OPENGL_GLEXT_H)
  check_include_file(OpenGL/CGLCurrent.h HAVE_OPENGL_CGLCURRENT_H)
endif()

#if(EXISTS ${CMAKE_SOURCE_DIR}/src/extensions/superglu) 
# set(HAVE_SUPERGLU 1)
# set(GLU_RUNTIME_LINKING OFF)
# list(REMOVE_ITEM COIN_TARGET_LINK_LIBRARIES ${OPENGL_glu_LIBRARY})
#endif()

if(HAVE_WINDOWS_H)
  set(SIM_INCLUDE_WINDOWS_H "#include <windows.h>")
else()
  set(SIM_INCLUDE_WINDOWS_H "/* #include <windows.h> - not needed on system */")
endif()
if(HAVE_GL_GL_H)
  set(SIM_INCLUDE_GL_H "#include <GL/gl.h>")
elseif(HAVE_OPENGL_GL_H)
  set(SIM_INCLUDE_GL_H "#include <OpenGL/gl.h>")
else()
  set(SIM_INCLUDE_GL_H "#error \"don't know how to include gl.h header\"")
endif()
if(HAVE_SUPERGLU)
  set(SIM_INCLUDE_GLU_H "/* #include <GL/glu.h> - not used, Coin linked with embedded SuperGLU */")
elseif(HAVE_GL_GLU_H)
  set(SIM_INCLUDE_GLU_H "#include <GL/glu.h>")
elseif(HAVE_OPENGL_GLU_H)
  set(SIM_INCLUDE_GLU_H "#include <OpenGL/glu.h>")
else()
  set(SIM_INCLUDE_GLU_H "/* #include <GL/glu.h> - not found on system */")
endif()
if(HAVE_GL_GLEXT_H)
  set(SIM_INCLUDE_GLEXT_H "#include <GL/glext.h>")
elseif(HAVE_OPENGL_GLEXT_H)
  set(SIM_INCLUDE_GLEXT_H "#include <OpenGL/glext.h>")
else()
  set(SIM_INCLUDE_GLEXT_H "/* #include <GL/glext.h> - not found on system */")
endif()

check_symbol_exists(va_copy stdarg.h HAVE_VA_COPY_MACRO)
check_symbol_exists(strncasecmp string.h HAVE_STRNCASECMP)
check_symbol_exists(memmove string.h HAVE_MEMMOVE)
check_symbol_exists(bcopy strings.h HAVE_BCOPY)
check_symbol_exists(fstat "sys/stat.h;sys/types.h" HAVE_FSTAT)
if(NOT HAVE_FSTAT)
  check_symbol_exists(_fstat "sys/stat.h;sys/types.h" HAVE__FSTAT)
endif()
check_symbol_exists(ftime "sys/types.h;sys/timeb.h" HAVE_FTIME)
if(NOT HAVE_FTIME)
  check_symbol_exists(_ftime "sys/types.h;sys/timeb.h" HAVE__FTIME)
endif()
check_symbol_exists(getcwd unistd.h HAVE_GETCWD)
if(NOT HAVE_GETCWD)
  check_symbol_exists(_getcwd direct.h HAVE__GETCWD)
endif()
check_symbol_exists(isinf math.h HAVE_ISINF)
check_symbol_exists(isnan math.h HAVE_ISNAN)
if(NOT HAVE_ISNAN)
  check_symbol_exists(_isnan float.h HAVE__ISNAN)
endif()
check_symbol_exists(finite math.h HAVE_FINITE)
if(NOT HAVE_FINITE)
  check_symbol_exists(_finite float.h HAVE__FINITE)
endif()
check_symbol_exists(ilogb math.h HAVE_ILOGB)
if(NOT HAVE_ILOGB)
  check_symbol_exists(_logb float.h HAVE__LOGB)
endif()
check_symbol_exists(vsnprintf "stdio.h;stdarg.h" HAVE_VSNPRINTF)
if(NOT HAVE_VSNPRINTF)
  check_symbol_exists(_vsnprintf "stdio.h;stdarg.h" HAVE__VSNPRINTF)
endif()
check_symbol_exists(__builtin_expect assert.h HAVE___BUILTIN_EXPECT)
set(HAVE_ASSERT_WITH_BUILTIN_EXPECT ${HAVE___BUILTIN_EXPECT})

check_symbol_exists(CGL_VERSION_1_0 OpenGL/OpenGL.h HAVE_CGL)
set(HAVE_CGL_PBUFFER ${HAVE_CGL})

if(NOT (HAVE_WGL OR HAVE_AGL OR HAVE_CGL))
  check_library_exists(GL glXChooseVisual "" HAVE_GLX)
endif()

check_symbol_exists(__func__ "" FUNC)
check_symbol_exists(__PRETTY_FUNCTION__ "" PRETTY_FUNCTION)
check_symbol_exists(__FUNCTION__ "" FUNCTION)
if(FUNC)
  set(HAVE_C_COMPILER_FUNCTION_NAME_VAR __func__)
  set(HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __func__)
elseif(PRETTY_FUNCTION)
  set(HAVE_C_COMPILER_FUNCTION_NAME_VAR __PRETTY_FUNCTION__)
  set(HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __PRETTY_FUNCTION__)
elseif(FUNCTION)
  set(HAVE_C_COMPILER_FUNCTION_NAME_VAR __FUNCTION__)
  set(HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __FUNCTION__)
endif()

if(NOT HAVE_UNISTD_H)
  add_definitions(-DYY_NO_UNISTD_H)
endif()

check_struct_has_member("struct timespec" tv_nsec pthread.h HAVE_PTHREAD_TIMESPEC_NSEC)

# Setting SIM_TIMEVAL_TV_SEC_T & SIM_TIMEVAL_TV_USEC_T for determining correct variable size
if(WIN32)
  set(CMAKE_EXTRA_INCLUDE_FILES winsock2.h)
else()
  set(CMAKE_EXTRA_INCLUDE_FILES "sys/time.h")
endif()

check_type_size("((struct timeval*)0)->tv_sec" TIMEVAL_TV_SEC)
if(HAVE_TIMEVAL_TV_SEC)
  #check_type_size_list("long;time_t" ${TIMEVAL_TV_SEC} SIM_TIMEVAL_TV_SEC_T)
  message(STATUS "TIMEVAL_TV_SEC: ${TIMEVAL_TV_SEC}")
  find_int_type_with_size(SIM_TIMEVAL_TV_SEC_T ${TIMEVAL_TV_SEC} "long" "time_t")
endif()

check_type_size("((struct timeval*)0)->tv_usec" TIMEVAL_TV_USEC)
if(HAVE_TIMEVAL_TV_USEC)
  #check_type_size_list("long;time_t" ${TIMEVAL_TV_USEC} SIM_TIMEVAL_TV_USEC_T)
  message(STATUS "TIMEVAL_TV_USEC: ${TIMEVAL_TV_USEC}")
  find_int_type_with_size(SIM_TIMEVAL_TV_USEC_T ${TIMEVAL_TV_USEC} "long" "time_t")
endif()
set(CMAKE_EXTRA_INCLUDE_FILES)

if(HAVE_X11_AVAILABLE)
  set(X_DISPLAY_MISSING 0)
else()
  set(X_DISPLAY_MISSING 1)
endif()

if(HAVE_WINDLL_RUNTIME_BINDING OR HAVE_DYLD_RUNTIME_BINDING OR HAVE_DLD_LIB OR HAVE_DL_LIB)
  set(HAVE_DYNAMIC_LINKING 1)
endif()

set(HASH_QUOTING_SRC_FILE "
  #include <string.h>
  #define TEST_QUOTE(str) #str
  int main(void) { return strcmp(\"sim\", TEST_QUOTE(sim)); }
")
check_c_source_runs("${HASH_QUOTING_SRC_FILE}" HAVE_HASH_QUOTING)

if(NOT HAVE_HASH_QUOTING)
  set(APOSTROPHES_QUOTING_SRC_FILE "
    #include <string.h>
    #define TEST_QUOTE(str) \"str\"
    int main(void) { return strcmp(\"sim\", TEST_QUOTE(sim)); }
  ")
  check_c_source_runs("${APOSTROPHES_QUOTING_SRC_FILE}" HAVE_APOSTROPHES_QUOTING)
endif()

if(HAVE_3DS_IMPORT_CAPABILITIES)
  set(FEAT_HAVE_3DS 1)
else()
  set(FEAT_HAVE_3DS 0)
endif()
if(HAVE_VRML97)
  set(FEAT_HAVE_VRML97 1)
else()
  set(FEAT_HAVE_VRML97 0)
endif()
if(HAVE_SOUND)
  set(FEAT_HAVE_SOUND 1)
else()
  set(FEAT_HAVE_SOUND 0)
endif()
if(HAVE_SUPERGLU)
  set(FEAT_HAVE_SUPERGLU 1)
else()
  set(FEAT_HAVE_SUPERGLU 0)
endif()
if(USE_PTHREAD OR USE_W32THREAD)
  set(FEAT_HAVE_THREADS 1)
else()
  set(FEAT_HAVE_THREADS 0)
endif()
if(COIN_THREADSAFE)
  set(FEAT_HAVE_SAFETHREAD 1)
else()
  set(FEAT_HAVE_SAFETHREAD 0)
endif()

# ############################################################################
# Setup targets in subdirectories
# ############################################################################

# COIN_DOCUMENTATION_FILES is filled with all source files.
unset(COIN_DOCUMENTATION_FILES CACHE)
# COIN_INTERNAL_DOCUMENTATION_FILES is filled with source files that are not part of the public API.
# If COIN_BUILD_INTERNAL_DOCUMENTATION is YES then these files are added to DOXYGEN_INPUT, otherwise
# they are put in DOXYGEN_EXCLUDE.
unset(COIN_INTERNAL_DOCUMENTATION_FILES CACHE)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(data)
#add_subdirectory(examples)

if(COIN_BUILD_TESTS)
  enable_testing()
  add_subdirectory(testsuite)
endif()

add_feature_info(ThreadSafe COIN_THREADSAFE "Thread safe render traversals.")
add_feature_info(VRML97 HAVE_VRML97 "VRML97 support.")
add_feature_info(JavaScript COIN_HAVE_JAVASCRIPT "JavaScript capabilities.")
add_feature_info(NodeKits HAVE_NODEKITS "NodeKit support.")
add_feature_info(Draggers HAVE_DRAGGERS "Dragger support.")
add_feature_info(Manipulators HAVE_MANIPULATORS "Manipulator support.")
add_feature_info(Sound HAVE_SOUND "Sound support.")
add_feature_info(3dsImport HAVE_3DS_IMPORT_CAPABILITIES "3ds import capabilities.")
add_feature_info(Expat USE_EXTERNAL_EXPAT "Use system install of expat.")
add_feature_info(Exceptions USE_EXCEPTIONS "Compile with exceptions (g++ only).")
add_feature_info(SuperGLU USE_SUPERGLU "Use superglu library.")
feature_summary(WHAT ALL
                INCLUDE_QUIET_PACKAGES
                DESCRIPTION "Enabled Features:"
                VAR enabledFeaturesText)
message(STATUS "${enabledFeaturesText}")

# ############################################################################
# New CPACK section, please see the README file inside cpack.d directory.
add_subdirectory(cpack.d)

# ############################################################################
# Legacy cmake statements, packagers should refer and populate the cpack.d 
# drop-in directory
# ############################################################################
# CPACK global settings
set(CPACK_COMPONENT_DEVELOPMENT_DEPENDS runtime)
# CPACK Debian
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_DOCUMENTATION_PACKAGE_NAME lib${PROJECT_NAME_LOWER}${COIN_SO_VERSION}-doc)
set(CPACK_DEBIAN_DOCUMENTATION_PACKAGE_SECTION "doc")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_DEPENDS lib${PROJECT_NAME_LOWER}${COIN_SO_VERSION})
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_NAME lib${PROJECT_NAME_LOWER}${COIN_SO_VERSION}-dev)
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_SECTION "libdevel")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://bitbucket.org/Coin3D/${PROJECT_NAME_LOWER}")
set(CPACK_DEBIAN_PACKAGE_NAME lib${PROJECT_NAME_LOWER}${COIN_SO_VERSION})
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_RUNTIME_PACKAGE_NAME lib${PROJECT_NAME_LOWER}${COIN_SO_VERSION}-dev)
set(CPACK_DEBIAN_RUNTIME_PACKAGE_SECTION "libs")
# CPACK Windows
set(CPACK_NSIS_PACKAGE_NAME "Coin ${PROJECT_VERSION}")
set(CPACK_NSIS_URL_INFO_ABOUT ${CPACK_DEBIAN_PACKAGE_HOMEPAGE})
# CPACK generic
set(CPACK_PACKAGE_CONTACT "coin-support@coin3d.org")
set(CPACK_PACKAGE_DISPLAY_NAME "Coin ${PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY Coin-${PROJECT_VERSION})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME}${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/COPYING)
# CPACK RPM
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_LICENSE "BSD")
set(CPACK_RPM_PACKAGE_NAME ${PROJECT_NAME}${PROJECT_VERSION_MAJOR})
set(CPACK_RPM_PACKAGE_URL ${CPACK_DEBIAN_PACKAGE_HOMEPAGE})
set(CPACK_RPM_DEVELOPMENT_PACKAGE_NAME ${PROJECT_NAME}${PROJECT_VERSION_MAJOR}-devel)
set(CPACK_RPM_DEVELOPMENT_PACKAGE_REQUIRES ${PROJECT_NAME}${PROJECT_VERSION_MAJOR})
set(CPACK_RPM_RUNTIME_PACKAGE_PROVIDES ${PROJECT_NAME}${PROJECT_VERSION_MAJOR})
# workaround for erroneous inclusion of system directories
# see https://stackoverflow.com/questions/24983181/cpack-generates-rpms-with-files-entries-that-conflict-with-the-rpm-spec-how-to
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/lib64/cmake")

if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
endif()

include(CPack)
