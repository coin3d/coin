cmake_minimum_required (VERSION 3.21)
project(qt_example)

find_package (Coin REQUIRED)
find_package (Quarter REQUIRED)
find_package (Qt5 COMPONENTS Core Widgets REQUIRED)
 
add_executable (qt_example 
    "qt_example.cpp"
)

target_link_libraries(qt_example
    PRIVATE
        Coin::Coin
        Quarter::Quarter
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
