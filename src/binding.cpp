#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/string_view.h>
#include <nanobind/stl/vector.h>
#include "ada.h"

namespace py = nanobind;

struct parse_impl_result {
    std::string scheme;
    std::string netloc;
    std::string path;
    std::string params;
    std::string query;
    std::string fragment;
};

static parse_impl_result parse_compat_impl(std::string_view input);

NB_MODULE(can_ada, m) {
#ifdef VERSION_INFO
    m.attr("__version__") = Py_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif

    m.def("can_parse", [](std::string_view input) {
        return ada::can_parse(input);
    }, py::arg("input"));

    m.def("can_parse", [](std::string_view input, std::string_view base_input) {
        return ada::can_parse(input, &base_input);
    }, py::arg("input"), py::arg("base_input"));

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
        .def("validate", &ada::url_aggregator::validate)

        .def("__add__", [](const ada::url_aggregator &self, std::string_view other) {
          ada::result<ada::url_aggregator> url = ada::parse<ada::url_aggregator>(other, &self);
          if (!url) {
              throw py::value_error("URL could not be parsed.");
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
                throw py::stop_iteration();
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
                throw py::stop_iteration();
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
        }, py::rv_policy::reference_internal)
        .def("values", [](ada::url_search_params &self) {
            return self.get_values();
        }, py::rv_policy::reference_internal)
        .def("__str__", &ada::url_search_params::to_string)
        .def("__getitem__", [](ada::url_search_params &self, std::string_view key) {
            auto v = self.get(key);
            if (v) {
                return v.value();
            }
            throw py::key_error("Key not found.");
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
            return py::make_iterator(py::type<ada::url_search_params>(), "Iterator", self.begin(), self.end());
        });

    m.def("idna_decode", [](py::bytes input) -> py::str {
        auto result = ada::idna::to_unicode(input.c_str());
        return py::str(result.c_str());
    });

    m.def("idna_encode", [](const std::string_view input) -> py::bytes {
        auto result = ada::idna::to_ascii(input);
        return py::bytes(result.c_str(), result.size());
    });

    m.def("parse", [](std::string_view input) {
        ada::result<ada::url_aggregator> url = ada::parse<ada::url_aggregator>(input);
        if (!url) {
            throw py::value_error("URL could not be parsed.");
        }
        return std::move(*url);
    });

    auto urllib = py::module_::import_("urllib.parse");

    auto ParseResult = py::object(urllib.attr("ParseResult"));
    auto ParseResultBytes = py::object(urllib.attr("ParseResultBytes"));

    m.def("parse_compat", [ParseResult](std::string_view input) {
        auto [scheme, netloc, path, params, query, fragment] =
            parse_compat_impl(input);
        return ParseResult(scheme, netloc, path, params, query, fragment);
    });

    m.def("parse_compat", [ParseResultBytes](py::bytes input) {
        auto [scheme, netloc, path, params, query, fragment] =
            parse_compat_impl(std::string_view(input.c_str(), input.size()));
        return ParseResultBytes(py::bytes(scheme.data(), scheme.size()),
                                py::bytes(netloc.data(), netloc.size()),
                                py::bytes(path.data(), path.size()),
                                py::bytes(params.data(), params.size()),
                                py::bytes(query.data(), query.size()),
                                py::bytes(fragment.data(), fragment.size()));
    });
}

static parse_impl_result parse_compat_impl(std::string_view input) {
    auto result = ada::parse<ada::url_aggregator>(input);
    if (!result) {
        throw py::value_error("URL could not be parsed.");
    }

    auto url = std::move(*result);

    auto scheme = url.get_protocol();
    if (!scheme.empty() && scheme.back() == ':') {
        scheme.remove_suffix(1);
    }

    std::string netloc;
    {
        if (url.has_non_empty_username()) {
            netloc.append(url.get_username());
            if (url.has_password()) {
                netloc.push_back(':');
                netloc.append(url.get_password());
            }
            netloc.push_back('@');
        }

        netloc.append(url.get_host());

        if (url.has_port()) {
            netloc.push_back(':');
            netloc.append(url.get_port());
        }
    }

    auto raw_path = url.get_pathname();
    auto path = raw_path;
    std::string_view params{};

    auto last_slash = raw_path.rfind('/');
    auto last_segment = (last_slash != std::string_view::npos)
                            ? raw_path.substr(last_slash + 1)
                            : raw_path;

    auto semi = last_segment.find(';');
    if (semi != std::string_view::npos) {
        path = (last_slash != std::string_view::npos)
                   ? raw_path.substr(0, last_slash + 1 + semi)
                   : last_segment.substr(0, semi);
        params = last_segment.substr(semi + 1);
    }

    auto query = url.get_search();
    if (!query.empty() && query.front() == '?') {
        query.remove_prefix(1);
    }

    auto fragment = url.get_hash();
    if (!fragment.empty() && fragment.front() == '#') {
        fragment.remove_prefix(1);
    }

    return {std::string(scheme), std::move(netloc),  std::string(path),
            std::string(params), std::string(query), std::string(fragment)};
}
