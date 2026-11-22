cmake_minimum_required(VERSION 3.21)
project(CoinExampleCapture LANGUAGES CXX)

find_package(Coin CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Quarter 1.2.3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

set(IVEXAMPLES_DIR "" CACHE PATH "Path to the ivexamples checkout")
set(toolmaker "${IVEXAMPLES_DIR}/Toolmaker/02.Nodes")
set(mentor_source "${IVEXAMPLES_DIR}/Mentor/05.2.IndexedFaceSet.cpp.in")
if(NOT EXISTS "${mentor_source}" OR NOT EXISTS "${toolmaker}/newNodes.iv")
  message(FATAL_ERROR "Set IVEXAMPLES_DIR to the ivexamples checkout listed in README.md")
endif()

# Reuse the Mentor mesh with its original license and without the SoQt entry point.
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${mentor_source}")
file(READ "${mentor_source}" mesh)
string(FIND "${mesh}" "// CODE FOR The Inventor Mentor ENDS HERE" mesh_end)
if(mesh_end EQUAL -1)
  message(FATAL_ERROR "The IndexedFaceSet source does not match the documented ivexamples revision")
endif()
string(SUBSTRING "${mesh}" 0 "${mesh_end}" mesh)
string(REGEX REPLACE "#include <Inventor/@Gui@/[^>]+>\n" "" mesh "${mesh}")
file(CONFIGURE OUTPUT IndexedFaceSet.cpp CONTENT "${mesh}" @ONLY)

set(SIM_INCLUDE_GL_H "#include <Inventor/system/gl.h>")
configure_file("${toolmaker}/Pyramid.cpp.in" Pyramid.cpp @ONLY)

add_executable(coin-example-capture
  main.cpp scenes.cpp
  "${CMAKE_CURRENT_BINARY_DIR}/IndexedFaceSet.cpp"
  "${CMAKE_CURRENT_BINARY_DIR}/Pyramid.cpp"
  "${toolmaker}/Glow.cpp" "${toolmaker}/Alternate.cpp")
target_compile_features(coin-example-capture PRIVATE cxx_std_17)
target_include_directories(coin-example-capture PRIVATE "${toolmaker}")
target_link_libraries(coin-example-capture PRIVATE
  Coin::Coin Quarter::Quarter Qt6::Widgets Qt6::OpenGLWidgets OpenGL::GL)

# The projector example writes the Inventor scene shown in its reference page.
set(projector_source "${CMAKE_CURRENT_SOURCE_DIR}/../../src/projectors/SbSphereSheetProjector.cpp")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${projector_source}")
file(READ "${projector_source}" projector)
string(FIND "${projector}" "\\code" projector_start)
string(FIND "${projector}" "\\endcode" projector_end)
if(projector_start LESS 0 OR projector_end LESS projector_start)
  message(FATAL_ERROR "Could not find the SbSphereSheetProjector example")
endif()
math(EXPR projector_start "${projector_start} + 5")
math(EXPR projector_length "${projector_end} - ${projector_start}")
string(SUBSTRING "${projector}" "${projector_start}" "${projector_length}" projector)
file(CONFIGURE OUTPUT SphereSheetProjector.cpp CONTENT "${projector}" @ONLY)
add_executable(coin-projector-example "${CMAKE_CURRENT_BINARY_DIR}/SphereSheetProjector.cpp")
target_link_libraries(coin-projector-example PRIVATE Coin::Coin)
