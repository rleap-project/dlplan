# Description Logics State Features for Planning Library (DLPlan)

Welcome to the DLPlan-library.

## 1. Features of the DLPlan-library

In this section, we start with defining some background and then describe the library components. Given a set of classical planning instances Q =  {P1, P2, ..., Pn} where each Pi with i=1,...,n consists of a state space S_i and each state s in S_i consists of ground atoms over a common planning domain D. For example, Q can be a set of planning instances from the Delivery domain where there is a set of trucks, packages, and locations. Next, we describe the components in more detail and refer to the `include` subdirectory for the C++ interface, the `api/python` subdirectory for the Python interface, and the `examples` subdirectory on how to use them.

### 1.1. Core

A numerical feature is n : S -> {0,1,...} and respectively, a Boolean feature is b : S -> {0,1}. Such features allow us to define abstract knowledge on top of each state. Furthermore, we are interested in features are meaningful for states coming from planning instances over the domain D. For example, let n denote the numerical feature that counts the number of undelivered packages in any given Delivery instance.

We are specifically interested in features that are understandable. Hence, we make use of a description logics grammar that is a fraction of two variable first-order logic and has shown to be useful in several works. Description logic consists of two types of objects that are concepts and roles. Concepts correspond to derived unary predicates and Roles correspond to derived binary predicates. By iteratively applying the grammar rules one can derive numerical or Boolean features. We included `guide.pdf` in the `docs` folder with further information regarding the available elements with a description of their syntax and semantics.

The core component provides functionality for constructing and evaluating features for the class of problems Q.

### 1.2. Generator

Consider the case where we want to automatically learn knowledge from the class of problems Q that makes use of numerical and Boolean features. The question then becomes: what are informative features for the learning task?

The generator component provides functionality for automatically generating a set of features F with complexity at most k such that for each f,f' in F there exists a state s in Si such that f yields a different valuation on s than f'.

### 1.3. Evaluator

Consider the case where we want to build additional structures on top of the numerical and Boolean features and evaluate them during a state space search. In order to evaluate features only once for a given state, we want to cache the feature valuations.

The evaluation component provides functionality for caching feature valuations per state and for a fixed set of features.

### 1.4. Policy

Consider the case where we want to define rules of the form C -> E where C is a set of feature conditions and E is set of feature effects. For a given pair of states (s, s') we say that (s, s') is compatible with C -> E if s satisfies C and (s, s') satisfies E. Feature conditions are: n=0, n>0 for numerical feature n with expected semantics, and b, not b for Boolean feature b with expected semantics. Feature effects are numerical increments, i.e., f(s) < f(s'), decrements, and whether boolean feature becomes true or false.

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
