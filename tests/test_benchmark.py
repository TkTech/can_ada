from functools import lru_cache
from pathlib import Path

import pytest
import urllib.parse

ada_url = pytest.importorskip("ada_url")
can_ada = pytest.importorskip("can_ada")


@lru_cache
def data() -> list[str]:
    current_file_dir = Path(__file__).parent
    with open(current_file_dir / "data" / "top100.txt", "r") as f:
        return f.readlines()


def urllib_parse():
    for line in data():
        urllib.parse.urlparse(line)


def ada_python_parse():
    for line in data():
        try:
            ada_url.URL(line)
        except ValueError:
            # There are a small number of URLs in the sample data that are
            # not valid WHATWG URLs.
            pass


def can_ada_parse():
    for line in data():
        try:
            can_ada.parse(line)
        except ValueError:
            # There are a small number of URLs in the sample data that are
            # not valid WHATWG URLs.
            pass


@pytest.mark.slow
def test_urllib_parse(benchmark):
    benchmark(urllib_parse)


@pytest.mark.slow
def test_ada_python_parse(benchmark):
    benchmark(ada_python_parse)


@pytest.mark.slow
def test_can_ada_parse(benchmark):
    benchmark(can_ada_parse)
