# source files
set(COIN_IO_FILES
	SoInput.cpp
	SoInput_FileInfo.cpp
	SoInput_Reader.cpp
	SoOutput.cpp
	SoOutput_Writer.cpp
	SoByteStream.cpp
	SoTranSender.cpp
	SoTranReceiver.cpp
	SoWriterefCounter.cpp
	gzmemio.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_IO_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} io/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(io OBJECT ${COIN_IO_FILES})
endif()

# define documentation
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/io/SoInput.cpp
	${CMAKE_SOURCE_DIR}/src/io/SoInputP.h
	${CMAKE_SOURCE_DIR}/src/io/SoOutput.cpp
	${CMAKE_SOURCE_DIR}/src/io/SoWriterefCounter.cpp
	${CMAKE_SOURCE_DIR}/src/io/SoWriterefCounter.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)
