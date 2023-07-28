#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors
#include <pybind11/iostream.h>
#include <pybind11/functional.h>
#include <pybind11/embed.h> // everything needed for embedding

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/serialization.h"
#include "../../../include/dlplan/state_space.h"

#include <sstream>

namespace py = pybind11;

using namespace dlplan::serialization;


void init_serialization(py::module_ &m_serialization) {
    py::class_<Data>(m_serialization, "Data")
        .def(py::init<>())
        .def_readwrite("state_spaces", &Data::state_spaces)
    ;

    m_serialization.def("serialize", [](const Data& data){
        std::stringstream buffer;
        serialize(data, buffer);
        return buffer.str();
    });

    m_serialization.def("deserialize", [](const std::string& text){
        std::stringstream buffer(text);
        return deserialize(buffer);
    });
}
