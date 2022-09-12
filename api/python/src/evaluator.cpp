#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/evaluator.h"

namespace py = pybind11;

using namespace dlplan::core;
using namespace dlplan::evaluator;


void init_evaluator(py::module_ &m) {
    py::class_<EvaluationCache>(m, "EvaluationCache")
        .def(py::init<int, int>())
        .def("__copy__", [](const EvaluationCache& cache, py::object){ return EvaluationCache(cache); })
        .def("__deepcopy__", [](const EvaluationCache& cache, py::object){ return EvaluationCache(cache); })
        .def("retrieve_or_evaluate", py::overload_cast<const Boolean&, const State&>(&EvaluationCache::retrieve_or_evaluate))
        .def("retrieve_or_evaluate", py::overload_cast<const Numerical&, const State&>(&EvaluationCache::retrieve_or_evaluate))
    ;
}
