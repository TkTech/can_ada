#include <pybind11/pybind11.h>
#include "ada.h"

namespace py = pybind11;

PYBIND11_MODULE(can_ada, m) {
#ifdef VERSION_INFO
    m.attr("__version__") = Py_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif

    m.def("can_parse",
          &ada::can_parse,
          py::arg("input"),
          py::arg("base_input") = static_cast<const std::string_view*>(nullptr));

    py::class_<ada::url_aggregator>(m, "URL")
        .def_property("hash", &ada::url_aggregator::get_hash, &ada::url_aggregator::set_hash)
        .def_property("host", &ada::url_aggregator::get_host, &ada::url_aggregator::set_host)
        .def_property("hostname", &ada::url_aggregator::get_hostname, &ada::url_aggregator::set_hostname)
        .def_property("href", &ada::url_aggregator::get_href, &ada::url_aggregator::set_href)
        .def_property("origin", &ada::url_aggregator::get_origin, nullptr)
        .def_property("password", &ada::url_aggregator::get_password, &ada::url_aggregator::set_password)
        .def_property("pathname", &ada::url_aggregator::get_pathname, &ada::url_aggregator::set_pathname)
        .def_property("pathname_length", &ada::url_aggregator::get_pathname_length, nullptr)
        .def_property("port", &ada::url_aggregator::get_port, &ada::url_aggregator::set_port)
        .def_property("protocol", &ada::url_aggregator::get_protocol, &ada::url_aggregator::set_protocol)
        .def_property("search", &ada::url_aggregator::get_search, &ada::url_aggregator::set_search)
        .def_property("username", &ada::url_aggregator::get_username, &ada::url_aggregator::set_username)

        .def("has_credentials", &ada::url_aggregator::has_credentials)
        .def("has_empty_hostname", &ada::url_aggregator::has_empty_hostname)
        .def("has_hash", &ada::url_aggregator::has_hash)
        .def("has_hostname", &ada::url_aggregator::has_hostname)
        .def("has_non_empty_password", &ada::url_aggregator::has_non_empty_password)
        .def("has_non_empty_username", &ada::url_aggregator::has_non_empty_username)
        .def("has_password", &ada::url_aggregator::has_password)
        .def("has_port", &ada::url_aggregator::has_port)
        .def("has_search", &ada::url_aggregator::has_search)
        .def("has_valid_domain", &ada::url_aggregator::has_valid_domain)

        .def("to_diagram", &ada::url_aggregator::to_diagram)
        .def("__str__", &ada::url_aggregator::get_href)
        .def("validate", &ada::url_aggregator::validate)

        .def("__add__", [](const ada::url_aggregator &self, std::string_view other) {
          ada::result<ada::url_aggregator> url = ada::parse<ada::url_aggregator>(other, &self);
          if (!url) {
              throw pybind11::value_error("URL could not be parsed.");
          }
          return url.value();
        });

    m.def("idna_decode", &ada::idna::to_unicode);
    m.def("idna_encode", [](std::string input) -> py::bytes {
      return py::bytes(ada::idna::to_ascii(input));
    });

    m.def("parse", [](std::string_view input) {
        ada::result<ada::url_aggregator> url = ada::parse<ada::url_aggregator>(input);
        if (!url) {
            throw pybind11::value_error("URL could not be parsed.");
        }
        return url.value();
    });

}