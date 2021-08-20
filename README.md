
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

## Available Elements:

Consider concepts C,D and roles R,S,T.

### Concepts

- Value Restriction c_all(R,C): <img src="https://render.githubusercontent.com/render/math?math=\forall R.C\equiv \{a\in\triangle\mid\forall b.(a,b)\in R\rightarrow b\in C\}">
- Intersection c_and(C,D): <img src="https://render.githubusercontent.com/render/math?math=C\sqcap D\equiv C\cap D">
- Bottom c_bot: <img src="https://render.githubusercontent.com/render/math?math=\bot = \emptyset">
- Difference c_diff(C,D): <img src="https://render.githubusercontent.com/render/math?math=C\setminus D\equiv C\sqcap \neg D">
- Negation c_not(C): <img src="https://render.githubusercontent.com/render/math?math=\neg C = \top\setminus C">
- One-of c_one_of(x): <img src="https://render.githubusercontent.com/render/math?math=x = \{x\} ">
- Union c_or(C,D): <img src="https://render.githubusercontent.com/render/math?math=C\sqcup D\equiv C\cup D">
- Primitive predicate_name(pos): <img src="https://render.githubusercontent.com/render/math?math=\text{predicate_name}[\text{pos}]\equiv\{c_{\text{pos}}\in\top\mid \text{predicate_name}(c_1,\ldots,c_{pos},\ldots,c_n)\in s  \}">
- Projection c_projection(R,pos): <img src="https://render.githubusercontent.com/render/math?math=R[0]\equiv\exists R.\top, R[1]\equiv\exists R^{-1}.\top">
- Existential abstraction c_some(R,C): <img src="https://render.githubusercontent.com/render/math?math=\exists R.C\equiv \{a\in\triangle\mid\exists b:(a,b)\in R\land b\in C  \}">
- Role-value-map c_subset(R,S): <img src="https://render.githubusercontent.com/render/math?math=R\subseteq S\equiv \{a\in\triangle\mid \forall (a,b)\in R\rightarrow (a,b)\in S \}">
- Top c_top: <img src="https://render.githubusercontent.com/render/math?math=\top = \triangle">

### Roles

- Intersection r_and(R,S)
- Composition r_compose(R,S)
- Identity r_identity(C)
- Inverse r_inverse(R)
- Negation r_not(R)
- Union r_or(R,S)
- Primitive predicate_name(pos_1,pos_2)
- Restriction r_restrict(R,C)
- Universal role r_top
- Transitive Closure r_transitive_closure(R)
- Reflexive transitive closure r_reflexive_transitive_closure(R)

### Numericals

- Concept distance n_concept_distance(C,R,D)
- Sum concept distance n_sum_concept_distance(C,R,D)
- Role distance n_role_distance(R,S,T)
- Sum role distance n_sum_role_distance(R,S,T)
- Count n_count(R) or n_count(C)

### Booleans

- Empty b_empty(R) or b_empty(C)

