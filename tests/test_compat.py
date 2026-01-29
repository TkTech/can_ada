import pytest
import can_ada
import urllib.parse


@pytest.mark.xfail(reason="parse_compat is not 100% urllib-compatible yet")
def test_urllib_parse_str_matches(subtests: pytest.Subtests, top100str: list[str]):
    for line in top100str:
        assert urllib.parse.urlparse(line) == can_ada.parse_compat(line)


@pytest.mark.xfail(reason="parse_compat is not 100% urllib-compatible yet")
def test_urllib_parse_bytes_matches(
    subtests: pytest.Subtests, top100bytes: list[bytes]
):
    for line in top100bytes:
        assert urllib.parse.urlparse(line) == can_ada.parse_compat(line)
