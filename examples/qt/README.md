# Qt example

Very basic Qt example. More examples can be found in the Quarter repository.

Building instructions:

    cmake -S <example path> -B <build path> -DCMAKE_PREFIX_PATH="<Qt path>;<Coin3D path>"
    cmake --build <build path> --config Release
    <build path>/Release/qt_example
