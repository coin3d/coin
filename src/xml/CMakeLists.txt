add_subdirectory(expat)

add_library(xml OBJECT
        document.cpp
        element.cpp
        attribute.cpp
        entity.cpp
        utils.cpp
        path.cpp
)
