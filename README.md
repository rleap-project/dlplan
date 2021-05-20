
# dl-state-features-for-planning

A library for constructing and evaluating state features made up of description logics for planning.
Description logics uses concepts and roles. Concepts represent unary predicates and roles represent binary predicates. More complex concepts and roles can be constructed using an abstract syntax tree (AST) where the leafs are primitive concepts or primitive roles and the inner nodes are other types of concept and role constructors. To allow for efficient evaluation, each concept and role is checked for uniqueness and the library takes care that there are no doublicates, allowing for more efficiency in the state evaluation.
Hence, objects are typically constructed and returned as std::shared_ptr. We begin, with some code that we consider to be executed throughout the API description to simplify the examples.

```cpp
#include <dlf.h>
#include <memory>
#include <vector>
#include <utility>

using namespace std;
using ConceptElement = shared_ptr<dlf::DLConceptElement>;
using RoleElement = shared_ptr<dlf::RoleElement>;
using NumericalFeature = shared_ptr<dlf::NumericalFeature>;
using BooleanFeature = shared_ptr<dlf::BooleanFeature>;
using Concept = unsigned;
using Concepts = vector<Concept>;
using Role = pair<Concept, Concept>;
using Roles = vector<Role>;
```

## State Representation
The library supports a vector state representation.
In the vector representation, each element corresponds to the index of an atom.

```cpp
using State = vector<unsigned>;
```

## Library Initialization

In the simplest case, let the library take care of the indexing of predicates and objects.
The library assign the first index (0) to the first added atom,
the second index (1) to the second added atom and so on.
The caller must only take care that atoms are added in the correct order.

```cpp
int main() {
    // Initialize library
    dlf::DLF dlf();
    // Pass each ground atoms of the planning instance
    dlf.add_atom(dlf::Atom(predicate_name="on", object_names={"objA", "objB"}));
    // ...
}
```

Alternatively, the caller can take responsibility of assigning the indices to predicates and objects.

```cpp
int  main() {
    // Initialize library
    dlf::DLF dlf();
    // Pass each ground atoms of the planning instance
    dlf.add_atom(dlf::Atom(predicate_name="on", predicate_idx=0,
                           object_names={"objA", "objB"}, object_idx={0, 1}));
    // ...
}
```

##  Constructing Concepts and Roles

```cpp
int main() {
    // Initialize library
    // ...
    // The parsed term yields a concept. Can throw parsing errors if unsuccesful.
    ConceptElement concept_element = dlf.make_concept_element("AND(on(0))");
    // The parsed term yields a role. Can throw parsing errors if unsuccesful.
    RoleElement role_element = dlf.make_role_element("AND(on(0,1)");
}
```

## Evaluating Concepts and Roles

```cpp

int main() {
    // Initialize library
    // ...
    // Construct an example State.
    State state({1, 2, 42});
    // Construct a ConceptElement.
    ConceptElement concept_element = dlf.make_concept_element("AND(on(0))");
    // Evaluate a ConceptElement.
    Concepts concepts = concept_element.evaluate(state);
    // Construct a RoleElement.
    RoleElement role_element = dlf.make_role_element("AND(on(0,1)");
    // Evaluate a RoleElement.
    Roles roles = role_element.evaluate(state);

}
```

## From Concepts and Roles to Features

```cpp
int main() {
    // Initialize library
    // ...
    // Construct a ConceptElement
    ConceptElement concept_element = dlf.make_concept_element("AND(on(0))");
    // Construct a CountFeature using the ConceptElement
    NumericalFeature count_feature = dlf.make_count_feature(concept_element);
    // Construct an EmptyFeature using the ConceptElement
    BooleanFeature count_feature = dlf.make_empty_feature(concept_element);
}
```

## Evaluating Features

```cpp
int main() {
    // Initialize library
    // ...
    // Construct an example State.
    State state({1, 2, 42});
    // Construct a ConceptElement.
    ConceptElement concept_element = dlf.make_concept_element("AND(on(0))");
    // Construct a CountFeature using the ConceptElement.
    NumericalFeature count_feature = dlf.make_count_feature(concept_element);
    // Evaluate the CountFeature.
    int value = count_feature.evaluate(state);
}
```
