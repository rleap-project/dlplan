
# Description Logics State Features for Planning

Welcome to the "description logics state features for planning" library.

## Building the Library

Run the following from the project root to build the library.
By default, the library compiles in `Debug` mode.
```console
cmake -S . -B build
cmake --build build -j4
```

To build the library in `Release` mode, run
```console
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build -j4
```

## Running the Examples

The subdirectory `examples/` contains a number of helpful examples that illustrate different use cases of this library.
You can run these examples e.g. with:

```console
./build/examples/core/simple
./build/examples/generator/generate_exhaustively
```
