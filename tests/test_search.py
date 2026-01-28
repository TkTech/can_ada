import pytest

import can_ada


def test_get_and_set():
    """
    Ensure we can get & ste the various parts of the URLSearchParams.
    """
    params = can_ada.URLSearchParams("?a=1&b=2&c=3")
    assert params["a"] == "1"
    assert params["b"] == "2"
    assert params["c"] == "3"

    with pytest.raises(KeyError):
        params["d"]

    assert params.get("a") == "1"
    assert params.get("d") is None

    assert "a" in params
    assert "d" not in params

    assert params.has("a") is True
    assert params.has("d") is False

    params.remove("a")
    assert params.has("a") is False
    assert params.get("a") is None


def test_get_all_and_set():
    """
    Ensure we can get & set multiple values for a single key.
    """
    params = can_ada.URLSearchParams("?a=1&b=2&c=3&c=4")
    assert params.get_all("a") == ["1"]
    assert params.get_all("b") == ["2"]
    assert params.get_all("c") == ["3", "4"]

    params["a"] = "2"
    assert params.get_all("a") == ["2"]

    params["c"] = "5"
    assert params.get_all("c") == ["5"]
    params.append("c", "6")
    assert params.get_all("c") == ["5", "6"]

    params.remove("c", "6")
    assert params.get_all("c") == ["5"]

    params.remove("c", "5")
    assert params.get_all("c") == []

    params.append("c", "5")
    params.append("c", "6")
    assert params.has("c", "5") is True
    assert params.has("c", "6") is True
    assert params.has("c", "7") is False


def test_size():
    """
    Ensure we can get the size of the URLSearchParams.
    """
    params = can_ada.URLSearchParams("?a=1&b=2&c=3")
    assert len(params) == 3


def test_copy():
    """
    Ensure we can copy a URLSearchParams object.
    """
    params_1 = can_ada.URLSearchParams("?a=1&b=2&c=3")
    params_2 = params_1.copy()

    # Ensure the copy isn't sharing memory
    assert params_2["a"] == "1"
    params_1["a"] = "2"
    assert params_2["a"] == "1"


def test_sort():
    """
    Ensure we can sort the URLSearchParams.
    """
    params = can_ada.URLSearchParams("?c=3&b=2&a=1")
    assert list(params.keys()) == ["c", "b", "a"]
    params.sort()
    assert list(params.keys()) == ["a", "b", "c"]


def test_iterators():
    """
    Ensure we can iterate over the URLSearchParams.
    """
    params = can_ada.URLSearchParams("?a=1&b=2&c=3")
    assert list(params) == [("a", "1"), ("b", "2"), ("c", "3")]
    assert list(params.keys()) == ["a", "b", "c"]
    assert list(params.values()) == ["1", "2", "3"]
