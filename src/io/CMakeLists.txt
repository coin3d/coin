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
foreach(SRCFILE ${COIN_IO_FILES})
	set(COIN_DOCUMENTATION_FILES ${COIN_DOCUMENTATION_FILES} ${CMAKE_SOURCE_DIR}/src/io/${SRCFILE} CACHE INTERNAL "COIN_DOCUMENTATION_FILES")
endforeach(SRCFILE)
