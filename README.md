
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

To install the library, run
```console
cmake --install build
```

### Building Python Bindings
At the moment we recommend building and installing the Python bindings directly into a virtual environment.
You can do that by issuing `pip install .` from the project root, or `pip install -e .`
if you prefer an _editable_ install because you're working on the library code.

## Running the Examples

The subdirectory `examples/` contains a number of helpful examples that illustrate different use cases of this library.
You can run these examples e.g. with:

```console
./build/examples/core/simple
./build/examples/generator/generate_exhaustively
```

## Running the Tests
You can run the C++ tests with:
```console
./build/tests/dlp_tests
```

The Python bindings also come with their own set of tests. Run them with
```console
pytest api/python/
```
