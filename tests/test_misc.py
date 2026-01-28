import can_ada


def test_url_join():
    """
    Ensures we can join URLs together.
    """

    assert str(can_ada.parse("http://a/b/c/d;p?q") + "../g") == "http://a/b/g"
