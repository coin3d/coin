cmake_minimum_required(VERSION 3.15)
project(CoinQuadMeshPrimitiveCount LANGUAGES CXX)

find_package(Coin CONFIG REQUIRED)

add_executable(coin-quadmesh-primitive-count main.cpp)
target_compile_features(coin-quadmesh-primitive-count PRIVATE cxx_std_11)
target_link_libraries(coin-quadmesh-primitive-count PRIVATE Coin::Coin)

enable_testing()
add_test(NAME quadmesh-primitive-count COMMAND coin-quadmesh-primitive-count)
