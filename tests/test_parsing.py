import can_ada


def test_can_parse():
    assert can_ada.can_parse("/", "https://tkte.ch") is True
    assert can_ada.can_parse("/", "/tkte.ch") is False


def test_parse():
    """
    Ensure we can simply call parse on a valid URL without crashing.
    """
    can_ada.parse("https://tkte.ch")


def test_port():
    """
    Ensure we can get and set the port.
    """
    url = can_ada.parse("https://tkte.ch")
    assert url.has_port() is False
    url.port = "9090"
    assert url.port == "9090"
