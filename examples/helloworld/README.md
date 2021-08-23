# HelloWorld example

Very basic helloworld example to get started with Coin3d. 

Building instructions:

    cd <source path of example>
    mkdir build && cd build
    cmake -DQt5_DIR=<path of Qt5> -DCoin_DIR=<path of Coin3d lib> -DSoQt_DIR=<path of SoQt lib> ..
    cmake --build .

