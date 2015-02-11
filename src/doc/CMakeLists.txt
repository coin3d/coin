#add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
	if(NOT DOXYGEN_FOUND)
		message(FATAL_ERROR "Doxygen is needed to build the documentation.")
	endif()
	
	set(doxyfile_in ${CMAKE_SOURCE_DIR}/docs/coin.doxygen.cmake.in)
	set(doxyfile_out ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	
	include(${CMAKE_SOURCE_DIR}/docs/coin_doxygenfiles.cmake.inc)
	
	configure_file(${doxyfile_in} ${doxyfile_out} @ONLY)
	
#	add_custom_target(doc
#		COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
#		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#		COMMENT "Generating API documentation with Doxygen"
#		VERBATIM)
		
#	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endif()
