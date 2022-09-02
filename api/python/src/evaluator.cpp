#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/evaluator.h"

namespace py = pybind11;

using namespace dlplan;

void init_evaluator(py::module_ &m) {
    py::class_<evaluator::EvaluationCache>(m, "EvaluationCache")
        .def(py::init<int, int>())
        .def("retrieve_or_evaluate", py::overload_cast<const core::Boolean&, const core::State&>(&evaluator::EvaluationCache::retrieve_or_evaluate))
        .def("retrieve_or_evaluate", py::overload_cast<const core::Numerical&, const core::State&>(&evaluator::EvaluationCache::retrieve_or_evaluate))
    ;
}
