#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "ada.h"

namespace py = pybind11;

PYBIND11_MODULE(can_ada, m) {
#ifdef VERSION_INFO
    m.attr("__version__") = Py_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif

    m.def("can_parse", [](std::string_view input, std::optional<const std::string_view> base_input) {
        if (base_input.has_value()) {
            return ada::can_parse(input, &base_input.value());
        }
        return ada::can_parse(input);
    }, py::arg("input"), py::arg("base_input") = py::none());

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

    py::class_<ada::url_search_params_keys_iter>(m, "URLSearchParamsKeysIter")
        .def("__iter__", [](ada::url_search_params_keys_iter &self) {
            return &self;
        })
        .def("__next__", [](ada::url_search_params_keys_iter &self) {
            if (self.has_next()) {
                return self.next();
            } else {
                throw pybind11::stop_iteration();
            }
        });

    py::class_<ada::url_search_params_values_iter>(m, "URLSearchParamsValuesIter")
        .def("__iter__", [](ada::url_search_params_values_iter &self) {
            return &self;
        })
        .def("__next__", [](ada::url_search_params_values_iter &self) {
            if (self.has_next()) {
                return self.next();
            } else {
                throw pybind11::stop_iteration();
            }
        });

    py::class_<ada::url_search_params>(m, "URLSearchParams")
        .def(py::init<>())
        .def(py::init<const std::string_view>())
        .def("get", &ada::url_search_params::get)
        .def("get_all", &ada::url_search_params::get_all)
        .def("has", [](ada::url_search_params &self, std::string_view key, std::optional<std::string_view> value) {
            if (value) {
                return self.has(key, value.value());
            }
            return self.has(key);
        }, py::arg("key"), py::arg("value") = py::none())
        .def("append", &ada::url_search_params::append)
        .def("remove", [](ada::url_search_params &self, std::string_view key, std::optional<std::string_view> value) {
            if (value) {
                self.remove(key, value.value());
                return;
            }
            self.remove(key);
        }, py::arg("key"), py::arg("value") = py::none())
        .def("copy", [](ada::url_search_params &self) {
            return ada::url_search_params(self);
        })
        .def("sort", &ada::url_search_params::sort)
        .def("size", &ada::url_search_params::size)
        .def("keys", [](ada::url_search_params &self) {
            return self.get_keys();
        }, py::keep_alive<0, 1>())
        .def("values", [](ada::url_search_params &self) {
            return self.get_values();
        }, py::keep_alive<0, 1>())
        .def("__str__", &ada::url_search_params::to_string)
        .def("__getitem__", [](ada::url_search_params &self, std::string_view key) {
            auto v = self.get(key);
            if (v) {
                return v.value();
            }
            throw pybind11::key_error("Key not found.");
        })
        .def("__setitem__", &ada::url_search_params::set)
        .def("__delitem__", [](ada::url_search_params &self, std::string_view key) {
            self.remove(key);
        })
        .def("__len__", &ada::url_search_params::size)
        .def("__contains__", [](ada::url_search_params &self, std::string_view key) {
            return self.has(key);
        })
        .def("__iter__", [](ada::url_search_params &self) {
            return py::make_iterator(self.begin(), self.end());
        }, py::keep_alive<0, 1>());

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