find_package(glfw3 3.3 REQUIRED)
find_package(yaml-cpp REQUIRED)

add_executable(CoinRenderTests
  src/Main.cpp
  src/SnapshotCommand.cpp
  src/CompareCommand.cpp
  src/RunCommand.cpp
  src/GLFWBackend.cpp
  src/RenderCore.cpp
  src/SpecLoader.cpp
  src/CompareCore.cpp
  ${PROJECT_SOURCE_DIR}/thirdparty/whereami/src/whereami.c)

target_include_directories(CoinRenderTests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include/Inventor/annex
  ${PROJECT_SOURCE_DIR}/thirdparty/whereami/src
  ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})

target_compile_features(CoinRenderTests PRIVATE cxx_std_17)

target_link_libraries(CoinRenderTests PRIVATE
  glfw
  Coin
  stb
  yaml-cpp::yaml-cpp
  ${COIN_TARGET_LINK_LIBRARIES})
