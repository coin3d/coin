cmake_minimum_required(VERSION 3.15)
project(CoinConsumer LANGUAGES CXX)

find_package(Coin CONFIG REQUIRED)

add_executable(coin-version main.cpp)
target_link_libraries(coin-version PRIVATE Coin::Coin)
