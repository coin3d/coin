# The following two-stage generation of config.h replaces configure_file(config.h.cmake.in config.h)
# and is necessary for the proper definition of COIN_SYSTEM_LIBRARY_NAME in config.h.
# Ref. https://public.kitware.com/pipermail/cmake/2015-April/060350.html
# Deal with @...@ configurable items:
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h_at_configured"
    @ONLY
    )
# Deal with items configured with generator expressions:
  file(GENERATE
    OUTPUT
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
    INPUT
    "${CMAKE_CURRENT_BINARY_DIR}/config.h_at_configured"
    )
configure_file(setup.h.cmake.in setup.h)

add_compile_options(-DHAVE_CONFIG_H -DCOIN_INTERNAL -DCOIN_DEBUG=$<CONFIG:Debug>)

include_directories(BEFORE
  "${CMAKE_BINARY_DIR}/include"
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/include/Inventor/annex"
  "${CMAKE_BINARY_DIR}/data"
)

# add include dirs for non targets
if(NOT "${COIN_TARGET_INCLUDE_DIRECTORIES}" STREQUAL "")
  include_directories("${COIN_TARGET_INCLUDE_DIRECTORIES}")
endif()
# add include dirs for targets
foreach(_tgt ${COIN_TARGET_LINK_LIBRARIES})
  if(TARGET ${_tgt} )
    include_directories($<TARGET_PROPERTY:${_tgt},INTERFACE_INCLUDE_DIRECTORIES>)
  endif()
endforeach()

set(COIN_SOURCE tidbits.cpp CACHE INTERNAL "COIN_SOURCE")
set(SUBDIRS
  extensions xml actions base bundles caches details draggers elements
  engines errors events fields fonts glue io manips misc rendering lists nodekits
  navigation nodes shapenodes projectors sensors upgraders
  3ds collision hardcopy shadows geo threads shaders profiler
  vrml97 foreignfiles scxml soscxml
)

foreach(_subdir ${SUBDIRS})
  add_subdirectory(${_subdir})
endforeach()
add_subdirectory(doc)

