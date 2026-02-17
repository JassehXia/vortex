#include "../include/InvertedIndex.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Auto-conversion for std::vector, std::map, etc.


namespace py = pybind11;

// PYBIND11_MODULE(name_of_module, variable_holding_module_def)
PYBIND11_MODULE(vortex, m) {
  m.doc() = "Vortex Search Engine C++ Core"; // Optional module docstring

  py::class_<DocFrequency>(m, "DocFrequency")
      .def_readonly("file_id", &DocFrequency::file_id)
      .def_readonly("frequency", &DocFrequency::frequency)
      .def("__repr__", [](const DocFrequency &a) {
        return "<DocFrequency file_id=" + std::to_string(a.file_id) +
               " frequency=" + std::to_string(a.frequency) + ">";
      });

  py::class_<InvertedIndex>(m, "InvertedIndex")
      .def(py::init<>()) // Constructor
      .def("add_document", &InvertedIndex::add_document,
           "Add a document string to index")
      .def("search", &InvertedIndex::search, "Search for a term")
      .def("save", &InvertedIndex::save, "Save index to binary file")
      .def("load", &InvertedIndex::load, "Load index from binary file");
}
