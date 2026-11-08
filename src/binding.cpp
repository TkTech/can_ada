#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/string_view.h>
#include <nanobind/stl/vector.h>
#include "ada.h"
#include "docstrings.h"

namespace py = nanobind;

NB_MODULE(can_ada, m) {
    m.doc() = "WHATWG URL parsing, query parameters, and IDNA conversion.";
#ifdef VERSION_INFO
    m.attr("__version__") = Py_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif

    m.def("can_parse", [](std::string_view input) {
        return ada::can_parse(input);
    }, py::arg("input"), docs::can_parse);

    m.def("can_parse", [](std::string_view input, std::string_view base_input) {
        return ada::can_parse(input, &base_input);
    }, py::arg("input"), py::arg("base_input"), docs::can_parse_base);

    py::class_<ada::url_aggregator>(m, "URL", docs::url)
        .def_prop_rw("hash", &ada::url_aggregator::get_hash, &ada::url_aggregator::set_hash, "Fragment including ``#``, or an empty string. Assign an empty string to remove it.")
        .def_prop_rw("host", &ada::url_aggregator::get_host, &ada::url_aggregator::set_host, "Hostname and optional port.")
        .def_prop_rw("hostname", &ada::url_aggregator::get_hostname, &ada::url_aggregator::set_hostname, "Hostname without the port.")
        .def_prop_rw("href", &ada::url_aggregator::get_href, &ada::url_aggregator::set_href, "Complete, normalized URL.")
        .def_prop_ro("origin", &ada::url_aggregator::get_origin, "Serialized origin, or ``null`` for an opaque origin.")
        .def_prop_rw("password", &ada::url_aggregator::get_password, &ada::url_aggregator::set_password, "Percent-encoded password.")
        .def_prop_rw("pathname", &ada::url_aggregator::get_pathname, &ada::url_aggregator::set_pathname, "Percent-encoded path.")
        .def_prop_ro("pathname_length", &ada::url_aggregator::get_pathname_length, "Path length in UTF-8 bytes.")
        .def_prop_rw("port", &ada::url_aggregator::get_port, &ada::url_aggregator::set_port, "Port string, or empty if absent. Assign an empty string to remove it.")
        .def_prop_rw("protocol", &ada::url_aggregator::get_protocol, &ada::url_aggregator::set_protocol, "Scheme including the trailing colon.")
        .def_prop_rw("search", &ada::url_aggregator::get_search, &ada::url_aggregator::set_search, "Query including ``?``, or an empty string. Assign an empty string to remove it.")
        .def_prop_rw("username", &ada::url_aggregator::get_username, &ada::url_aggregator::set_username, "Percent-encoded username.")

        .def("has_credentials", &ada::url_aggregator::has_credentials, "Return whether the username or password is non-empty.")
        .def("has_empty_hostname", &ada::url_aggregator::has_empty_hostname, "Return whether a hostname is present but empty.")
        .def("has_hash", &ada::url_aggregator::has_hash, "Return whether a fragment is present, including an empty fragment.")
        .def("has_hostname", &ada::url_aggregator::has_hostname, "Return whether a hostname is present, including an empty hostname.")
        .def("has_non_empty_password", &ada::url_aggregator::has_non_empty_password, "Return whether the password is non-empty.")
        .def("has_non_empty_username", &ada::url_aggregator::has_non_empty_username, "Return whether the username is non-empty.")
        .def("has_password", &ada::url_aggregator::has_password, "Return whether a password component is present.")
        .def("has_port", &ada::url_aggregator::has_port, "Return whether an explicit, non-default port is present.")
        .def("has_search", &ada::url_aggregator::has_search, "Return whether a query is present, including an empty query.")
        .def("has_valid_domain", &ada::url_aggregator::has_valid_domain, "Check domain and label lengths according to RFC 1034.")

        .def("to_diagram", &ada::url_aggregator::to_diagram, "Return a diagram of the URL component boundaries.")
        .def("__str__", &ada::url_aggregator::get_href, "Return the normalized URL.")
        .def("validate", &ada::url_aggregator::validate, "Check internal component-offset consistency for debugging.")

        .def("__add__", [](const ada::url_aggregator &self, std::string_view other) {
          ada::result<ada::url_aggregator> url = ada::parse<ada::url_aggregator>(other, &self);
          if (!url) {
              throw py::value_error("URL could not be parsed.");
          }
          return url.value();
        }, docs::add);

    py::class_<ada::url_search_params_keys_iter>(m, "URLSearchParamsKeysIter", "Iterator over query parameter keys.")
        .def("__iter__", [](ada::url_search_params_keys_iter &self) {
            return &self;
        }, "Return this iterator.")
        .def("__next__", [](ada::url_search_params_keys_iter &self) {
            if (self.has_next()) {
                return self.next();
            } else {
                throw py::stop_iteration();
            }
        }, docs::next);

    py::class_<ada::url_search_params_values_iter>(m, "URLSearchParamsValuesIter", "Iterator over query parameter values.")
        .def("__iter__", [](ada::url_search_params_values_iter &self) {
            return &self;
        }, "Return this iterator.")
        .def("__next__", [](ada::url_search_params_values_iter &self) {
            if (self.has_next()) {
                return self.next();
            } else {
                throw py::stop_iteration();
            }
        }, docs::next);

    py::class_<ada::url_search_params>(m, "URLSearchParams", docs::search_params)
        .def(py::init<>(), "Create an empty parameter collection.")
        .def(py::init<const std::string_view>(), "Parse a query string with an optional leading question mark.")
        .def("get", &ada::url_search_params::get, "Return the first value for a key, or ``None`` if absent.")
        .def("get_all", &ada::url_search_params::get_all, "Return all values for a key in insertion order.")
        .def("has", [](ada::url_search_params &self, std::string_view key, std::optional<std::string_view> value) {
            if (value) {
                return self.has(key, value.value());
            }
            return self.has(key);
        }, py::arg("key"), py::arg("value") = py::none(), docs::has)
        .def("append", &ada::url_search_params::append, "Append a key-value pair, preserving duplicate keys.")
        .def("remove", [](ada::url_search_params &self, std::string_view key, std::optional<std::string_view> value) {
            if (value) {
                self.remove(key, value.value());
                return;
            }
            self.remove(key);
        }, py::arg("key"), py::arg("value") = py::none(), docs::remove)
        .def("copy", [](ada::url_search_params &self) {
            return ada::url_search_params(self);
        }, "Return an independent copy of the parameters.")
        .def("sort", &ada::url_search_params::sort, "Sort stably by key using UTF-16 code-unit order.")
        .def("size", &ada::url_search_params::size, "Return the number of pairs, including duplicate keys.")
        .def("keys", [](ada::url_search_params &self) {
            return self.get_keys();
        }, py::rv_policy::reference_internal, "Return an iterator over keys, including duplicates.")
        .def("values", [](ada::url_search_params &self) {
            return self.get_values();
        }, py::rv_policy::reference_internal, "Return an iterator over values, including duplicates.")
        .def("__str__", &ada::url_search_params::to_string, "Serialize as a percent-encoded query string without a leading question mark.")
        .def("__getitem__", [](ada::url_search_params &self, std::string_view key) {
            auto v = self.get(key);
            if (v) {
                return v.value();
            }
            throw py::key_error("Key not found.");
        }, docs::getitem)
        .def("__setitem__", &ada::url_search_params::set, "Set a key to one value, replacing all existing values for that key.")
        .def("__delitem__", [](ada::url_search_params &self, std::string_view key) {
            self.remove(key);
        }, "Remove all pairs for a key; missing keys are ignored.")
        .def("__len__", &ada::url_search_params::size, "Return the number of pairs, including duplicate keys.")
        .def("__contains__", [](ada::url_search_params &self, std::string_view key) {
            return self.has(key);
        }, "Return whether a key is present.")
        .def("__iter__", [](ada::url_search_params &self) {
            return py::make_iterator(py::type<ada::url_search_params>(), "Iterator", self.begin(), self.end());
        }, "Return an iterator over key-value pairs, including duplicates.");

    m.def("idna_decode", [](py::bytes input) -> py::str {
        auto result = ada::idna::to_unicode(input.c_str());
        return py::str(result.c_str());
    }, "Decode an ASCII domain supplied as bytes to Unicode.");

    m.def("idna_encode", [](const std::string_view input) -> py::bytes {
        auto result = ada::idna::to_ascii(input);
        return py::bytes(result.c_str(), result.size());
    }, docs::idna_encode);

    m.def("parse", [](std::string_view input) {
        ada::result<ada::url_aggregator> url = ada::parse<ada::url_aggregator>(input);
        if (!url) {
            throw py::value_error("URL could not be parsed.");
        }
        return std::move(*url);
    }, docs::parse);

}