# Adding source groups for IDE's (Visual Studio, XCode, Qt Creator, etc.)
source_group("Header Files\\Inventor"                            REGULAR_EXPRESSION ".*/include/Inventor/.*[.]h")
source_group("Header Files\\Inventor\\actions"                   REGULAR_EXPRESSION ".*/include/Inventor/actions/.*[.]h")
source_group("Header Files\\Inventor\\annex"                     REGULAR_EXPRESSION ".*/include/Inventor/annex/.*[.]h")
source_group("Header Files\\Inventor\\annex\\ForeignFiles"       REGULAR_EXPRESSION ".*/include/Inventor/annex/ForeignFiles/.*[.]h")
source_group("Header Files\\Inventor\\annex\\FXViz"              REGULAR_EXPRESSION ".*/include/Inventor/annex/FXViz/.*[.]h")
source_group("Header Files\\Inventor\\annex\\FXViz\\elements"    REGULAR_EXPRESSION ".*/include/Inventor/annex/FXViz/elements/.*[.]h")
source_group("Header Files\\Inventor\\annex\\FXViz\\nodes"       REGULAR_EXPRESSION ".*/include/Inventor/annex/FXViz/nodes/.*[.]h")
source_group("Header Files\\Inventor\\annex\\Hardcopy"           REGULAR_EXPRESSION ".*/include/Inventor/annex/Hardcopy/.*[.]h")
source_group("Header Files\\Inventor\\annex\\Profiler"           REGULAR_EXPRESSION ".*/include/Inventor/annex/Profiler/.*[.]h")
source_group("Header Files\\Inventor\\annex\\Profiler\\elements" REGULAR_EXPRESSION ".*/include/Inventor/annex/Profiler/elements/.*[.]h")
source_group("Header Files\\Inventor\\annex\\Profiler\\engines"  REGULAR_EXPRESSION ".*/include/Inventor/annex/Profiler/engines/.*[.]h")
source_group("Header Files\\Inventor\\annex\\Profiler\\nodekits" REGULAR_EXPRESSION ".*/include/Inventor/annex/Profiler/nodekits/.*[.]h")
source_group("Header Files\\Inventor\\annex\\Profiler\\nodes"    REGULAR_EXPRESSION ".*/include/Inventor/annex/Profiler/nodes/.*[.]h")
source_group("Header Files\\Inventor\\annex\\Profiler\\utils"    REGULAR_EXPRESSION ".*/include/Inventor/annex/Profiler/utils/.*[.]h")
source_group("Header Files\\Inventor\\bundles"                   REGULAR_EXPRESSION ".*/include/Inventor/bundles/.*[.]h")
source_group("Header Files\\Inventor\\C"                         REGULAR_EXPRESSION ".*/include/Inventor/C/.*[.]h")
source_group("Header Files\\Inventor\\C\\base"                   REGULAR_EXPRESSION ".*/include/Inventor/C/base/.*[.]h")
source_group("Header Files\\Inventor\\C\\errors"                 REGULAR_EXPRESSION ".*/include/Inventor/C/errors/.*[.]h")
source_group("Header Files\\Inventor\\C\\glue"                   REGULAR_EXPRESSION ".*/include/Inventor/C/glue/.*[.]h")
source_group("Header Files\\Inventor\\C\\threads"                REGULAR_EXPRESSION ".*/include/Inventor/C/threads/.*[.]h")
source_group("Header Files\\Inventor\\C\\XML"                    REGULAR_EXPRESSION ".*/include/Inventor/C/XML/.*[.]h")
source_group("Header Files\\Inventor\\caches"                    REGULAR_EXPRESSION ".*/include/Inventor/caches/.*[.]h")
source_group("Header Files\\Inventor\\collision"                 REGULAR_EXPRESSION ".*/include/Inventor/collision/.*[.]h")
source_group("Header Files\\Inventor\\details"                   REGULAR_EXPRESSION ".*/include/Inventor/details/.*[.]h")
source_group("Header Files\\Inventor\\draggers"                  REGULAR_EXPRESSION ".*/include/Inventor/draggers/.*[.]h")
source_group("Header Files\\Inventor\\elements"                  REGULAR_EXPRESSION ".*/include/Inventor/elements/.*[.]h")
source_group("Header Files\\Inventor\\engines"                   REGULAR_EXPRESSION ".*/include/Inventor/engines/.*[.]h")
source_group("Header Files\\Inventor\\errors"                    REGULAR_EXPRESSION ".*/include/Inventor/errors/.*[.]h")
source_group("Header Files\\Inventor\\events"                    REGULAR_EXPRESSION ".*/include/Inventor/events/.*[.]h")
source_group("Header Files\\Inventor\\fields"                    REGULAR_EXPRESSION ".*/include/Inventor/fields/.*[.]h")
source_group("Header Files\\Inventor\\lists"                     REGULAR_EXPRESSION ".*/include/Inventor/lists/.*[.]h")
source_group("Header Files\\Inventor\\lock"                      REGULAR_EXPRESSION ".*/include/Inventor/lock/.*[.]h")
source_group("Header Files\\Inventor\\manips"                    REGULAR_EXPRESSION ".*/include/Inventor/manips/.*[.]h")
source_group("Header Files\\Inventor\\misc"                      REGULAR_EXPRESSION ".*/include/Inventor/misc/.*[.]h")
source_group("Header Files\\Inventor\\navigation"                REGULAR_EXPRESSION ".*/include/Inventor/navigation/.*[.]h")
source_group("Header Files\\Inventor\\nodekits"                  REGULAR_EXPRESSION ".*/include/Inventor/nodekits/.*[.]h")
source_group("Header Files\\Inventor\\nodes"                     REGULAR_EXPRESSION ".*/include/Inventor/nodes/.*[.]h")
source_group("Header Files\\Inventor\\projectors"                REGULAR_EXPRESSION ".*/include/Inventor/projectors/.*[.]h")
source_group("Header Files\\Inventor\\scxml"                     REGULAR_EXPRESSION ".*/include/Inventor/scxml/.*[.]h")
source_group("Header Files\\Inventor\\sensors"                   REGULAR_EXPRESSION ".*/include/Inventor/sensors/.*[.]h")
source_group("Header Files\\Inventor\\system"                    REGULAR_EXPRESSION ".*/include/Inventor/system/.*[.]h")
source_group("Header Files\\Inventor\\threads"                   REGULAR_EXPRESSION ".*/include/Inventor/threads/.*[.]h")
source_group("Header Files\\Inventor\\VRMLnodes"                 REGULAR_EXPRESSION ".*/include/Inventor/VRMLnodes/.*[.]h")
source_group("Header Files\\Inventor\\XML"                       REGULAR_EXPRESSION ".*/include/Inventor/XML/.*[.]h")

