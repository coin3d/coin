configure_file(config.h.cmake.in config.h)
configure_file(setup.h.cmake.in setup.h)

add_definitions(-DCOIN_INTERNAL -DCOIN_DEBUG=1 -DHAVE_CONFIG_H)

include_directories(../include/Inventor/annex . ${CMAKE_CURRENT_BINARY_DIR} ../build/msvc10)

set(SUBDIRS
        extensions xml actions base bundles caches details draggers elements
        engines errors events fields fonts glue io manips misc rendering lists nodekits
        navigation nodes shapenodes projectors sensors upgraders
        3ds collision hardcopy shadows geo threads shaders profiler
        vrml97 foreignfiles scxml soscxml doc
)

include_directories(../include ..)

foreach(SUBDIR ${SUBDIRS})
    add_subdirectory(${SUBDIR})
endforeach(SUBDIR)

add_library(Coin
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
        $<TARGET_OBJECTS:expat>
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
        tidbits.cpp
)

find_package(OpenGL)
target_link_libraries(Coin ${OPENGL_LIBRARIES})

install(TARGETS Coin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
