
# Description Logics State Features for Planning

Welcome to the "description logics state features for planning" library.

## Building the Library

Run the following from the project root to build the library.
By default, the library compiles in `Debug` mode.
```console
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -S . -B build
cmake --build build -j4
```

To build the library in `Release` mode, run
```console
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/ -S . -B build
cmake --build build -j4
```

To install the library, run
```console
cmake --install build
```

Don't forget to update environment variable `CPLUS_INCLUDE_PATH` and `LD_LIBRARY_PATH` if necessary.

### Building Python Bindings
At the moment we recommend building and installing the Python bindings directly into a virtual environment.
You can do that by issuing `pip install .` from the project root, or `pip install -e .`
if you prefer an _editable_ install because you're working on the library code.

## Running the Examples

The subdirectory `examples/` contains a number of helpful examples that illustrate different use cases of this library.
You can run the C++ examples with
```console
./build/examples/core/simple
./build/examples/generator/generate_exhaustively
```

The Python bindings also come with the same examples. Run them with
```console
python3 examples/core/simple.py
python3 examples/generator/generate_exhaustively.py
```

## Running the Tests
You can run the C++ tests with:
```console
./build/tests/dlplan_tests
```

The Python bindings also come with their own set of tests. Run them with
```console
pytest api/python/
```

or use the tox test tool. Run it with
```console
tox
```

## Available Elements:

We included `guide.pdf` in the `docs` folder with further information regarding the available elements with a description of their syntax and semantics.

## Modifying/Extending the library:

1. Adding new elements to [core](https://github.com/rleap-project/dlplan/wiki/Adding-elements-to-the-core-component) and [generator](https://github.com/rleap-project/dlplan/wiki/Adding-elements-to-the-generator-component) component.

2. [Current order and modifying the order of feature generation.](https://github.com/rleap-project/dlplan/wiki/Modifying-the-order-of-feature-generation)

## Citing dlplan

We created a DOI on zenodo under this [link](https://zenodo.org/record/5826140#.YfK9E_so85k). A bibtex entry can look like this:

```
@software{drexler_dominik_2022_5826140,
  author       = {Drexler, Dominik and
                  Francès, Guillem and
                  Seipp, Jendrik},
  title        = {{Description Logics State Features for Planning 
                   (dlplan)}},
  year         = 2022,
  publisher    = {Zenodo},
  doi          = {10.5281/zenodo.5826140},
  url          = {https://doi.org/10.5281/zenodo.5826140}
}
```