source_group("Source Files\\3ds"                       REGULAR_EXPRESSION ".*/src/3ds/.*[.](h|c|cpp)")
source_group("Source Files\\actions"                   REGULAR_EXPRESSION ".*/src/actions/.*[.](h|c|cpp)")
source_group("Source Files\\base"                      REGULAR_EXPRESSION ".*/src/base/.*[.](h|c|cpp)")
source_group("Source Files\\bundles"                   REGULAR_EXPRESSION ".*/src/bundles/.*[.](h|c|cpp)")
source_group("Source Files\\caches"                    REGULAR_EXPRESSION ".*/src/caches/.*[.](h|c|cpp)")
source_group("Source Files\\collision"                 REGULAR_EXPRESSION ".*/src/collision/.*[.](h|c|cpp)")
source_group("Source Files\\details"                   REGULAR_EXPRESSION ".*/src/details/.*[.](h|c|cpp)")
source_group("Source Files\\draggers"                  REGULAR_EXPRESSION ".*/src/draggers/.*[.](h|c|cpp)")
source_group("Source Files\\elements"                  REGULAR_EXPRESSION ".*/src/elements/.*[.](h|c|cpp)")
source_group("Source Files\\elements\\GL"              REGULAR_EXPRESSION ".*/src/elements/GL/.*[.](h|c|cpp)")
source_group("Source Files\\engines"                   REGULAR_EXPRESSION ".*/src/engines/.*[.](h|c|cpp)")
source_group("Source Files\\errors"                    REGULAR_EXPRESSION ".*/src/errors/.*[.](h|c|cpp)")
source_group("Source Files\\events"                    REGULAR_EXPRESSION ".*/src/events/.*[.](h|c|cpp)")
source_group("Source Files\\fields"                    REGULAR_EXPRESSION ".*/src/fields/.*[.](h|c|cpp)")
source_group("Source Files\\fonts"                     REGULAR_EXPRESSION ".*/src/fonts/.*[.](h|c|cpp)")
source_group("Source Files\\foreignfiles"              REGULAR_EXPRESSION ".*/src/foreignfiles/.*[.](h|c|cpp)")
source_group("Source Files\\geo"                       REGULAR_EXPRESSION ".*/src/geo/.*[.](h|c|cpp)")
source_group("Source Files\\glue"                      REGULAR_EXPRESSION ".*/src/glue/.*[.](h|c|cpp)")
source_group("Source Files\\hardcopy"                  REGULAR_EXPRESSION ".*/src/hardcopy/.*[.](h|c|cpp)")
source_group("Source Files\\io"                        REGULAR_EXPRESSION ".*/src/io/.*[.](h|c|cpp)")
source_group("Source Files\\lists"                     REGULAR_EXPRESSION ".*/src/lists/.*[.](h|c|cpp)")
source_group("Source Files\\manips"                    REGULAR_EXPRESSION ".*/src/manips/.*[.](h|c|cpp)")
source_group("Source Files\\misc"                      REGULAR_EXPRESSION ".*/src/misc/.*[.](h|c|cpp)")
source_group("Source Files\\navigation"                REGULAR_EXPRESSION ".*/src/navigation/.*[.](h|c|cpp)")
source_group("Source Files\\nodekits"                  REGULAR_EXPRESSION ".*/src/nodekits/.*[.](h|c|cpp)")
source_group("Source Files\\nodes"                     REGULAR_EXPRESSION ".*/src/nodes/.*[.](h|c|cpp)")
source_group("Source Files\\profiler"                  REGULAR_EXPRESSION ".*/src/profiler/.*[.](h|c|cpp)")
source_group("Source Files\\profiler\\tools"           REGULAR_EXPRESSION ".*/src/profiler/tools/.*[.](h|c|cpp)")
source_group("Source Files\\projectors"                REGULAR_EXPRESSION ".*/src/projectors/.*[.](h|c|cpp)")
source_group("Source Files\\rendering"                 REGULAR_EXPRESSION ".*/src/rendering/.*[.](h|c|cpp)")
source_group("Source Files\\scxml"                     REGULAR_EXPRESSION ".*/src/scxml/.*[.](h|c|cpp)")
source_group("Source Files\\sensors"                   REGULAR_EXPRESSION ".*/src/sensors/.*[.](h|c|cpp)")
source_group("Source Files\\shaders"                   REGULAR_EXPRESSION ".*/src/shaders/.*[.](h|c|cpp)")
source_group("Source Files\\shadows"                   REGULAR_EXPRESSION ".*/src/shadows/.*[.](h|c|cpp)")
source_group("Source Files\\shapenodes"                REGULAR_EXPRESSION ".*/src/shapenodes/.*[.](h|c|cpp)")
source_group("Source Files\\soscxml"                   REGULAR_EXPRESSION ".*/src/soscxml/.*[.](h|c|cpp)")
source_group("Source Files\\system"                    REGULAR_EXPRESSION ".*/src/system/.*[.](h|c|cpp)")
source_group("Source Files\\threads"                   REGULAR_EXPRESSION ".*/src/threads/.*[.](h|c|cpp)")
source_group("Source Files\\upgraders"                 REGULAR_EXPRESSION ".*/src/upgraders/.*[.](h|c|cpp)")
source_group("Source Files\\vrml97"                    REGULAR_EXPRESSION ".*/src/vrml97/.*[.](h|c|cpp)")
source_group("Source Files\\xml"                       REGULAR_EXPRESSION ".*/src/xml/.*[.](h|c|cpp)")
source_group("Source Files\\xml\\expat"                REGULAR_EXPRESSION ".*/src/xml/expat/.*[.](h|c|cpp)")

