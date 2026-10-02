if(COIN_BUILD_LEGACY_GL_RENDERER)
  find_package(Threads REQUIRED)
  add_executable(CoinBumpProgramTest bumprender/FailureTest.cpp)
  target_compile_definitions(CoinBumpProgramTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
  target_include_directories(CoinBumpProgramTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src
    ${PROJECT_SOURCE_DIR}/include/Inventor/annex ${COIN_TARGET_INCLUDE_DIRECTORIES})
  target_link_libraries(CoinBumpProgramTest Coin Threads::Threads ${COIN_TARGET_LINK_LIBRARIES})
  add_test(NAME BumpProgramFailuresAndLifetime COMMAND CoinBumpProgramTest)

  if(UNIX AND NOT APPLE)
    find_package(X11 QUIET)
    if(X11_FOUND AND HAVE_GLX)
      add_executable(CoinBumpGLXTest bumprender/GLXTest.cpp)
      target_compile_definitions(CoinBumpGLXTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
      target_include_directories(CoinBumpGLXTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
        ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src
        ${PROJECT_SOURCE_DIR}/include/Inventor/annex
        ${COIN_TARGET_INCLUDE_DIRECTORIES} ${X11_INCLUDE_DIR})
      target_link_libraries(CoinBumpGLXTest Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
      add_test(NAME BumpProgramGLX COMMAND CoinBumpGLXTest)
      set_tests_properties(BumpProgramGLX PROPERTIES SKIP_RETURN_CODE 77)
    endif()
  endif()
endif()

macro(create_testsuite input)
	get_filename_component(FLNAME "${input}" NAME_WE)
	get_filename_component(FLPATH "${input}" PATH)
	string(REGEX REPLACE ".*[/\\]" "" FLSUBFLD "${FLPATH}")
	set(COIN_STR_TEST_CLASS "${FLNAME}")
	file(READ ${PROJECT_SOURCE_DIR}/${input} f0)
	if(f0 MATCHES "#ifdef[ \t]+COIN_TEST_SUITE")
		# message(STATUS "Parse: ${PROJECT_SOURCE_DIR}/${input} - ${FLPATHSUB}${FLNAME}Test.cpp")
		# get first include from file, which we assume is include to tested class
		string(REGEX MATCH "[\n\r]+#include[ \t]<[^\n]+" iclass "${f0}")
		# get block between '#ifdef COIN_TEST_SUITE' and '#endif'
		string(REGEX REPLACE ".*#ifdef[ \t]+COIN_TEST_SUITE" "" f1 "${f0}")
		string(REGEX REPLACE "#endif[ \t/!]+COIN_TEST_SUITE.*" "" f2 "${f1}")
		# get all #include statements within COIN_TEST_SUITE code block
		string(REGEX MATCHALL "#include[ \t]<[^\n]+" i0 "${f2}")
		string(REPLACE ";" "\n" COIN_STR_TEST_INCL "${i0}")
		set(COIN_STR_TEST_INCL "${iclass}\n${COIN_STR_TEST_INCL}")
		# remove #include statements from test code string (moved to ${COIN_STR_TEST_INCL})
		string(REGEX REPLACE "[\n\r ]*#include[ \t]<[^\n]+" "" COIN_STR_TEST_CODE "${f2}")
		# generate new test code file with extracted snippets
		configure_file(TestSuiteTemplate.cmake.in "${FLSUBFLD}${FLNAME}Test.cpp")
		if("${FLSUBFLD}${FLNAME}" STREQUAL "basedict")
			set_source_files_properties("${FLSUBFLD}${FLNAME}Test.cpp" PROPERTIES
				COMPILE_DEFINITIONS COIN_INTERNAL)
		endif()
	endif()
endmacro()

macro(create_file_test input testfunc filter)
	file(GLOB_RECURSE COIN_IV_FILES RELATIVE ${PROJECT_SOURCE_DIR}
		../${input}/*.iv
		../${input}/*.iv.gz
	)
	if (HAVE_VRML97)
		file(GLOB_RECURSE COIN_WRL_VRML_FILES RELATIVE ${PROJECT_SOURCE_DIR}
			../${input}/*.wrl
			../${input}/*.vrml
			../${input}/*.wrl.gz
			../${input}/*.vrml.gz
		)
		list(APPEND COIN_IV_FILES ${COIN_WRL_VRML_FILES})
	endif()
	string(REGEX REPLACE "[\\./\\-]+" "" TESTSUITENAME "${input}")
	set(COIN_STR_TEST_INCL "")
	set(COIN_STR_TEST_CODE "const char *readError[] = { \"Coin read error:\", 0 };\n")
	set(COIN_STR_TEST_CLASS "${TESTSUITENAME}")
	set(extra_macro_args ${ARGN})
	list(LENGTH extra_macro_args num_extra_args)
	foreach(INPUTFILE ${COIN_IV_FILES})
		if(${num_extra_args} EQUAL 0 OR NOT ("${INPUTFILE}" MATCHES "${extra_macro_args}"))
			string(REGEX REPLACE "[\\./\\-]+" "_" TESTNAME "${INPUTFILE}")
			if (${filter})
				set(COIN_STR_TEST_CODE "${COIN_STR_TEST_CODE}\nBOOST_AUTO_TEST_CASE(${TESTNAME}) { PushMessageSuppressFilters(readError); ResetReadErrorCount(); test_file(\"${PROJECT_SOURCE_DIR}/${INPUTFILE}\", &${testfunc}); PopMessageSuppressFilters(); }")
			else()
				set(COIN_STR_TEST_CODE "${COIN_STR_TEST_CODE}\nBOOST_AUTO_TEST_CASE(${TESTNAME}) { ResetReadErrorCount(); test_file(\"${PROJECT_SOURCE_DIR}/${INPUTFILE}\", &${testfunc}); }")
			endif()
		endif()
	endforeach()
	configure_file(TestSuiteTemplate.cmake.in "${TESTSUITENAME}Test.cpp")
endmacro()

# Parse all source files for embedded '#ifdef COIN_TEST_SUITE' and extract those blocks
# to separate test source files.
file(GLOB_RECURSE COIN_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src/*.cpp)
foreach(INPUTFILE ${COIN_SRC_FILES})
	create_testsuite(${INPUTFILE})
endforeach()

# Create test cases for all scenes under models folder (this replaces StandardTests.cpp).
# The original StandardTests.cpp also tested additional folders:
# - "killers" with testInCorrectFile
# - "slackers" with testOutOfSpecFile
# However, those folders are not in the repository.
create_file_test("models" "testCorrectFile" false "incorrect")
# bug039.iv and bug055-5.iv contain errors but load anyway: ignore for now
create_file_test("models/io/incorrect" "testIncorrectFile" true "(bug039|bug055-5)")

# Include all extracted '*Tests.cpp' files in the target.
FILE(GLOB COIN_TEST_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/*Test.cpp")

add_executable(CoinTests
	TestSuiteMain.cpp
	TestSuiteUtils.cpp
	TestSuiteMisc.cpp
	InternalMacroTest.cpp
	${COIN_TEST_SOURCES}
)

# Private Coin symbols are not exported from the Windows DLL. Compile the
# dictionary implementation and the existing prime-number support directly
# into CoinTests. Static builds resolve these symbols from the Coin archive.
if(WIN32 AND COIN_BUILD_SHARED_LIBS)
	target_sources(CoinTests PRIVATE
		${PROJECT_SOURCE_DIR}/src/base/dict.cpp
		CoinGeqPrimeNumberWin32TestSupport.cpp)
	set_source_files_properties(
		${PROJECT_SOURCE_DIR}/src/base/dict.cpp
		CoinGeqPrimeNumberWin32TestSupport.cpp
		PROPERTIES
		COMPILE_DEFINITIONS COIN_INTERNAL)
endif()

set_source_files_properties(InternalMacroTest.cpp PROPERTIES
	COMPILE_DEFINITIONS COIN_INTERNAL)
set_target_properties(CoinTests PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
target_link_libraries(CoinTests Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/Inventor/annex
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_BINARY_DIR}/include
	${COIN_TARGET_INCLUDE_DIRECTORIES}
)
if(NOT WIN32)
	target_sources(CoinTests PRIVATE GLUWrapperTest.cpp)
	target_include_directories(CoinTests PRIVATE ${PROJECT_BINARY_DIR}/src)
endif()
if (USE_PTHREAD)
	target_link_libraries(CoinTests pthread)
endif()
add_test(NAME CoinTests COMMAND CoinTests)

add_subdirectory(xml-c-api/c-headers)

add_executable(SoPathRangeTest SoPathRangeTest.cpp)
target_link_libraries(SoPathRangeTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoPathRangeTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SoPathRange COMMAND SoPathRangeTest)
# Process-isolated public behavior checks. Keeping each invalid access in its
# own CTest process prevents one unsafe access from masking the remaining
# boundary classes.
add_executable(SoPathLegacyAccessorBoundsTest
  reproducers/sopath-legacy-accessor-bounds/repro.cpp)
target_link_libraries(SoPathLegacyAccessorBoundsTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoPathLegacyAccessorBoundsTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_compile_definitions(SoPathLegacyAccessorBoundsTest PRIVATE
  COIN_DEBUG=$<CONFIG:Debug>)

set(SOPATH_LEGACY_ACCESSORS node node-tail index index-tail)
foreach(accessor ${SOPATH_LEGACY_ACCESSORS})
  add_test(NAME SoPathLegacyAccessorBounds_${accessor}
    COMMAND SoPathLegacyAccessorBoundsTest ${accessor} all)
  set_tests_properties(SoPathLegacyAccessorBounds_${accessor}
    PROPERTIES LABELS sopath-legacy-accessor)
endforeach()
foreach(control valid visibility)
  add_test(NAME SoPathLegacyAccessorBounds_${control}
    COMMAND SoPathLegacyAccessorBoundsTest ${control})
  set_tests_properties(SoPathLegacyAccessorBounds_${control}
    PROPERTIES LABELS sopath-legacy-accessor)
endforeach()

add_executable(SoPathAssignmentAuditorTest
  reproducers/sopath-assignment-auditor-lifecycle/repro.cpp)
target_link_libraries(SoPathAssignmentAuditorTest
  Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoPathAssignmentAuditorTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(assignment_case reassign self-assign copy-construct)
  add_test(NAME SoPathAssignmentAuditor_${assignment_case}
    COMMAND SoPathAssignmentAuditorTest ${assignment_case})
endforeach()

add_executable(SoPathAuditorCallbackTest
  reproducers/sopath-auditor-callback-validation/repro.cpp)
target_link_libraries(SoPathAuditorCallbackTest
  Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoPathAuditorCallbackTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(callback_case empty foreign-parent negative-index null-replacement valid-callbacks)
  add_test(NAME SoPathAuditorCallback_${callback_case}
    COMMAND SoPathAuditorCallbackTest ${callback_case})
endforeach()

# Replacing operator new[] across a shared-library boundary is not portable
# on Windows, so keep the deterministic allocation-failure test on platforms
# where executable symbol interposition applies to the linked Coin library.
if(NOT WIN32)
  add_executable(CoinSbPListGrowthFailureTest
    SbPListGrowthFailureTest.cpp)
  target_link_libraries(CoinSbPListGrowthFailureTest
    Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CoinSbPListGrowthFailureTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME SbPListGrowthFailure COMMAND CoinSbPListGrowthFailureTest)

  add_executable(SoPathAssignmentAllocationFailureTest
    reproducers/sopath-assignment-auditor-lifecycle/allocation_failure.cpp)
  target_link_libraries(SoPathAssignmentAllocationFailureTest
    Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(SoPathAssignmentAllocationFailureTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME SoPathAssignmentAllocationFailure
    COMMAND SoPathAssignmentAllocationFailureTest)
endif()

add_executable(SoPathChildAppendTest SoPathChildAppendTest.cpp)
target_link_libraries(SoPathChildAppendTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoPathChildAppendTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SoPathChildAppend COMMAND SoPathChildAppendTest)

add_test(NAME CoinConfigCompatibility
  COMMAND ${CMAKE_COMMAND}
    -DCOIN_CONFIG_TEMPLATE=${PROJECT_SOURCE_DIR}/src/coin-config.cmake.in
    -DCOIN_COMPAT_TEST_DIR=${CMAKE_CURRENT_BINARY_DIR}/coin-config-compatibility
    -P ${CMAKE_CURRENT_SOURCE_DIR}/CoinConfigCompatibilityTest.cmake)

add_test(NAME CoinTestsFilterExact
  COMMAND CoinTests --run_test=SbList_TestSuite/truncate_zero_then_grow_past_default_size)
set_tests_properties(CoinTestsFilterExact PROPERTIES
  PASS_REGULAR_EXPRESSION "\\[OK\\] 1 tests")

add_test(NAME CoinTestsFilterWildcard
  COMMAND CoinTests -t SbList_TestSuite/truncate*)
set_tests_properties(CoinTestsFilterWildcard PROPERTIES
  PASS_REGULAR_EXPRESSION "\\[OK\\] 1 tests")

add_test(NAME CoinTestsFilterSuite
  COMMAND CoinTests --run_test=SbList_TestSuite)
set_tests_properties(CoinTestsFilterSuite PROPERTIES
  PASS_REGULAR_EXPRESSION "\\[OK\\] 5 tests")

add_test(NAME CoinTestsFilterCombined
  COMMAND CoinTests
    --run_test=SoCallbackList_TestSuite/typed_callback_copies_preserve_dispatch_and_lifetime,mixed_callback_removal_preserves_indices_and_last_duplicate:SbList_TestSuite/truncate_zero_then_grow_past_default_size)
set_tests_properties(CoinTestsFilterCombined PROPERTIES
  PASS_REGULAR_EXPRESSION "\\[OK\\] 3 tests")

add_test(NAME CoinTestsFilterRepeated
  COMMAND CoinTests
    --run_test=SbList_TestSuite/truncate_zero_then_grow_past_default_size
    --run_test=SoCallbackList_TestSuite/callback_mutations_affect_only_later_invocations)
set_tests_properties(CoinTestsFilterRepeated PROPERTIES
  PASS_REGULAR_EXPRESSION "\\[OK\\] 2 tests")

add_test(NAME CoinTestsFilterNoMatch
  COMMAND CoinTests --run_test=CoinTest_NoSuchSuite/*)
set_tests_properties(CoinTestsFilterNoMatch PROPERTIES WILL_FAIL TRUE)

add_test(NAME CoinTestsFilterMissingValue COMMAND CoinTests --run_test)
set_tests_properties(CoinTestsFilterMissingValue PROPERTIES WILL_FAIL TRUE)

add_test(NAME CoinTestsFilterEmptyValue COMMAND CoinTests --run_test=)
set_tests_properties(CoinTestsFilterEmptyValue PROPERTIES WILL_FAIL TRUE)

if(NOT WIN32)
  add_test(NAME CoinTestsFilterDirectSuite
    COMMAND CoinTests --run_test=GLUWrapper_TestSuite/runtimeLoader)
  set_tests_properties(CoinTestsFilterDirectSuite PROPERTIES
    PASS_REGULAR_EXPRESSION "\\[OK\\] 1 tests")
endif()

# These public-API tests need fresh processes: CoinTests initializes SoDB
# before running its inline cases, which is too late to test startup options.
add_executable(CoinProfilerInitTest
  reproducers/soprofiler-initclass-ordering/repro.cpp)
target_link_libraries(CoinProfilerInitTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinProfilerInitTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(profile_mode unset off 0 on 1 syncgl invalid)
  add_test(NAME ProfilerInit_${profile_mode}
    COMMAND ${CMAKE_COMMAND}
      -DTEST_EXECUTABLE=$<TARGET_FILE:CoinProfilerInitTest>
      -DPROFILE_MODE=${profile_mode}
      -P ${CMAKE_CURRENT_SOURCE_DIR}/reproducers/soprofiler-initclass-ordering/verify.cmake)
endforeach()

add_library(CoinCHeapHeaderCompileTest OBJECT CHeapHeaderCompileTest.c)
target_link_libraries(CoinCHeapHeaderCompileTest PRIVATE Coin)

include(CheckCoreGLHeaders.cmake)

# Many warnings are generated from test macros on macOS with Xcode.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-Wtautological-compare DISABLE_WARNING_TAUTOLOGCOMPARE)
if(DISABLE_WARNING_TAUTOLOGCOMPARE)
	target_compile_options(CoinTests PUBLIC -Wno-tautological-compare)
endif()
