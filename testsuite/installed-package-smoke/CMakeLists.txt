cmake_minimum_required(VERSION 3.15)

project(CoinInstalledPackageSmoke LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Coin CONFIG REQUIRED)

if(NOT DEFINED COIN_EXPECT_LEGACY_GL_RENDERER OR
   NOT COIN_EXPECT_LEGACY_GL_RENDERER MATCHES "^(ON|OFF)$")
  message(FATAL_ERROR "COIN_EXPECT_LEGACY_GL_RENDERER must be ON or OFF")
endif()

get_target_property(_coin_consumer_includes Coin::Coin INTERFACE_INCLUDE_DIRECTORIES)
foreach(_coin_include IN LISTS _coin_consumer_includes)
  if(_coin_include MATCHES "[/\\]src([/\\]|$)" OR
     _coin_include MATCHES "glue[/\\]khronos")
    message(FATAL_ERROR
      "Installed Coin target exposes a private source include path: ${_coin_include}")
  endif()
endforeach()

add_executable(coin-installed-package-smoke main.cpp)
target_link_libraries(coin-installed-package-smoke PRIVATE Coin::Coin)
target_compile_definitions(coin-installed-package-smoke PRIVATE
  COIN_EXPECT_LEGACY_GL_RENDERER=$<BOOL:${COIN_EXPECT_LEGACY_GL_RENDERER}>
)