if(COIN_BUILD_SHARED_LIBS)
  set(COIN_LIBRARY_TYPE SHARED)
else()
  set(COIN_LIBRARY_TYPE STATIC)
endif()

set(ADDITIONAL_HDRS ${CMAKE_SOURCE_DIR}/include/SoWinEnterScope.h ${CMAKE_SOURCE_DIR}/include/SoWinLeaveScope.h ${CMAKE_SOURCE_DIR}/include/SoDebug.h ${CMAKE_BINARY_DIR}/src/config.h)

# Create the library target
if(COIN_BUILD_SINGLE_LIB)
  add_library(${PROJECT_NAME} ${COIN_LIBRARY_TYPE} ${COIN_SOURCE} ${COMMON_HDRS} ${ADDITIONAL_HDRS})
else()
  if(NOT HAVE_EXPAT)
    set(expat_target $<TARGET_OBJECTS:expat>)
  endif()

  add_library(${PROJECT_NAME} ${COIN_LIBRARY_TYPE} ${COIN_SOURCE} ${COMMON_HDRS} ${ADDITIONAL_HDRS}
    $<TARGET_OBJECTS:3ds>
    $<TARGET_OBJECTS:actions>
    $<TARGET_OBJECTS:base>
    $<TARGET_OBJECTS:bundles>
    $<TARGET_OBJECTS:caches>
    $<TARGET_OBJECTS:collision>
    $<TARGET_OBJECTS:details>
    $<TARGET_OBJECTS:draggers>
    $<TARGET_OBJECTS:elements>
    $<TARGET_OBJECTS:engines>
    $<TARGET_OBJECTS:errors>
    $<TARGET_OBJECTS:events>
    ${expat_target}
    $<TARGET_OBJECTS:fields>
    $<TARGET_OBJECTS:fonts>
    $<TARGET_OBJECTS:foreignfiles>
    $<TARGET_OBJECTS:geo>
    $<TARGET_OBJECTS:glelements>
    $<TARGET_OBJECTS:glue>
    $<TARGET_OBJECTS:hardcopy>
    $<TARGET_OBJECTS:io>
    $<TARGET_OBJECTS:lists>
    $<TARGET_OBJECTS:manips>
    $<TARGET_OBJECTS:misc>
    $<TARGET_OBJECTS:navigation>
    $<TARGET_OBJECTS:nodekits>
    $<TARGET_OBJECTS:nodes>
    $<TARGET_OBJECTS:profiler>
    $<TARGET_OBJECTS:projectors>
    $<TARGET_OBJECTS:rendering>
    $<TARGET_OBJECTS:scxml>
    $<TARGET_OBJECTS:sensors>
    $<TARGET_OBJECTS:shaders>
    $<TARGET_OBJECTS:shadows>
    $<TARGET_OBJECTS:shapenodes>
    $<TARGET_OBJECTS:soscxml>
    $<TARGET_OBJECTS:threads>
    $<TARGET_OBJECTS:upgraders>
    $<TARGET_OBJECTS:vrml97>
    $<TARGET_OBJECTS:xml>
  )
