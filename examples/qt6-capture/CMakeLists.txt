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
