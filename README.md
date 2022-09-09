# Description Logics State Features for Planning Library (DLPlan)

Welcome to the DLPlan-library.

## 0. Background

### 0.1. State Language

We consider a set of predicates where each predicate has the form p\n where p is a name and n is the number of arguments coming from a set of objects. A unary predicate takes exactly one argument and a binary predicates takes exactly two arguments. An atom is a predicate p where each argument takes one of the objects. A state is a set of atoms.

### 0.2. Classical Planning

We consider a set of classical planning instances Q = {P_1, P_2, ..., P_n} where each P in Q consists of a set of states over a common state language.

### 0.3. Description Logics

There are two types of objects in description logics: concepts and roles. A Concept C is an additional unary predicate C\1 a role R is an additional binary predicate R\2. There are several base grammar rules and inductive grammar rules in description logics. Their interpretation on the states yield sets of atoms over the unary (resp. binary) predicates. Counting the number of ground atoms yield the valuation for a numerical feature n : S -> {0,1,...} or checking whether there exists a least one ground atoms yields a Boolean feature b : S -> {0,1}. Since we assume a common state language for all planning instances in Q, we can evaluate the features on any given state from any planning instane P in Q.

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

### 1.5. State Space

The state space component allows to generate state spaces from PDDL input files.

### 1.6 Weisfeiler-Lehman

The weisfeiler-lehman component allows to identify subgraphs that are indistinguishable
by two-variable first order logic and hence, are also indistinguishable by less expressive description logics.

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

### 2.2. Additional Compile Flags

- DENABLE_TESTING:BOOL=TRUE enables compilation of tests
- DENABLE_SPARSE:BOOL=TRUE enabled compilation with sparse set representation for concept and role denotations. Recommended for large number of objects.
- DPYTHON_EXECUTABLE:FILEPATH=/path/to/python to manually set path to python interpreter to install scorpion. The path can be obtained with
```console
where python
```

### 2.3. Building the Python Interface

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
cd build && ctest
```

The Python bindings also come with their own set of tests. Run them with
```console
python3 -m pytest api/python/
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

## 7. Publications List

- Learning and Exploiting Progress States in Greedy Best-First Search, Patrick Ferber, Liat Cohen, Jendrik Seipp, and Thomas Keller, International Joint Conference on Artificial Intelligence, 2022, Vienna, Austria
- Learning Sketches for Decomposing Planning Problems into Subproblems of Bounded Width, Dominik Drexler, Jendrik Seipp, and Hector Geffner, International Conference on Automated Planning and Scheduling, 2022, Singapore, Singapore