endif()

if(WIN32)
  if(COIN_BUILD_SHARED_LIBS)
    configure_file(${PROJECT_NAME}.rc.cmake.in ${PROJECT_NAME}.rc)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc")
    target_compile_definitions(${PROJECT_NAME} INTERFACE COIN_DLL)
    if(MINGW)
      set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_NAME ${PROJECT_NAME}-${COIN_SO_VERSION})
    endif()
  else()
    target_compile_definitions(${PROJECT_NAME} INTERFACE COIN_NOT_DLL)
    if(MSVC)
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7")                       # Override default /Zi to embed
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Z7")     # debugging info in the .lib.
    endif()
  endif()
  if(MSVC)
    # Suppress warning about empty switch statement in Bison-generated files:
    set_property(SOURCE scxml/eval-minimum-tab.cpp APPEND PROPERTY COMPILE_FLAGS /wd4065)
    set_property(SOURCE soscxml/eval-coin-tab.cpp APPEND PROPERTY COMPILE_FLAGS /wd4065)
  endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "${COIN_TARGET_INCLUDE_DIRECTORIES}")
target_link_libraries(${PROJECT_NAME} PRIVATE "${COIN_TARGET_LINK_LIBRARIES}")
target_include_directories(${PROJECT_NAME} INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${COIN_SO_VERSION})

# ############################################################################
# Install headers
# ############################################################################

  install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/Inventor/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor COMPONENT development FILES_MATCHING PATTERN "*.h")
  install(DIRECTORY ${CMAKE_BINARY_DIR}/include/Inventor/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor COMPONENT development FILES_MATCHING PATTERN "*.h")
  install(FILES ${CMAKE_SOURCE_DIR}/include/SoWinEnterScope.h ${CMAKE_SOURCE_DIR}/include/SoWinLeaveScope.h ${CMAKE_SOURCE_DIR}/include/SoDebug.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor COMPONENT development)

# ############################################################################
# Install data files
# ############################################################################

  install(DIRECTORY ${CMAKE_SOURCE_DIR}/data/ DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME} COMPONENT development FILES_MATCHING PATTERN [cep]*.xml PATTERN *.iv PATTERN *.glsl)

# ############################################################################
# Install binaries and config package
# ############################################################################

if(UNIX OR MINGW)
  configure_file(${CMAKE_SOURCE_DIR}/Coin.pc.cmake.in ${CMAKE_BINARY_DIR}/Coin.pc @ONLY)
  install(FILES ${CMAKE_BINARY_DIR}/Coin.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_SKIP
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
)

if(COIN_BUILD_SHARED_LIBS)
  install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development NAMELINK_ONLY)
  if(MSVC)
    install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
  endif()
endif()

# Support find_package(Coin NO_MODULE).
set(COIN_DOC_DIR ${CMAKE_INSTALL_DOCDIR})
set(COIN_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set(COIN_LIB_DIR ${CMAKE_INSTALL_LIBDIR})
set(COINDIR ${CMAKE_INSTALL_PREFIX})
include(CMakePackageConfigHelpers)

configure_package_config_file(${PROJECT_NAME_LOWER}-config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  PATH_VARS COINDIR COIN_INCLUDE_DIR COIN_LIB_DIR COIN_DOC_DIR
)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

# To make the component usable not only from the install directory but also from the build directory
export(
  TARGETS ${PROJECT_NAME}
  FILE ${PROJECT_NAME_LOWER}-export.cmake
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  COMPONENT development
)

install(EXPORT ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}::
  FILE ${PROJECT_NAME_LOWER}-export.cmake
  COMPONENT development
)

