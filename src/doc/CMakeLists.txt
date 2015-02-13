#add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(COIN_BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})
file(GLOB_RECURSE DOX *.dox*)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/docs/releases.dox
	${DOX}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

if(COIN_BUILD_DOCUMENTATION)
	if(NOT DOXYGEN_FOUND)
		message(FATAL_ERROR "Doxygen is needed to build the documentation.")
	endif()
	
	set(GENERATE_HTMLHELP NO)
	if (WIN32)
		set(DOXYGEN_GENERATE_MAN NO)
		find_program(HHC_PROGRAM NAMES hhc.exe PATHS "c:/Program Files/HTML Help Workshop" "c:/Program Files (x86)/HTML Help Workshop" DOC "HTML Help Compiler program")
		if(HHC_PROGRAM)
			option(COIN_BUILD_DOCUMENTATION_CHM "Create compressed HTML help (requires HHC)" ON)
			mark_as_advanced(COIN_BUILD_DOCUMENTATION_CHM)
		else()
			set(COIN_BUILD_DOCUMENTATION_CHM OFF)
		endif()
		if(COIN_BUILD_DOCUMENTATION_CHM)
			set(GENERATE_HTMLHELP YES)
		else()
			set(HHC_PROGRAM)
		endif()
	else()
		set(DOXYGEN_GENERATE_MAN YES)
		set(HHC_PROGRAM)
	endif()

	set(DOXYGEN_INPUT)
	foreach(XF ${COIN_DOCUMENTATION_FILES})
		set(DOXYGEN_INPUT "${DOXYGEN_INPUT}                         ${XF} \\\n")
	endforeach(XF)
	set(DOXYGEN_INPUT "${DOXYGEN_INPUT}                         ${CMAKE_SOURCE_DIR}/src/tidbits.cpp")
	set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	set(DOXYGEN_OUTPUT ${CMAKE_BINARY_DIR}/html/index.html)
	configure_file(${CMAKE_SOURCE_DIR}/docs/coin.doxygen.cmake.in ${DOXYFILE} @ONLY)
	
	add_custom_command(
		OUTPUT ${DOXYGEN_OUTPUT}
		COMMAND ${CMAKE_COMMAND} -E echo_append "Generating API documentation with Doxygen"
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
		COMMAND ${CMAKE_COMMAND} -E echo "Done."
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		DEPENDS ${DOXYFILE}
	)
	add_custom_target(documentation ALL DEPENDS ${DOXYGEN_OUTPUT})
	install(DIRECTORY ${CMAKE_BINARY_DIR}/html DESTINATION share/doc)
endif()
