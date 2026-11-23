# Using an installed Coin package with CMake

This standalone C++ example uses the installed CMake package and prints the Coin version.

From the Coin source directory, configure and build the example. Set
`CMAKE_PREFIX_PATH` to the Coin installation prefix if CMake needs help finding it.

```sh
cmake -S examples/cmake-consumer -B consumer-build -DCMAKE_PREFIX_PATH=/path/to/coin-install
cmake --build consumer-build --config Release
./consumer-build/coin-version
```

On Windows, use a prefix such as `C:/Coin3D`. With the Visual Studio generator,
run `consumer-build/Release/coin-version.exe` with the Coin installation's
`bin` directory on `PATH`.

In your own project, `find_package(Coin CONFIG REQUIRED)` finds the package.
Link your executable or library to `Coin::Coin` to inherit the package's
public include paths and link requirements.
