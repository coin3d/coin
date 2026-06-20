# HelloWorld example

Very basic helloworld example to get started with Coin3D. 

Building instructions:

    cmake -S <example path> -B <build path> -DCMAKE_PREFIX_PATH="<Qt5 path>;<Coin3D path>"
    cmake --build <build path> --config Release
    <build path>/Release/coin_helloworld
