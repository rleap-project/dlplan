# Description Logics State Features for Planning Library (DLPlan)

Welcome to the DLPlan-library.

## 0. Background 

### 0.1. Classical Planning

We consider a set of classical planning instances Q =  {P_1, P_2, ..., P_n} where each P in Q consists of a state space S and each state s in S consists of ground atoms over a common planning domain D. The ground atoms have form p(o_1,...,o_k) where p is a predicate occuring in D and o_1,...,o_k are objects that occur in the instance specific part. For example, Q can be a set of planning instances from the Delivery domain where each planning instance consists of a set of packages, and a set of locations. Note that all those planning instance use the same state language allowing us to define domain general features by starting from primitives that correspond to the predicates and composing them to more complex features using description logics.

### 0.2. Description Logics

There are two types of objects in description logics: concepts and roles. Concepts can be seen as derived unary predicates and Roles and derived binary predicates. There are several base grammar rules and inductive grammar rules. Their interpretation on the states yield sets of ground atoms over the derived unary (resp. binary) predicates. Counting the number of ground atoms yield the valuation for a numerical feature n : S -> {0,1,...} or checking whether there exists a least one ground atoms yields a Boolean feature b : S -> {0,1}

## 1. Features of the DLPlan-library

The library consists of four components. Each having its own public header file, examples, tests, and python bindings.

### 1.1. Core

The core component provides functionality for constructing and evaluating features for the class of problems Q. We included `guide.pdf` in the `docs` folder with further information regarding the available elements with a description of their syntax and semantics.

### 1.2. Generator

The generator component provides functionality for automatically generating a set of features F with complexity at most k such that for each f,f' in F there exists a state s in Si such that f yields a different valuation on s than f'.

### 1.3. Evaluator

The evaluation component provides functionality for caching feature valuations per state and for a fixed set of features.

### 1.4. Policy

The policy component allows to construct a policy and evaluate state pairs (s, s') on it.

## 2. Building the Library

### 2.1. Building the C++ Interface

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

### 2.2. Building the Pyhon Interface

At the moment we recommend building and installing the Python bindings directly into a virtual environment.

You can do that by issuing `pip install .` from the project root, or `pip install -e .`

if you prefer an _editable_ install because you're working on the library code.

## 3. Running the Examples

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

## 4. Running the Tests

You can run the C++ tests with:
```console
./build/tests/dlplan_tests
```

The Python bindings also come with their own set of tests. Run them with
```console
pytest api/python/
```

## 5. Profiling

In the `experiments/` directory, we provide code to profile parts of the library. To run the experimental code, it is necessary to install the modified version of `tarski` that we provide in the respective submodule in the `submodules/tarski` directory.

## 6. Citing DLPlan

We created a DOI on zenodo under this [link](https://zenodo.org/record/5826140#.YfK9E_so85k). A bibtex entry can look like this:

```
@software{drexler-et-al-dlplan2022,
author = {Drexler, Dominik and
Francès, Guillem and
Seipp, Jendrik},
title = {{DLPlan}},
year = 2022,
publisher = {Zenodo},
doi = {10.5281/zenodo.5826140},
url = {https://doi.org/10.5281/zenodo.5826140}
}
```
