
macro(create_testsuite input)
	get_filename_component(FLNAME "${input}" NAME_WE)
	get_filename_component(FLPATH "${input}" PATH)
    string(REGEX REPLACE ".*[/\\]" "" FLSUBFLD "${FLPATH}")
    set(COIN_STR_TEST_CLASS "${FLNAME}")
	file(READ ${CMAKE_SOURCE_DIR}/${input} f0)
    if(f0 MATCHES "#ifdef[ \t]+COIN_TEST_SUITE")
	    # message(STATUS "Parse: ${CMAKE_SOURCE_DIR}/${input} - ${FLPATHSUB}${FLNAME}Test.cpp")
        # get first include from file, which we assume is include to tested class
  	    string(REGEX MATCH "[\n\r]+#include[ \t]<[^\n]+" iclass "${f0}")
        # get block between '#ifdef COIN_TEST_SUITE' and '#endif'
	    string(REGEX REPLACE ".*#ifdef[ \t]+COIN_TEST_SUITE" "" f1 "${f0}")
	    string(REGEX REPLACE "#endif[ \t/!]+COIN_TEST_SUITE.*" "" f2 "${f1}")
        # get all #include statements within COIN_TEST_SUITE code block
  	    string(REGEX MATCHALL "#include[ \t]<[^\n]+" i0 "${f2}")
	    string(REPLACE ";" "\n" COIN_STR_TEST_INCL "${i0}")
        set(COIN_STR_TEST_INCL "${iclass}\n${COIN_STR_TEST_INCL}")
        # remove #inlcude statements from test code string (moved to ${COIN_STR_TEST_INCL})
	    string(REGEX REPLACE "[\n\r ]*#include[ \t]<[^\n]+" "" COIN_STR_TEST_CODE "${f2}")
        # generate new test code file with extacted snippets
	    configure_file(TestSuiteTemplate.cmake.in "${FLSUBFLD}${FLNAME}Test.cpp")
    endif()
endmacro()

macro(create_file_test input testfunc)
    file(GLOB_RECURSE COIN_IV_FILES RELATIVE ${CMAKE_SOURCE_DIR} ../${input}/*.wrl ../${input}/*.wrml ../${input}/*.gz ../${input}/*.iv)
    set(COIN_STR_TEST_INCL "")
    set(COIN_STR_TEST_CODE "")
    set(COIN_STR_TEST_CLASS "${input}")
    foreach(INPUTFILE ${COIN_IV_FILES})
        string(REGEX REPLACE "[\\./\\-]+" "_" TESTNAME "${INPUTFILE}")
        set(COIN_STR_TEST_CODE "${COIN_STR_TEST_CODE}\nBOOST_AUTO_TEST_CASE(${TESTNAME}) { test_file(\"${CMAKE_SOURCE_DIR}/${INPUTFILE}\", &${testfunc}); }")
    endforeach(INPUTFILE)
    configure_file(TestSuiteTemplate.cmake.in "${input}Test.cpp")
endmacro()


# Parse all source files for embedded '#ifdef COIN_TEST_SUITE' and extract those blocks
# to spearate test source files.
file(GLOB_RECURSE COIN_SRC_FILES RELATIVE ${CMAKE_SOURCE_DIR} ../src/*.cpp)
foreach(INPUTFILE ${COIN_SRC_FILES})
    create_testsuite(${INPUTFILE})
endforeach(INPUTFILE)

# Create test cases for all scenes under models folder (this replaces StandardTests.cpp).
# The original StandardTests.cpp also tested additional folders:
# - "killers" with testInCorrectFile
# - "slackers" with testOutOfSpecFile
# However, those folders are not in the repository.
create_file_test("models" "testCorrectFile")

# Include all extracted '*Tests.cpp' files in the target.
FILE(GLOB TEST_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/*Test.cpp")

include_directories(.)
include_directories(../include)
include_directories(../include/Inventor/annex)
include_directories(${CMAKE_BINARY_DIR}/include)
add_executable(CoinTests TestSuiteMain.cpp TestSuiteUtils.cpp TestSuiteMisc.cpp ${TEST_SOURCES})
target_link_libraries(CoinTests Coin)
