# The following two-stage generation of config.h replaces configure_file(config.h.cmake.in config.h)
# and is necessary for the proper definition of COIN_SYSTEM_LIBRARY_NAME in config.h.
# Ref. https://public.kitware.com/pipermail/cmake/2015-April/060350.html
# Deal with @...@ configurable items:
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h_at_configured
    @ONLY
    )
# Deal with items configured with generator expressions:
  file(GENERATE
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    INPUT
    ${CMAKE_CURRENT_BINARY_DIR}/config.h_at_configured
    )
configure_file(setup.h.cmake.in setup.h)

add_definitions(-DCOIN_INTERNAL -DHAVE_CONFIG_H)

add_compile_options(-DCOIN_DEBUG=$<CONFIG:Debug>)

include_directories(
  ${CMAKE_BINARY_DIR}/include
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include/Inventor/annex
  ${CMAKE_BINARY_DIR}/data
)

# add include dirs for non targets
if(NOT "${COIN_TARGET_INCLUDE_DIRECTORIES}" STREQUAL "")
  include_directories("${COIN_TARGET_INCLUDE_DIRECTORIES}")
endif()
# add include dirs for targets
foreach(tgt ${COIN_TARGET_LINK_LIBRARIES})
  if(TARGET ${tgt} )
    include_directories($<TARGET_PROPERTY:${tgt},INTERFACE_INCLUDE_DIRECTORIES>)
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

foreach(SUBDIR ${SUBDIRS})
  add_subdirectory(${SUBDIR})
endforeach()
add_subdirectory(doc)

if(COIN_BUILD_SHARED_LIBS)
  set(COIN_LIBRARY_TYPE SHARED)
  if(WIN32)
    configure_file(${PROJECT_NAME}.rc.cmake.in ${PROJECT_NAME}.rc)
    set(COIN_SOURCE ${COIN_SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc)
  endif()
else()
  set(COIN_LIBRARY_TYPE STATIC)
endif()

if(COIN_BUILD_SINGLE_LIB)
  add_library(${PROJECT_NAME} ${COIN_LIBRARY_TYPE} tidbits.cpp ${COIN_SOURCE})
else()
  if(NOT HAVE_EXPAT)
    set(expat_target $<TARGET_OBJECTS:expat>)
  endif()

  add_library(${PROJECT_NAME} ${COIN_LIBRARY_TYPE} tidbits.cpp
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
    ${COIN_SOURCE}
  )
endif()

if(WIN32)
  if(COIN_BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} INTERFACE COIN_DLL)
  else()
    target_compile_definitions(${PROJECT_NAME} INTERFACE COIN_NOT_DLL)
  endif()
endif()

if(MSVC)
  if(NOT COIN_BUILD_SHARED_LIBS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7")                       # Override default /Zi to embed
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Z7")     # debugging info in the .lib.
  endif()
  # Suppress warning about empty switch statement in Bison-generated files:
  set_property(SOURCE scxml/eval-minimum-tab.cpp APPEND PROPERTY COMPILE_FLAGS /wd4065)
  set_property(SOURCE soscxml/eval-coin-tab.cpp APPEND PROPERTY COMPILE_FLAGS /wd4065)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "${COIN_TARGET_INCLUDE_DIRECTORIES}")
target_link_libraries(${PROJECT_NAME} PRIVATE "${COIN_TARGET_LINK_LIBRARIES}")
target_include_directories(${PROJECT_NAME} INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${COIN_SO_VERSION})

# ############################################################################
# Install binaries and config package
# ############################################################################

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

configure_package_config_file(${PROJECT_NAME_LOWER}-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  PATH_VARS COINDIR COIN_INCLUDE_DIR COIN_LIB_DIR COIN_DOC_DIR
)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

# To make the component usable not only from the install directory but also from the build directory
export(
  TARGETS ${PROJECT_NAME}
  FILE ${PROJECT_NAME_LOWER}-export.cmake
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config.cmake ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  COMPONENT development
)

install(EXPORT ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}::
  FILE ${PROJECT_NAME_LOWER}-export.cmake
  COMPONENT development
)

