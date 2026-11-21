cmake_minimum_required(VERSION 3.15)

project(CoinInstalledPackageSmoke LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Coin CONFIG REQUIRED)

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
