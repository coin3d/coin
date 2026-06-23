cmake_minimum_required (VERSION 3.21)
project(coin_helloworld)

find_package (Coin REQUIRED)
find_package (SoQt REQUIRED)
find_package (Qt5 COMPONENTS Core Widgets REQUIRED)
 
add_executable (coin_helloworld 
    "coin_helloworld.cpp"
)

target_link_libraries(coin_helloworld
    PRIVATE
        Coin::Coin
        SoQt::SoQt
        Qt5::Core
        Qt5::Widgets
) 

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${Qt5_DIR}/../../../plugins/platforms/qwindows.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
    )
endif()
