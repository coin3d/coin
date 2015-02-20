# source files
set(COIN_PROFILER_FILES
	SoProfiler.cpp
	SoProfilerElement.cpp
	SoProfilerOverlayKit.cpp
	SoProfilerStats.cpp
	SoProfilingReportGenerator.cpp
	SoProfilerTopEngine.cpp
	SoScrollingGraphKit.cpp
	SoNodeVisualize.cpp
	SoProfilerTopKit.cpp
	SoProfilerVisualizeKit.cpp
	SbProfilingData.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_PROFILER_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} profiler/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(profiler OBJECT ${COIN_PROFILER_FILES})
endif()

# define documentation
foreach(SRCFILE ${COIN_PROFILER_FILES})
	set(COIN_DOCUMENTATION_FILES ${COIN_DOCUMENTATION_FILES} ${CMAKE_SOURCE_DIR}/src/profiler/${SRCFILE} CACHE INTERNAL "COIN_DOCUMENTATION_FILES")
endforeach(SRCFILE)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/profiler/SoProfilerP.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)
