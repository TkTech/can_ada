from typing import Callable, Any
from collections.abc import Iterable
import pytest
import urllib.parse

ada_url = pytest.importorskip("ada_url")
can_ada = pytest.importorskip("can_ada")
yarl = pytest.importorskip("yarl")


@pytest.mark.slow
def test_urllib_parse(benchmark: Callable[[Any], Any], top100str: Iterable[str]):
    def urllib_parse():
        for line in top100str:
            urllib.parse.urlparse(line)

    benchmark(urllib_parse)


@pytest.mark.slow
def test_ada_python_parse(benchmark: Callable[[Any], Any], top100str: Iterable[str]):
    def ada_python_parse():
        for line in top100str:
            try:
                ada_url.URL(line)
            except ValueError:
                # There are a small number of URLs in the sample data that are
                # not valid WHATWG URLs.
                pass

    benchmark(ada_python_parse)


@pytest.mark.slow
def test_can_ada_parse(benchmark: Callable[[Any], Any], top100str: Iterable[str]):
    def can_ada_parse():
        for line in top100str:
            try:
                can_ada.parse(line)
            except ValueError:
                # There are a small number of URLs in the sample data that are
                # not valid WHATWG URLs.
                pass

    benchmark(can_ada_parse)


@pytest.mark.slow
def test_yarl_parse(benchmark: Callable[[Any], Any], top100str: Iterable[str]):
    def yarl_parse():
        for line in top100str:
            try:
                yarl.URL(line)
            except ValueError:
                # There are a small number of URLs in the sample data that are
                # not valid WHATWG URLs.
                pass

    benchmark(yarl_parse)


@pytest.mark.slow
def test_can_ada_parse_compat_str(
    benchmark: Callable[[Any], Any], top100str: Iterable[str]
):
    def can_ada_parse_compat():
        for line in top100str:
            try:
                can_ada.parse_compat(line)
            except ValueError:
                # There are a small number of URLs in the sample data that are
                # not valid WHATWG URLs.
                pass

    benchmark(can_ada_parse_compat)


@pytest.mark.slow
def test_can_ada_parse_compat_bytes(
    benchmark: Callable[[Any], Any], top100bytes: Iterable[str]
):
    def can_ada_parse_compat():
        for line in top100bytes:
            try:
                can_ada.parse_compat(line)
            except ValueError:
                # There are a small number of URLs in the sample data that are
                # not valid WHATWG URLs.
                pass

    benchmark(can_ada_parse_compat)
