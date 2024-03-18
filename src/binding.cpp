#include <nanobind/nanobind.h>
#include <nanobind/stl/string_view.h>
#include "ada.h"

namespace py = nanobind;

NB_MODULE(can_ada, m) {
#ifdef VERSION_INFO
    m.attr("__version__") = Py_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif

    m.def("can_parse",
          &ada::can_parse,
          py::arg("input"),
          py::arg("base_input") = static_cast<std::string_view*>(nullptr));

    py::class_<ada::url_aggregator>(m, "URL")
        .def_prop_rw("hash", &ada::url_aggregator::get_hash, &ada::url_aggregator::set_hash)
        .def_prop_rw("host", &ada::url_aggregator::get_host, &ada::url_aggregator::set_host)
        .def_prop_rw("hostname", &ada::url_aggregator::get_hostname, &ada::url_aggregator::set_hostname)
        .def_prop_rw("href", &ada::url_aggregator::get_href, &ada::url_aggregator::set_href)
        .def_prop_ro("origin", &ada::url_aggregator::get_origin)
        .def_prop_rw("password", &ada::url_aggregator::get_password, &ada::url_aggregator::set_password)
        .def_prop_rw("pathname", &ada::url_aggregator::get_pathname, &ada::url_aggregator::set_pathname)
        .def_prop_ro("pathname_length", &ada::url_aggregator::get_pathname_length)
        .def_prop_rw("port", &ada::url_aggregator::get_port, &ada::url_aggregator::set_port)
        .def_prop_rw("protocol", &ada::url_aggregator::get_protocol, &ada::url_aggregator::set_protocol)
        .def_prop_rw("search", &ada::url_aggregator::get_search, &ada::url_aggregator::set_search)
        .def_prop_rw("username", &ada::url_aggregator::get_username, &ada::url_aggregator::set_username)

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
        .def("validate", &ada::url_aggregator::validate);

    m.def("idna_decode", [](py::bytes input) -> py::str {
        auto result = ada::idna::to_unicode(input.c_str());
        return py::str(result.c_str());
    });

    m.def("idna_encode", [](const std::string_view input) -> py::bytes {
        auto result = ada::idna::to_ascii(input);
        return py::bytes(result.c_str(), result.size());
    });

    m.def("parse", [](const std::string_view input) {
        ada::result<ada::url_aggregator> url = ada::parse<ada::url_aggregator>(input);
        if (!url) {
            throw py::value_error("URL could not be parsed.");
        }
        return url.value();
    });

}