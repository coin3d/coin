cmake_minimum_required (VERSION 3.8)
 

find_package (Coin COMPONENTS Coin4 REQUIRED)
find_package (SoQt COMPONENTS SoQt REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
 
add_executable (coin_helloworld 
"coin_helloworld.cpp"
)

target_link_libraries(coin_helloworld  PRIVATE 
Coin::Coin SoQt::SoQt
Qt5::Core Qt5::Widgets
) 

